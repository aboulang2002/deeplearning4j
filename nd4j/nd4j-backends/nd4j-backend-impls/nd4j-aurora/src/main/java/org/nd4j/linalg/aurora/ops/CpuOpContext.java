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

package org.nd4j.linalg.aurora.ops;

import lombok.NonNull;
import lombok.extern.slf4j.Slf4j;
import org.bytedeco.javacpp.BooleanPointer;
import org.bytedeco.javacpp.DoublePointer;
import org.bytedeco.javacpp.LongPointer;
import org.bytedeco.javacpp.Pointer;
import org.nd4j.linalg.api.memory.Deallocatable;
import org.nd4j.linalg.api.memory.Deallocator;
import org.nd4j.linalg.api.memory.pointers.PagedPointer;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.BaseOpContext;
import org.nd4j.linalg.api.ops.ExecutionMode;
import org.nd4j.linalg.api.ops.OpContext;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.primitives.Pair;
import org.nd4j.nativeblas.NativeOps;
import org.nd4j.nativeblas.NativeOpsHolder;
import org.nd4j.nativeblas.OpaqueContext;
import org.nd4j.nativeblas.OpaqueRandomGenerator;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * CPU backend Context wrapper
 *
 * @author raver119@gmail.com
 */
@Slf4j
public class CpuOpContext extends BaseOpContext implements OpContext, Deallocatable {
    // we might want to have configurable
    private NativeOps nativeOps = NativeOpsHolder.getInstance().getDeviceNativeOps();
    private final transient long id = Nd4j.getDeallocatorService().nextValue();
    private OpaqueContext context = nativeOps.createGraphContext((int) id);

    protected Map<Integer,Pointer> inputDeviceBuffers = new HashMap<>();
    protected Map<Integer,Pointer> inputDeviceShapes = new HashMap<>();
    protected Map<Integer,Pointer> outputDeviceBuffers = new HashMap<>();
    protected Map<Integer,Pointer> outputDeviceShapes = new HashMap<>();

    @Override
    public void close() {
        List<INDArray> inputArrays = getInputArrays();
        for (int i = 0; i < inputArrays.size(); i++) {
            INDArray array = inputArrays.get(i);
            if (!array.isEmpty()) {
                Pointer p = array.data().addressPointer();
                long size = (p.limit() - p.position()) * p.sizeof();
                nativeOps.memcpySync(p, inputDeviceBuffers.get(i), size, 0, null);
            }
            nativeOps.freeDevice(inputDeviceBuffers.get(i), -1);
            nativeOps.freeDevice(inputDeviceShapes.get(i), -1);
        }
        List<INDArray> outputArrays = getOutputArrays();
        for (int i = 0; i < outputArrays.size(); i++) {
            INDArray array = outputArrays.get(i);
            if (!array.isEmpty()) {
                Pointer p = array.data().addressPointer();
                long size = (p.limit() - p.position()) * p.sizeof();
                nativeOps.memcpySync(p, outputDeviceBuffers.get(i), size, 0, null);
            }
            nativeOps.freeDevice(outputDeviceBuffers.get(i), -1);
            nativeOps.freeDevice(outputDeviceShapes.get(i), -1);
        }
        nativeOps.deleteGraphContext(context);
    }

    @Override
    public void setIArguments(long... arguments) {
        super.setIArguments(arguments);
        nativeOps.setGraphContextIArguments(context, new LongPointer(arguments), arguments.length);
    }

    @Override
    public void setBArguments(boolean... arguments) {
        super.setBArguments(arguments);
        nativeOps.setGraphContextBArguments(context, new BooleanPointer(arguments), arguments.length);
    }

    @Override
    public void setTArguments(double... arguments) {
        super.setTArguments(arguments);
        nativeOps.setGraphContextTArguments(context, new DoublePointer(arguments), arguments.length);
    }

    @Override
    public void setRngStates(long rootState, long nodeState) {
        nativeOps.setRandomGeneratorStates(nativeOps.getGraphContextRandomGenerator(context), rootState, nodeState);
    }

