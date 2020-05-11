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

#ifndef SD_SHAPEBUILDERS_H
#define SD_SHAPEBUILDERS_H

#include <array/ArrayOptions.h>
#include <array/DataType.h>
#include <helpers/shape.h>
#include <memory/Workspace.h>
#include <system/pointercast.h>

#include <vector>

namespace sd {
class SD_EXPORT ShapeBuilders {
 public:
  static Nd4jLong* createScalarShapeInfo(
      sd::DataType dataType, sd::memory::Workspace* workspace = nullptr);

  static Nd4jLong* createVectorShapeInfo(
      const sd::DataType dataType, const Nd4jLong length,
      sd::memory::Workspace* workspace = nullptr);

  /**
   *   create shapeInfo for given order basing on shape stored in shapeOnly
   * vector memory allocation for shapeInfo is on given workspace
   */
  static Nd4jLong* createShapeInfo(const sd::DataType dataType,
                                   const char order, int rank,
                                   const Nd4jLong* shapeOnly,
                                   memory::Workspace* workspace = nullptr);
  static Nd4jLong* createShapeInfo(const sd::DataType dataType,
                                   const char order,
                                   const std::vector<Nd4jLong>& shapeOnly,
                                   memory::Workspace* workspace = nullptr);
  static Nd4jLong* createShapeInfo(
      const sd::DataType dataType, const char order,
      const std::initializer_list<Nd4jLong>& shapeOnly,
      memory::Workspace* workspace = nullptr);

  /**
   *   allocates memory for new shapeInfo and copy all information from
   * inShapeInfo to new shapeInfo if copyStrides is false then strides for new
   * shapeInfo are recalculated
   */
  static Nd4jLong* copyShapeInfo(const Nd4jLong* inShapeInfo,
                                 const bool copyStrides,
                                 memory::Workspace* workspace = nullptr);
  static Nd4jLong* copyShapeInfoAndType(const Nd4jLong* inShapeInfo,
                                        const DataType dtype,
                                        const bool copyStrides,
                                        memory::Workspace* workspace = nullptr);
  static Nd4jLong* copyShapeInfoAndType(const Nd4jLong* inShapeInfo,
                                        const Nd4jLong* shapeInfoToGetTypeFrom,
                                        const bool copyStrides,
                                        memory::Workspace* workspace = nullptr);

  /**
   * allocates memory for new shapeInfo and copy all information from
   * inShapeInfo to new shapeInfo except dimensions in dimsToExclude (unit
   * dimensions) and corresponding strides for example inShapeInfo is {3,
   * 2,1,3,1,4,  12,12,4,4,1, 16384,1,99}, dimsToExclude = {2,3}, dimsSize = 2
   * then outShapeInfo will contain {3, 2,3,4, 12,4,1, 16384,1,99}
   */
  static Nd4jLong* copyShapeInfoWithoutUnites(
      const Nd4jLong* inShapeInfo, const int dimsSize, const int* dimsToExclude,
      memory::Workspace* workspace = nullptr);

  static Nd4jLong* emptyShapeInfo(const sd::DataType dataType,
                                  memory::Workspace* workspace = nullptr);

  static Nd4jLong* emptyShapeInfo(const sd::DataType dataType, const char order,
                                  const std::vector<Nd4jLong>& shape,
                                  memory::Workspace* workspace = nullptr);
};
}  // namespace sd

#endif  // SD_SHAPEBUILDERS_H
