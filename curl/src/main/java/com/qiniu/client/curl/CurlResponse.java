package com.qiniu.client.curl;

import java.util.Map;

class CurlResponse {

    public final String url;
    public final int statusCode;
    public final Map<String, String> allHeaderFields;
    public final String mimeType;
    public final long expectedContentLength;
    public final String httpVersion;

    CurlResponse(String url,
                        int statusCode,
                        Map<String, String> allHeaderFields,
                        String mimeType,
                        long expectedContentLength,
                        String httpVersion) {
        this.url = url;
        this.statusCode = statusCode;
        this.allHeaderFields = allHeaderFields;
        this.mimeType = mimeType;
        this.expectedContentLength = expectedContentLength;
        this.httpVersion = httpVersion;
    }

    String getUrl() {
        return url;
    }

    int getStatusCode() {
        return statusCode;
    }

    Map<String, String> getAllHeaderFields() {
        return allHeaderFields;
    }

    String getMimeType() {
        return null;
    }

    long getExpectedContentLength() {
        return expectedContentLength;
    }

    String getHttpVersion() {
        return httpVersion;
    }
}
