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

#include <helpers/DebugHelper.h>
#include <loops/legacy_ops.h>
#include <loops/transform_any.h>
#include <system/Environment.h>
#include <system/op_boilerplate.h>
#include <types/types.h>

using namespace simdOps;

template <typename X, typename Z, typename OpType>
__global__ void transformAnySimple(const void *x, const Nd4jLong *xShapeInfo,
                                   int xRank, void *params, void *z,
                                   const Nd4jLong *zShapeInfo, int zRank,
                                   int *allocationPointer,
                                   void *reductionPointer,
                                   const Nd4jLong *tadShapeInfo,
                                   const Nd4jLong *tadOffsets) {
  functions::transform::TransformAny<X, Z>::template transformCuda<OpType>(
      x, xShapeInfo, params, z, zShapeInfo, allocationPointer, reductionPointer,
      tadShapeInfo, tadOffsets);
}

namespace functions {
namespace transform {

template <typename X, typename Y>
_CUDA_H void TransformAny<X, Y>::executeTransformShaped(
    dim3 launchDims, cudaStream_t *stream, const int opNum, const void *x,
    const Nd4jLong *xShape, int xRank, void *extraParams, void *z,
    const Nd4jLong *zShape, int zRank, int *allocationPointer,
    void *reductionPointer, const Nd4jLong *tadShapeInfo,
    const Nd4jLong *tadOffsets) {
  DISPATCH_BY_OPNUM_TT(intermediateShaped,
                       PARAMS(launchDims, stream, x, xShape, xRank, extraParams,
                              z, zShape, zRank, allocationPointer,
                              reductionPointer, tadShapeInfo, tadOffsets),
                       TRANSFORM_ANY_OPS);

  DEBUG_KERNEL(stream, opNum);
}

template <typename X, typename Z>
template <typename OpType>
__device__ void TransformAny<X, Z>::transformCuda(
    const void *vx, const Nd4jLong *xShapeInfo, void *vparams, void *vz,
    const Nd4jLong *zShapeInfo, int *allocationPointer, void *vreductionPointer,
    const Nd4jLong *tadShapeInfo, const Nd4jLong *tadOffsets) {
  auto x = reinterpret_cast<const X *>(vx);
  auto z = reinterpret_cast<Z *>(vz);
  auto params = reinterpret_cast<X *>(vparams);
  auto reductionPointer = reinterpret_cast<Z *>(vreductionPointer);

  __shared__ Nd4jLong xEws;
  __shared__ Nd4jLong zEws;
  __shared__ char xOrder;
  __shared__ char zOrder;
  __shared__ Nd4jLong length;

  if (threadIdx.x == 0) {
    xEws = shape::elementWiseStride(xShapeInfo);
    zEws = shape::elementWiseStride(zShapeInfo);
    xOrder = shape::order(xShapeInfo);
    zOrder = shape::order(zShapeInfo);
    length = shape::length(xShapeInfo);
  }
  __syncthreads();

  auto tid = blockIdx.x * blockDim.x + threadIdx.x;
  int totalThreads = gridDim.x * blockDim.x;

  if (xEws > 0 && zEws > 0 && xOrder == zOrder && xOrder == 'c') {
    for (int i = tid; i < length; i += totalThreads)
      z[i * zEws] = OpType::op(x[i * xEws], params);
  } else {
    if (vx == vz) {
      for (Nd4jLong i = tid; i < length; i += totalThreads) {
        auto xOffset = shape::getIndexOffset(i, xShapeInfo);
        z[xOffset] = OpType::op(x[xOffset], params);
      }
    } else {
      for (Nd4jLong i = tid; i < length; i += totalThreads) {
        auto xOffset = shape::getIndexOffset(i, xShapeInfo);
        auto zOffset = shape::getIndexOffset(i, zShapeInfo);
        z[zOffset] = OpType::op(x[xOffset], params);
      }
    }
  }
};

template <typename X, typename Z>
template <typename OpType>
_CUDA_H void TransformAny<X, Z>::intermediateShaped(
    dim3 launchDims, cudaStream_t *stream, const void *x,
    const Nd4jLong *xShape, int xRank, void *extraParams, void *z,
    const Nd4jLong *zShape, int zRank, int *allocationPointer,
    void *reductionPointer, const Nd4jLong *tadShapeInfo,
    const Nd4jLong *tadOffsets) {
  transformAnySimple<X, Z, OpType>
      <<<launchDims.x, launchDims.y, launchDims.z, *stream>>>(
          x, xShape, xRank, extraParams, z, zShape, zRank, allocationPointer,
          reductionPointer, tadShapeInfo, tadOffsets);

  sd::DebugHelper::checkErrorCode(stream, "transformAny(...) failed");
}

BUILD_DOUBLE_TEMPLATE(template class SD_EXPORT TransformAny, , LIBND4J_TYPES,
                      LIBND4J_TYPES);
}  // namespace transform
}  // namespace functions
