/* *****************************************************************************
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

package org.nd4j.linalg.activations;

import lombok.NonNull;
import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.linalg.api.ndarray.INDArray;

import java.util.Arrays;

/**
 * Base IActivation for activation functions without parameters
 *
 * @author Alex Black
 */
public abstract class BaseActivationFunction implements IActivation {

    @Override
    public int numParams(int inputSize) {
        return 0;
    }

    protected void assertShape(INDArray in, INDArray epsilon){
        if(!in.equalShapes(epsilon)){
            throw new IllegalStateException("Shapes must be equal during backprop: in.shape{} = " + Arrays.toString(in.shape())
                    + ", epsilon.shape() = " + Arrays.toString(epsilon.shape()));
        }
    }

    public
    @NonNull SDVariable defineActivation(@NonNull SameDiff sameDiff, @NonNull SDVariable input){
        throw new UnsupportedOperationException("SameDiff conversion has not been implemented for " + this.getClass().getSimpleName());
    }
}
