package com.qiniu.client.curl;

import java.util.Date;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class BaseTest {

    private long maxWaitTimestamp = 0;

    /**
     * waitCondition: 等待条件
     * maxWaitTime: 等待最大时长 单位-秒
     */
    void wait(WaitConditional waitConditional, float maxWaitTime) {

        WaitConditional waitConditionalP = waitConditional;
        if (waitConditionalP == null) {
            waitConditionalP = new WaitCondition();
        }

        this.maxWaitTimestamp = new Date().getTime() + (long) (maxWaitTime * 1000);
        while (waitConditionalP.shouldWait()) {
            long currentTimestamp = new Date().getTime();
            if (currentTimestamp > maxWaitTimestamp) {
                break;
            }
            CountDownLatch pieceWaitSignal = new CountDownLatch(1);
            try {
                pieceWaitSignal.await(10, TimeUnit.MILLISECONDS);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }


    public interface WaitConditional {
        boolean shouldWait();
    }

    public static class WaitCondition implements WaitConditional {
        public boolean shouldWait = true;

        public boolean shouldWait() {
            return shouldWait;
        }
    }
}
