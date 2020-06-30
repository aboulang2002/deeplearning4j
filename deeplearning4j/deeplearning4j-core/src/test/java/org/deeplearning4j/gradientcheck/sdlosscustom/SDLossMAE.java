/*******************************************************************************
 * Copyright (c) 2020 Konduit K.K.
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
package org.deeplearning4j.gradientcheck.sdlosscustom;

import lombok.EqualsAndHashCode;
import org.nd4j.autodiff.samediff.SDVariable;
import org.nd4j.autodiff.samediff.SameDiff;
import org.nd4j.linalg.lossfunctions.BaseSameDiffLoss;
import org.nd4j.linalg.lossfunctions.SameDiffNonFusedLoss;

@EqualsAndHashCode(callSuper = false)
public class SDLossMAE extends SameDiffNonFusedLoss {

    @Override
    public SDVariable defineLossArray(SameDiff sameDiff, SDVariable layerInput, SDVariable labels) {
        return sameDiff.math.abs(labels.sub(layerInput)).mean(1);
    }
}
