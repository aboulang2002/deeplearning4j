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
// Created by raver119 on 30.01.18.
//

#ifndef SD_LOGICEXIT_H
#define SD_LOGICEXIT_H

#include <graph/Graph.h>
#include <system/pointercast.h>

namespace sd {
namespace graph {
class LogicExit {
 public:
  static Nd4jStatus processNode(Graph* graph, Node* node);
};
}  // namespace graph
}  // namespace sd

#endif  // LIBND4J_LOGICEXIT_H
