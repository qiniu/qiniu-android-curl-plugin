package com.qiniu.client.curl;

import android.util.Log;

import java.util.HashMap;


class CurlHandler {

    CurlHandler(Curl.Handler curlHandler) {
        this.curlHandler = curlHandler;
    }

    private final Curl.Handler curlHandler;

    void receiveResponse(String url, int statusCode, String httpVersion, Object[] headers){

        if (curlHandler != null){
            HashMap<String, String> responseHeader = new HashMap<>();
            if (headers != null){
                for (int i = 0; i < headers.length; i++) {
                    if (headers[i] instanceof String){
                        String headerField = (String) headers[i];
                        headerField = headerField.replace(" ", "");
                        headerField = headerField.replace("\r", "");
                        headerField = headerField.replace("\n", "");

                        String[] fieldArray = headerField.split(":", 2);
                        if (fieldArray.length != 2){
                            continue;
                        }
                        String key = fieldArray[0];
                        String value = fieldArray[1];
                        responseHeader.put(key.toLowerCase(), value);
                    }
                }
            }
            String mineType = responseHeader.get("content-type");
            long contentLength = 0;
            String contentLengthObject = responseHeader.get("content-length");
            if (contentLengthObject != null){
                contentLength = Long.parseLong(contentLengthObject);
            }
            CurlResponse response = new CurlResponse(url, statusCode, responseHeader, mineType, contentLength, httpVersion);
            curlHandler.receiveResponse(response);
        }
    }
    byte[] sendData(long dataLength){
        if (curlHandler != null){
            return curlHandler.sendData(dataLength);
        } else {
            return null;
        }
    }
    void receiveData(byte[] data){
        if (curlHandler != null){
            curlHandler.receiveData(data);
        }
    }
    void completeWithError(int errorCode, String errorInfo){
        if (curlHandler != null){
            curlHandler.completeWithError(errorCode, errorInfo);
        }
    }
    void sendProgress(long bytesSent, long totalBytesSent, long totalBytesExpectedToSend){
        if (curlHandler != null){
            curlHandler.sendProgress(bytesSent, totalBytesSent, totalBytesExpectedToSend);
        }
    }
    void receiveProgress(long bytesReceive, long totalBytesReceive, long totalBytesExpectedToReceive){
        if (curlHandler != null){
            curlHandler.receiveProgress(bytesReceive, totalBytesReceive, totalBytesExpectedToReceive);
        }
    }
    void didFinishCollectingMetrics(CurlTransactionMetrics metrics){
        if (curlHandler != null){
            curlHandler.didFinishCollectingMetrics(metrics);
        }
    }
}
