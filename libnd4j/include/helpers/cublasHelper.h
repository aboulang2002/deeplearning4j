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
// @author raver119@gmail.com
//

#ifndef SD_CUBLASHELPER_H
#define SD_CUBLASHELPER_H

#include <system/dll.h>
#include <system/pointercast.h>

#include <mutex>
#include <vector>

namespace sd {
class SD_EXPORT CublasHelper {
 private:
  static CublasHelper* _INSTANCE;
  static std::mutex _mutex;

  std::vector<void*> _cache;
  std::vector<void*> _solvers;
  std::vector<void*> _cudnn;

  CublasHelper();
  ~CublasHelper();

 public:
  static CublasHelper* getInstance();

  void* cudnn();
  void* solver();

  void* handle();
  void* handle(int deviceId);
};
}  // namespace sd

#endif  // SD_CUBLASHELPER_H
