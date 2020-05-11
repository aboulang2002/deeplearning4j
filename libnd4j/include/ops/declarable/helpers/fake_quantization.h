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
//  @author sgazeos@gmail.com
//
#ifndef __FAKE_QUANTIZATION_H_HELPERS__
#define __FAKE_QUANTIZATION_H_HELPERS__
#include <array/NDArray.h>
#include <system/op_boilerplate.h>

namespace sd {
namespace ops {
namespace helpers {

void fakeQuantWithMinMaxVars(NDArray* input, NDArray* min, NDArray* max,
                             int numBits, bool narrowed, NDArray* output);
void fakeQuantWithMinMaxVarsPerChannel(LaunchContext* context, NDArray* input,
                                       NDArray* min, NDArray* max, int numBits,
                                       bool narrowed, NDArray* output);
}  // namespace helpers
}  // namespace ops
}  // namespace sd
#endif
