package org.nd4j.linalg.aurora;

import org.junit.Test;
import org.nd4j.nativeblas.Nd4jAurora;

import static org.nd4j.nativeblas.Nd4jAurora.*;

public class AuroraTests {

    @Test
    public void testAuroraBasic() {
        veo_proc_handle proc = veo_proc_create(0);
        Nd4jAurora.veo_args argp = veo_args_alloc();
        veo_thr_ctxt ctx = veo_context_open(proc);
        String libPath = System.getenv("LIB_PATH");
        String methodName = System.getenv("METHOD_NAME");
        System.out.println("Lib path " + libPath + " method name " + methodName);
        long handle = veo_load_library(proc, libPath);
        veo_call_async_by_name(ctx, handle, methodName, argp);
        long[] output = new long[1];
        int result2 = veo_call_wait_result(ctx,handle,output);
        System.out.println("Output result " + output[0] + " with exit code " + result2);
        veo_args_free(argp);
        veo_context_close(ctx);

        //veo_proc_destroy(proc);
    }


}
