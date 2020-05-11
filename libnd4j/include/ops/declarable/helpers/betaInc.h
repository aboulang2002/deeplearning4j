/*******************************************************************************
 * Copyright (c) 2015-2018 Skymind, Inc.
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
// Created by Yurii Shyrma on 11.12.2017
//

#ifndef LIBND4J_BETAINC_H
#define LIBND4J_BETAINC_H

#include <ops/declarable/helpers/helpers.h>

#include "array/NDArray.h"

namespace sd {
namespace ops {
namespace helpers {

const uint maxIter =
    MAX_NUM_THREADS /*articles propose 10000*/;  // max number of loop
                                                 // iterations in function for
                                                 // continued fractions

void betaInc(sd::LaunchContext* context, const NDArray& a, const NDArray& b,
             const NDArray& x, NDArray& output);

}  // namespace helpers
}  // namespace ops
}  // namespace sd

#endif  // LIBND4J_BETAINC_H