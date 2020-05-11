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

#ifndef SD_CUDA_EXCEPTION_H
#define SD_CUDA_EXCEPTION_H

#include <system/dll.h>

#include <stdexcept>
#include <string>

#if defined(_MSC_VER)

// we're ignoring warning about non-exportable parent class, since
// std::runtime_error is a part of Standard C++ Library
#pragma warning(disable : 4275)

#endif

namespace sd {
class SD_EXPORT cuda_exception : public std::runtime_error {
 public:
  cuda_exception(std::string message);
  ~cuda_exception() = default;

  static cuda_exception build(std::string message, int errorCode);
};
}  // namespace sd

#endif  // SD_CUDA_EXCEPTION_H
