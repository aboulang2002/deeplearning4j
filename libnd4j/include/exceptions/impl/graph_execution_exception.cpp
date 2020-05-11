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
// Created by raver on 8/31/2018.
//

#include <exceptions/graph_execution_exception.h>
#include <helpers/StringUtils.h>

namespace sd {
graph_execution_exception::graph_execution_exception(Nd4jLong graphId)
    : graph_exception(StringUtils::buildGraphErrorMessage(
                          "Caught exception during graph execution", graphId),
                      graphId) {
  _graphId = graphId;
}

graph_execution_exception::graph_execution_exception(const std::string &message,
                                                     Nd4jStatus status)
    : graph_exception(message, status) {
  //
}
}  // namespace sd
