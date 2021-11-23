package com.qiniu.client.curl;

class Curl {

    private boolean isCancelled = false;

    static {
        System.loadLibrary("qn-curl");
    }

    boolean isCancelled() {
        return isCancelled;
    }

    private static boolean hasGlobalInit = false;
    private static long globalInitCode = 0;

    private static synchronized long init(Curl curl) {
        if (hasGlobalInit) {
            return globalInitCode;
        }
        globalInitCode = curl.globalInit();
        hasGlobalInit = true;
        return globalInitCode;
    }

    native long globalInit();

    static native String getCurlVersion();

    void request(final CurlRequest request,
                 final CurlConfiguration curlConfiguration,
                 final Handler curlHandler) {
        long ret = init(this);
        if (ret != 0) {
            if (curlHandler != null) {
                curlHandler.completeWithError((int) ret, "curl global init error");
            }
            return;
        }

        CurlHandler curlHandlerReal = new CurlHandler(curlHandler);
        requestNative(request, curlConfiguration, curlHandlerReal);
    }

    native void requestNative(final CurlRequest request,
                              final CurlConfiguration curlConfiguration,
                              final CurlHandler curlHandler);

    void cancel() {
        isCancelled = true;
    }


    interface Handler {
        void receiveResponse(CurlResponse response);

        byte[] sendData(long dataLength);

        void receiveData(byte[] data);

        void completeWithError(int code, String errorMsg);

        void sendProgress(long bytesSent, long totalBytesSent, long totalBytesExpectedToSend);

        void receiveProgress(long bytesReceive, long totalBytesReceive, long totalBytesExpectedToReceive);

        void didFinishCollectingMetrics(CurlTransactionMetrics metrics);
    }
}
