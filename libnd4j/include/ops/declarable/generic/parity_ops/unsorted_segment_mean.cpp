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
// Created by george@skymind.io on 9/6/2018.
//

#include <ops/declarable/CustomOperations.h>
#include <ops/declarable/helpers/segment.h>

namespace sd {
namespace ops {
CUSTOM_OP_IMPL(unsorted_segment_mean, 2, 1, false, 0, 0) {
  auto input = INPUT_VARIABLE(0);
  auto idxSegments = INPUT_VARIABLE(1);
  auto segmentedOutput = OUTPUT_NULLIFIED(0);
  Nd4jLong numOfClasses =
      block.width() == 3 ? INPUT_VARIABLE(2)->e<Nd4jLong>(0) : INT_ARG(0);

  REQUIRE_TRUE(idxSegments->isVector(), 0,
               "unsorted_segment_mean: segment indexes array should be a "
               "vector, but it rank is %i.",
               idxSegments->rankOf());
  REQUIRE_TRUE(idxSegments->lengthOf() == input->sizeAt(0), 0,
               "unsorted_segment_mean: segment indexes array length should be "
               "equal to the input first dimension, but %ld != %ld.",
               idxSegments->lengthOf(), input->sizeAt(0));

  Nd4jLong wrong;

  REQUIRE_TRUE(helpers::unsortedSegmentIndicesValidate(
                   block.launchContext(), idxSegments, numOfClasses, wrong),
               0,
               "unsorted_segment_mean: segment indices should be in range [0, "
               "%ld), but %ld != %ld",
               numOfClasses, wrong, numOfClasses);

  helpers::unsortedSegmentMeanFunctor(block.launchContext(), input, idxSegments,
                                      numOfClasses, segmentedOutput);

  return ND4J_STATUS_OK;
}
DECLARE_TYPES(unsorted_segment_mean) {
  getOpDescriptor()
      ->setAllowedOutputTypes({ALL_FLOATS})
      ->setAllowedInputTypes(0, {ALL_FLOATS, ALL_INTS})
      ->setAllowedInputTypes(1, {ALL_INTS})
      ->setSameMode(false);
}

DECLARE_SHAPE_FN(unsorted_segment_mean) {
  auto in = inputShape->at(0);
  int outRank = shape::rank(in);
  Nd4jLong* outputShape = nullptr;
  Nd4jLong numOfClasses =
      block.width() == 3 ? INPUT_VARIABLE(2)->e<Nd4jLong>(0) : INT_ARG(0);

  ALLOCATE(outputShape, block.workspace(), shape::shapeInfoLength(outRank),
           Nd4jLong);

  outputShape[0] = outRank;
  outputShape[1] = numOfClasses;
  for (int i = 1; i < outRank; ++i) outputShape[i + 1] = shape::sizeAt(in, i);

  ShapeUtils::updateStridesAndType(outputShape, in, shape::order(in));

  return SHAPELIST(CONSTANT(outputShape));
}

CUSTOM_OP_IMPL(unsorted_segment_mean_bp, 3, 2, false, 0, 1) {
  return helpers::unsortedSegmentMeanFunctorBP(
      block.launchContext(), INPUT_VARIABLE(0), INPUT_VARIABLE(1),
      INPUT_VARIABLE(2), INT_ARG(0), OUTPUT_NULLIFIED(0));
}

DECLARE_TYPES(unsorted_segment_mean_bp) {
  getOpDescriptor()
      ->setAllowedOutputTypes(0, {ALL_FLOATS})
      ->setAllowedOutputTypes(1, {ALL_INTS})
      ->setAllowedInputTypes(0, {ALL_FLOATS})
      ->setAllowedInputTypes(1, {ALL_INTS})
      ->setAllowedInputTypes(2, {ALL_FLOATS})
      ->setSameMode(false);
}

DECLARE_SHAPE_FN(unsorted_segment_mean_bp) {
  auto in = inputShape->at(0);
  auto inIdx = inputShape->at(1);

  Nd4jLong* outShape;
  Nd4jLong* outIndex;
  COPY_SHAPE(in, outShape);
  COPY_SHAPE(inIdx, outIndex);
  return SHAPELIST(CONSTANT(outShape), CONSTANT(outIndex));
  //            return SHAPELIST(in, inIdx);
}
}  // namespace ops
}  // namespace sd