    @Override
    public Pair<Long, Long> getRngStates() {
        OpaqueRandomGenerator g = nativeOps.getGraphContextRandomGenerator(context);
        return Pair.makePair(nativeOps.getRandomGeneratorRootState(g), nativeOps.getRandomGeneratorNodeState(g));
    }

    @Override
    public void setInputArray(int index, @NonNull INDArray array) {
        if (array.isEmpty()) {
            log.info("Empty array invocation for input array on index {} ",index);
            inputDeviceBuffers.put(index, null);
        } else {
            Pointer p = array.data().addressPointer();
            long size = array.data().getElementSize() * array.data().length();
            log.debug("Mallocing memory of size {} for index {}",size,index);
            Pointer p2 = nativeOps.mallocDevice(size, -1, 0);
            log.debug("After Mallocing memory of size {} for index {}",size,index);
            log.debug("Memcpy memory of size {} for index {}",size,index);
            nativeOps.memcpySync(p2, p, size, 0, null);
            log.debug("Finish Memcpy memory of size {} for index {}",size,index);
            inputDeviceBuffers.put(index, p2);
        }

        Pointer p = array.shapeInfoDataBuffer().addressPointer();
        long size = (p.limit() - p.position()) * p.sizeof();
        log.debug("Mallocing shape buffer memory of size {} for index {}",size,index);
        Pointer p2 = nativeOps.mallocDevice(size, -1, 0);
        log.debug("After Mallocing shape buffer memory of size {} for index {}",size,index);
        log.debug("Memcpy shape buffer memory of size {} for index {}",size,index);
        nativeOps.memcpySync(p2, p, size, 0, null);
        log.debug("After Memcpy shape buffer memory of size {} for index {}",size,index);
        inputDeviceShapes.put(index, p2);
        log.debug("Invoking set context graph input array on size {} and index {}",size,index);
        nativeOps.setGraphContextInputArray(context, index, inputDeviceBuffers.get(index), inputDeviceShapes.get(index), null, null);
        log.debug("Finished Invoking set context graph input array on size {} and index {}",size,index);

        super.setInputArray(index, array);
    }

    @Override
    public void setOutputArray(int index, @NonNull INDArray array) {
        if (array.isEmpty()) {
            outputDeviceBuffers.put(index, null);
        } else {
            Pointer p = array.data().addressPointer();
            long size = (p.limit() - p.position()) * p.sizeof();
            Pointer p2 = nativeOps.mallocDevice(size, -1, 0);
            nativeOps.memcpySync(p2, p, size, 0, null);
            outputDeviceBuffers.put(index, p2);
        }
        Pointer p = array.shapeInfoDataBuffer().addressPointer();
        long size = (p.limit() - p.position()) * p.sizeof();
        Pointer p2 = nativeOps.mallocDevice(size, -1, 0);
        nativeOps.memcpySync(p2, p, size, 0, null);
        outputDeviceShapes.put(index, p2);

        nativeOps.setGraphContextOutputArray(context, index, outputDeviceBuffers.get(index), outputDeviceShapes.get(index), null, null);

        super.setOutputArray(index, array);
    }

    @Override
    public Pointer contextPointer() {
        return context;
    }

    @Override
    public void markInplace(boolean reallyInplace) {
        nativeOps.markGraphContextInplace(context, reallyInplace);
    }


    @Override
    public void allowHelpers(boolean reallyAllow) {
        nativeOps.ctxAllowHelpers(context, reallyAllow);
    }

    @Override
    public void shapeFunctionOverride(boolean reallyOverride) {
        nativeOps.ctxShapeFunctionOverride(context, reallyOverride);
    }

    @Override
    public void setExecutionMode(@NonNull ExecutionMode mode) {
        super.setExecutionMode(mode);
        nativeOps.ctxSetExecutionMode(context, mode.ordinal());
    }

    @Override
    public void purge() {
        super.purge();
        nativeOps.ctxPurge(context);
    }

    @Override
    public String getUniqueId() {
        return new String("CTX_" + id);
    }

    @Override
    public Deallocator deallocator() {
        return new AuroraOpContextDeallocator(this);
    }

    @Override
    public int targetDevice() {
        return 0;
    }
}
