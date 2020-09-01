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

package org.nd4j.nativeblas;

import org.bytedeco.javacpp.annotation.NoException;
import org.bytedeco.javacpp.annotation.Platform;
import org.bytedeco.javacpp.annotation.Properties;
import org.bytedeco.javacpp.tools.*;

/**
 *
 * @author saudet
 */
@Properties(
        value = {
                @Platform(
                        value = "linux-x86_64",
                        cinclude = "ve_offload.h",
                        link = "veo@.0",
                        includepath = "/opt/nec/ve/veos/include/",
                        linkpath = "/opt/nec/ve/veos/lib64/",
                        library = "jnind4jaurora",
                        resource = {"nd4jaurora", "libnd4jaurora.so"},
                        define = "LIBND4J_ALL_OPS", include = {
                        "memory/MemoryType.h",
                        "array/DataType.h",
                        "array/DataBuffer.h",
                        "array/ConstantDataBuffer.h",
                        "array/ConstantDescriptor.h",
                        "array/TadPack.h",
                        "execution/ErrorReference.h",
                        "execution/Engine.h",
                        "execution/ExecutionMode.h",
                        "system/Environment.h",
                        "types/utf8string.h",
                        "legacy/NativeOps.h",
                        "memory/ExternalWorkspace.h",
                        "memory/Workspace.h",
                        "indexing/NDIndex.h",
                        "indexing/IndicesList.h",
                        "graph/VariableType.h",
                        "graph/ArgumentsList.h",
                        "types/pair.h",
                        "array/NDArray.h",
                        "array/NDArrayList.h",
                        "array/ResultSet.h",
                        "types/pair.h",
                        "graph/RandomGenerator.h",
                        "graph/Variable.h",
                        "graph/VariablesSet.h",
                        "graph/FlowPath.h",
                        "graph/Intervals.h",
                        "graph/Stash.h",
                        "graph/GraphState.h",
                        "graph/VariableSpace.h",
                        "helpers/helper_generator.h",
                        "graph/profiling/GraphProfile.h",
                        "graph/profiling/NodeProfile.h",
                        "graph/Context.h",
                        "graph/ContextPrototype.h",
                        "graph/ResultWrapper.h",
                        "helpers/shape.h",
                        "helpers/OpArgsHolder.h",
                        "array/ShapeList.h",
                        "system/type_boilerplate.h",
                        "system/op_boilerplate.h",
                        //"enum_boilerplate.h",
                        //"op_enums.h",
                        "ops/InputType.h",
                        "ops/declarable/OpDescriptor.h",
                        "ops/declarable/PlatformHelper.h",
                        "ops/declarable/BroadcastableOp.h",
                        "ops/declarable/DeclarableOp.h",
                        "ops/declarable/DeclarableListOp.h",
                        "ops/declarable/DeclarableReductionOp.h",
                        "ops/declarable/DeclarableCustomOp.h",
                        "ops/declarable/BooleanOp.h",
                        "ops/declarable/LogicOp.h",
                        "ops/declarable/OpRegistrator.h",
                        "ops/declarable/CustomOperations.h",
                        "ops/declarable/headers/activations.h",
                        "ops/declarable/headers/boolean.h",
                        "ops/declarable/headers/broadcastable.h",
                        "ops/declarable/headers/convo.h",
                        "ops/declarable/headers/list.h",
                        "ops/declarable/headers/recurrent.h",
                        "ops/declarable/headers/transforms.h",
                        "ops/declarable/headers/parity_ops.h",
                        "ops/declarable/headers/shape.h",
                        "ops/declarable/headers/random.h",
                        "ops/declarable/headers/nn.h",
                        "ops/declarable/headers/blas.h",
                        "ops/declarable/headers/tests.h",
                        "ops/declarable/headers/bitwise.h",
                        "ops/declarable/headers/loss.h",
                        "ops/declarable/headers/datatypes.h",
                        "execution/ContextBuffers.h",
                        "execution/LaunchContext.h",
                        "array/ShapeDescriptor.h",
                        "array/TadDescriptor.h",
                        "helpers/DebugInfo.h",
                        "ops/declarable/headers/third_party.h"},
                        exclude = {"ops/declarable/headers/activations.h",
                                "ops/declarable/headers/boolean.h",
                                "ops/declarable/headers/broadcastable.h",
                                "ops/declarable/headers/convo.h",
                                "ops/declarable/headers/list.h",
                                "ops/declarable/headers/recurrent.h",
                                "ops/declarable/headers/transforms.h",
                                "ops/declarable/headers/parity_ops.h",
                                "ops/declarable/headers/shape.h",
                                "ops/declarable/headers/random.h",
                                "ops/declarable/headers/nn.h",
                                "ops/declarable/headers/blas.h",
                                "ops/declarable/headers/bitwise.h",
                                "ops/declarable/headers/tests.h",
                                "ops/declarable/headers/loss.h",
                                "ops/declarable/headers/datatypes.h",
                                "ops/declarable/headers/third_party.h",
                                "cnpy/cnpy.h"
                        }
                )
        },
        target = "org.nd4j.nativeblas.Nd4jAurora"
)
@NoException
public class Nd4jAuroraPresets implements InfoMapper, BuildEnabled {

    private Logger logger;
    private java.util.Properties properties;
    private String encoding;

    @Override
    public void init(Logger logger, java.util.Properties properties, String encoding) {
        this.logger = logger;
        this.properties = properties;
        this.encoding = encoding;
    }

    @Override
    public void map(InfoMap infoMap) {
    }
}
