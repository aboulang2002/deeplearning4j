package org.nd4j.linalg.aurora;

import org.bytedeco.javacpp.LongPointer;
import org.junit.Ignore;
import org.junit.Test;
import org.nd4j.linalg.api.buffer.DataBuffer;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.nativeblas.Nd4jAurora;

import static org.junit.Assert.assertEquals;
import static org.nd4j.nativeblas.Nd4jAurora.*;

public class AuroraTests {

    @Test
    public void testBasicAdd() {
        Nd4j.getEnvironment().setVerbose(true);
        Nd4j.getEnvironment().setDebug(true);
        INDArray arr = Nd4j.scalar(1);
        INDArray arr2 = Nd4j.scalar(2);
        System.out.println(arr.add(arr2));
    }

    @Test
    public void testDataBuffers() {
        DataBuffer buffer = Nd4j.createBuffer(2);
        assertEquals(2,buffer.getInt(0));
    }

    @Test
    @Ignore
    public void testAuroraBasic() {
        veo_proc_handle proc = veo_proc_create(0);
        Nd4jAurora.veo_args argp = veo_args_alloc();
        veo_args_set_i64(argp, 0, 42);
        veo_thr_ctxt ctx = veo_context_open(proc);
        //load library and method name to run
        String libPath = System.getenv("LIB_PATH");
        String methodName = System.getenv("METHOD_NAME");
        System.out.println("Lib path " + libPath + " method name " + methodName);

        long handle = veo_load_library(proc, libPath);
        veo_call_async_by_name(ctx, handle, methodName, argp);

        long[] output = {-2};
        int result2 = veo_call_wait_result(ctx,handle,output);
        System.out.println("Output result " + output[0] + " with exit code " + result2);
        veo_args_free(argp);
        veo_context_close(ctx);

        //veo_proc_destroy(proc);
    }


}
