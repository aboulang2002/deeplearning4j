package org.nd4j.linalg.aurora;

import org.bytedeco.javacpp.LongPointer;
import org.bytedeco.javacpp.Pointer;
import org.junit.Ignore;
import org.junit.Test;
import org.nd4j.linalg.api.buffer.DataBuffer;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.api.ops.impl.transforms.pairwise.arithmetic.AddOp;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.profiler.ProfilerConfig;
import org.nd4j.nativeblas.NativeOpsHolder;
import org.nd4j.nativeblas.Nd4jAurora;
import org.nd4j.nativeblas.Nd4jAuroraOps;

import static org.junit.Assert.assertEquals;
import static org.nd4j.nativeblas.Nd4jAurora.*;

public class AuroraTests {

    @Test
    public void testBasicAdd() {
        Nd4j.getExecutioner().setProfilingConfig(ProfilerConfig.builder()
                .checkWorkspaces(true)
                .checkLocality(true)
                .stackTrace(true)
                .checkElapsedTime(true)
                .checkForINF(true)
                .nativeStatistics(true)
                .notOptimalTAD(true)
                .checkForNAN(true)
                .notOptimalArguments(true)
                .build());
        INDArray arr = Nd4j.scalar(1);
        INDArray arr2 = Nd4j.scalar(2);
        INDArray result = Nd4j.scalar(0.0);
        System.out.println("Arr " + arr + " arr 2 " + arr2);
        AddOp addOp = new AddOp(arr,arr2,result);
        Nd4j.exec(addOp);
        System.out.println(result);
    }

    @Test
    @Ignore
    public void testMemory() {
        Nd4jAuroraOps ops = (Nd4jAuroraOps) NativeOpsHolder.getInstance().getDeviceNativeOps();
        long size = 16;
        Pointer allocated = ops.mallocDevice(size,0,0);
        LongPointer hostPointer = new LongPointer(1,2);
        ops.memcpySync(allocated,hostPointer,size,0,null);

        LongPointer readFromMemory = new LongPointer(0,0);
        ops.memcpySync(readFromMemory,allocated,size,0,null);
        assertEquals(1,readFromMemory.get(0));
        assertEquals(2,readFromMemory.get(1));
    }


    @Test
    public void testDataBuffers() {
        DataBuffer buffer = Nd4j.createBuffer(new int[] {2});
        assertEquals(2,buffer.getInt(0));
    }

    @Test
    @Ignore
    public void testAuroraBasic() {
        veo_proc_handle proc = Nd4jAuroraOps.getInstance().veo_proc_create(0);
        Nd4jAurora.veo_args argp = Nd4jAuroraOps.getInstance().veo_args_alloc();
        Nd4jAuroraOps.getInstance().veo_args_set_i64(argp, 0, 42);
        veo_thr_ctxt ctx = Nd4jAuroraOps.getInstance().veo_context_open(proc);
        //load library and method name to run
        String libPath = System.getenv("LIB_PATH");
        String methodName = System.getenv("METHOD_NAME");
        System.out.println("Lib path " + libPath + " method name " + methodName);

        long handle = Nd4jAuroraOps.getInstance().veo_load_library(proc, libPath);
        Nd4jAuroraOps.getInstance().veo_call_async_by_name(ctx, handle, methodName, argp);

        long[] output = {-2};
        int result2 = Nd4jAuroraOps.getInstance().veo_call_wait_result(ctx,handle,output);
        System.out.println("Output result " + output[0] + " with exit code " + result2);
        Nd4jAuroraOps.getInstance().veo_args_free(argp);
        Nd4jAuroraOps.getInstance().veo_context_close(ctx);

        //veo_proc_destroy(proc);
    }


}
