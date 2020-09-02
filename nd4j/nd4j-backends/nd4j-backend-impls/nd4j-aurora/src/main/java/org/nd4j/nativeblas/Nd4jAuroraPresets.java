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

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Scanner;

/**
 *
 * @author saudet
 */
@Properties(
        value = {
                @Platform(
                        value = "linux-x86_64",
                        link = "veo@.0",
                        includepath = "/opt/nec/ve/veos/include/",
                        linkpath = "/opt/nec/ve/veos/lib64/",
                        library = "jnind4jaurora",
                        resource = {"nd4jaurora", "libnd4jaurora.so"},
                        include = {
                                "ve_offload.h"}

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
        infoMap.put(new Info("thread_local", "ND4J_EXPORT", "INLINEDEF", "CUBLASWINAPI", "FORCEINLINE",
                "_CUDA_H", "_CUDA_D", "_CUDA_G", "_CUDA_HD", "LIBND4J_ALL_OPS", "NOT_EXCLUDED").cppTypes().annotations())
                .put(new Info("NativeOps.h").objectify())
                .put(new Info("OpaqueTadPack").pointerTypes("OpaqueTadPack"))
                .put(new Info("OpaqueResultWrapper").pointerTypes("OpaqueResultWrapper"))
                .put(new Info("OpaqueShapeList").pointerTypes("OpaqueShapeList"))
                .put(new Info("OpaqueVariablesSet").pointerTypes("OpaqueVariablesSet"))
                .put(new Info("OpaqueVariable").pointerTypes("OpaqueVariable"))
                .put(new Info("OpaqueConstantDataBuffer").pointerTypes("OpaqueConstantDataBuffer"))
                .put(new Info("OpaqueDataBuffer").pointerTypes("OpaqueDataBuffer"))
                .put(new Info("OpaqueContext").pointerTypes("OpaqueContext"))
                .put(new Info("OpaqueRandomGenerator").pointerTypes("OpaqueRandomGenerator"))
                .put(new Info("OpaqueLaunchContext").pointerTypes("OpaqueLaunchContext"))
                .put(new Info("const char").valueTypes("byte").pointerTypes("@Cast(\"char*\") String",
                        "@Cast(\"char*\") BytePointer"))
                .put(new Info("char").valueTypes("char").pointerTypes("@Cast(\"char*\") BytePointer",
                        "@Cast(\"char*\") String"))
                .put(new Info("Nd4jPointer").cast().valueTypes("Pointer").pointerTypes("PointerPointer"))
                .put(new Info("Nd4jLong").cast().valueTypes("long").pointerTypes("LongPointer", "LongBuffer",
                        "long[]"))
                .put(new Info("Nd4jStatus").cast().valueTypes("int").pointerTypes("IntPointer", "IntBuffer",
                        "int[]"))
                .put(new Info("float16").cast().valueTypes("short").pointerTypes("ShortPointer", "ShortBuffer",
                        "short[]"))
                .put(new Info("bfloat16").cast().valueTypes("short").pointerTypes("ShortPointer", "ShortBuffer",
                        "short[]"));

        infoMap.put(new Info("__CUDACC__", "MAX_UINT", "HAVE_MKLDNN", "__CUDABLAS__").define(false))
                .put(new Info("__JAVACPP_HACK__", "LIBND4J_ALL_OPS").define(true))
                .put(new Info("std::initializer_list", "cnpy::NpyArray", "sd::NDArray::applyLambda", "sd::NDArray::applyPairwiseLambda",
                        "sd::graph::FlatResult", "sd::graph::FlatVariable", "sd::NDArray::subarray").skip())
                .put(new Info("std::string").annotations("@StdString").valueTypes("BytePointer", "String")
                        .pointerTypes("@Cast({\"char*\", \"std::string*\"}) BytePointer"))
                .put(new Info("std::pair<int,int>").pointerTypes("IntIntPair").define())
                .put(new Info("std::vector<std::vector<int> >").pointerTypes("IntVectorVector").define())
                .put(new Info("std::vector<std::vector<Nd4jLong> >").pointerTypes("LongVectorVector").define())
                .put(new Info("std::vector<const sd::NDArray*>").pointerTypes("ConstNDArrayVector").define())
                .put(new Info("std::vector<sd::NDArray*>").pointerTypes("NDArrayVector").define())
                .put(new Info("sd::graph::ResultWrapper").base("org.nd4j.nativeblas.ResultWrapperAbstraction").define())
                .put(new Info("bool").cast().valueTypes("boolean").pointerTypes("BooleanPointer", "boolean[]"))
                .put(new Info("sd::IndicesList").purify());

        /*
        String classTemplates[] = {
                "sd::NDArray",
                "sd::NDArrayList",
                "sd::ResultSet",
                "sd::OpArgsHolder",
                "sd::graph::GraphState",
                "sd::graph::Variable",
                "sd::graph::VariablesSet",
                "sd::graph::Stash",
                "sd::graph::VariableSpace",
                "sd::graph::Context",
                "sd::graph::ContextPrototype",
                "sd::ops::DeclarableOp",
                "sd::ops::DeclarableListOp",
                "sd::ops::DeclarableReductionOp",
                "sd::ops::DeclarableCustomOp",
                "sd::ops::BooleanOp",
                "sd::ops::BroadcastableOp",
                "sd::ops::LogicOp"};
        for (String t : classTemplates) {
            String s = t.substring(t.lastIndexOf(':') + 1);
            infoMap.put(new Info(t + "<float>").pointerTypes("Float" + s))
                   .put(new Info(t + "<float16>").pointerTypes("Half" + s))
                   .put(new Info(t + "<double>").pointerTypes("Double" + s));
        }
        */

        // pick up custom operations automatically from CustomOperations.h and headers in libnd4j
        String separator = properties.getProperty("platform.path.separator");
        String[] includePaths = properties.getProperty("platform.includepath").split(separator);
        File file = null;
        for (String path : includePaths) {
            file = new File(path, "ops/declarable/CustomOperations.h");
            if (file.exists()) {
                break;
            }
        }
        ArrayList<File> files = new ArrayList<File>();
        ArrayList<String> opTemplates = new ArrayList<String>();
        files.add(file);
        files.addAll(Arrays.asList(new File(file.getParent(), "headers").listFiles()));
        Collections.sort(files);
        for (File f : files) {
            try (Scanner scanner = new Scanner(f, "UTF-8")) {
                while (scanner.hasNextLine()) {
                    String line = scanner.nextLine().trim();
                    if (line.startsWith("DECLARE_")) {
                        try {
                            int start = line.indexOf('(') + 1;
                            int end = line.indexOf(',');
                            if (end < start) {
                                end = line.indexOf(')');
                            }
                            String name = line.substring(start, end).trim();
                            opTemplates.add(name);
                        } catch(Exception e) {
                            throw new RuntimeException("Could not parse line from CustomOperations.h and headers: \"" + line + "\"", e);
                        }
                    }
                }
            } catch (IOException e) {
                throw new RuntimeException("Could not parse CustomOperations.h and headers", e);
            }
        }
        logger.info("Ops found in CustomOperations.h and headers: " + opTemplates);
        /*
        String floatOps = "", halfOps = "", doubleOps = "";
        for (String t : opTemplates) {
            String s = "sd::ops::" + t;
            infoMap.put(new Info(s + "<float>").pointerTypes("float_" + t))
                   .put(new Info(s + "<float16>").pointerTypes("half_" + t))
                   .put(new Info(s + "<double>").pointerTypes("double_" + t));
            floatOps  += "\n        float_" + t + ".class,";
            halfOps   += "\n        half_" + t + ".class,";
            doubleOps += "\n        double_" + t + ".class,";

        }
        infoMap.put(new Info().javaText("\n"
                                      + "    Class[] floatOps = {" + floatOps + "};" + "\n"
                                      + "    Class[] halfOps = {" + halfOps + "};" + "\n"
                                      + "    Class[] doubleOps = {" + doubleOps + "};"));
        */
        infoMap.put(new Info("sd::ops::OpRegistrator::updateMSVC").skip());
    }
}
