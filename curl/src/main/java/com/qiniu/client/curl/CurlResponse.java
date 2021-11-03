package com.qiniu.client.curl;

import java.util.Map;

public class CurlResponse {

    public final String url;
    public final int statusCode;
    public final Map<String, String> allHeaderFields;
    public final String mimeType;
    public final long expectedContentLength;
    public final String httpVersion;

    public CurlResponse(String url,
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

    public String getUrl() {
        return url;
    }

    public int getStatusCode() {
        return statusCode;
    }

    public Map<String, String> getAllHeaderFields() {
        return allHeaderFields;
    }

    public String getMimeType() {
        return null;
    }

    public long getExpectedContentLength() {
        return expectedContentLength;
    }

    public String getHttpVersion() {
        return httpVersion;
    }
}
