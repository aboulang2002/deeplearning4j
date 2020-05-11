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
#ifndef __MAX_POOLING_HELPERS__
#define __MAX_POOLING_HELPERS__
#include <array/NDArray.h>
#include <graph/Context.h>
#include <system/op_boilerplate.h>

namespace sd {
namespace ops {
namespace helpers {

void maxPoolingFunctor(sd::LaunchContext* context, sd::graph::Context& block,
                       NDArray* input, NDArray* values,
                       std::vector<int> const& params, NDArray* indices);
}
}  // namespace ops
}  // namespace sd
#endif
