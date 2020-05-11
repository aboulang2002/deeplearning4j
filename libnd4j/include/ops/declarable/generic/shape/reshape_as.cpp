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
// Created by raver119 on 29/10/17.
//

#include <system/op_boilerplate.h>
#if NOT_EXCLUDED(OP_reshapeas)

#include <ops/declarable/CustomOperations.h>

namespace sd {
namespace ops {

//////////////////////////////////////////////////////////////////////////
CUSTOM_OP_IMPL(reshapeas, 2, 1, false, 0, 0) {
  auto x = INPUT_VARIABLE(0);
  auto y = INPUT_VARIABLE(1);

  auto z = OUTPUT_VARIABLE(0);

  // FIXME: add validation here?
  auto tmp = x->reshape(y->ordering(), y->getShapeAsVector());
  z->assign(tmp);

  return Status::OK();
}
DECLARE_SYN(reshape_as, reshapeas);

DECLARE_SHAPE_FN(reshapeas) {
  return SHAPELIST(CONSTANT(ShapeBuilders::copyShapeInfo(
      INPUT_VARIABLE(1)->shapeInfo(), false, block.workspace())));
}

DECLARE_TYPES(reshapeas) {
  getOpDescriptor()->setAllowedInputTypes(sd::DataType::ANY)->setSameMode(true);
}
}  // namespace ops
}  // namespace sd

#endif