/*******************************************************************************
 * Copyright (c) 2020 Konduit, K.K.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Apache License, Version 2.0 which is available at
 * https://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 ******************************************************************************/

//
//  @author GS <sgazeos@gmail.com>
//
#include <op_boilerplate.h>
#include <NDArray.h>
#include <execution/Threads.h>
#include <MmulHelper.h>

#include "../lup.h"
#include "../triangular_solve.h"
#include "../lstsq.h"
#include "../qr.h"

namespace nd4j {
namespace ops {
namespace helpers {

    template <typename T>
    int leastSquaresSolveFunctor_(nd4j::LaunchContext* context, NDArray const* leftInput, NDArray const* rightInput, double const l2Regularizer, bool const fast, NDArray* output) {
        if (fast) { // Cholesky decomposition approach
            // Equation for solve A^T * Ax = A^T * b, so
            // 1. Computing A2:
            NDArray leftOutput(leftInput, false, context); leftOutput.nullify();
            MmulHelper::matmul(leftInput, leftInput, &leftOutput, true, false); // Computing A2 = A^T * A
            // 2. Computing B' = A^T * b
            auto rightOutput = rightInput->ulike();
            MmulHelper::matmul(leftInput, rightInput, &rightOutput, true, false); // Computing B' = A^T * b
            // 3. due l2Regularizer = 0, skip regularization ( indeed A' = A2 - l2Regularizer * I)
//            auto regularizer = leftOutput.ulike();
//            regularizer.setIdentity();
//            regularizer *= l2Regularizer;
//            leftOutput -= regularizer;
//
            // 4. Cholesky decomposition -- output matrix is square and lower triangular
            helpers::cholesky(context, &leftOutput, &leftOutput, true); // inplace decomposition
            leftOutput.printIndexedBuffer("Left OUTPUT (L matrix)");
            // 5. Solve two triangular systems:
            helpers::triangularSolveFunctor(context, &leftOutput, &rightOutput, true, false, &rightOutput);
            leftOutput.transposei();leftOutput.printIndexedBuffer("Left OUTPUT transposed");
            helpers::triangularSolveFunctor(context, &leftOutput, &rightOutput, false, false, output);
            // All done
        }
        else { // QR decomposition approach
            // Equation for solve Rx = Q^T * b, where A = Q * R, where Q - orthogonal matrix, and R - upper triangular
            // 1. QR decomposition
            auto Q = leftInput->ulike();
            auto R = rightInput->ulike();
            helpers::qr(context, leftInput, &Q, &R, false);
            // 2. b` = Q^t * b:
            auto rightOutput = rightInput->ulike();
            MmulHelper::matmul(&Q, rightInput, &rightOutput, true, false);
            // 3. Solve triangular system
            helpers::triangularSolveFunctor(context, &R, &rightOutput, false, false, output);
        }
        return Status::OK();
    }

    int leastSquaresSolveFunctor(nd4j::LaunchContext* context, NDArray const* leftInput, NDArray const* rightInput, double const l2Regularizer, bool const fast, NDArray* output) {
        BUILD_SINGLE_SELECTOR(leftInput->dataType(), return leastSquaresSolveFunctor_, (context, leftInput, rightInput, l2Regularizer, fast, output), FLOAT_TYPES);
    }

}
}
}
