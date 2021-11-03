package com.qiniu.client.curl;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class CurlThreadPool {

    private static final ExecutorService executorService = Executors.newScheduledThreadPool(1);

    static void run(Runnable r){
        executorService.submit(r);
    }
}
