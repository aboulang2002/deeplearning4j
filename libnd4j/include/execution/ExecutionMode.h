/*******************************************************************************
 * Copyright (c) 2019-2020 Konduit K.K
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
// @author raver119@gmail.com
//

#ifndef SD_EXECUTIONMODE_H
#define SD_EXECUTIONMODE_H

namespace samediff {
enum ExecutionMode {
  MODE_UNDEFINED = 0,
  MODE_TRAINING = 1,
  MODE_INFERENCE = 2,
};
}

#endif  // SD_EXECUTIONMODE_H
