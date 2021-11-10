package com.qiniu.client.curl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

class CurlRequest {
    static final int HttpMethodHead = 0;
    static final int HttpMethodGet = 1;
    static final int HttpMethodPost = 2;
    static final int HttpMethodPut = 3;
    static final int HttpMethodDelete = 4;

    static final int HttpVersionCurlMatch = 0;
    static final int HttpVersion3 = 30;

    private final String urlString;
    private final int httpVersion;
    private final int httpMethod;
    private final Map<String, String> allHeaders;
    private final int timeout;
    private final byte[] httpBody;

    CurlRequest(String urlString,
                int httpVersion,
                int httpMethod,
                Map<String, String> allHeaders,
                byte[] httpBody,
                int timeout) {
        this.urlString = urlString;
        this.httpMethod = httpMethod;
        if (httpVersion == HttpVersionCurlMatch || httpVersion == HttpVersion3) {
            this.httpVersion = httpVersion;
        } else {
            this.httpVersion = HttpVersionCurlMatch;
        }
        this.allHeaders = (allHeaders != null) ? allHeaders : new HashMap<>();
        this.httpBody = (httpBody != null) ? httpBody : new byte[0];
        this.timeout = timeout;
    }

    String getUrlString() {
        return urlString;
    }

    int getHttpMethod() {
        return httpMethod;
    }

    int getHttpVersion() {
        return httpVersion;
    }

    Map<String, String> getAllHeaders() {
        return allHeaders;
    }

    String[] getAllHeaderList() {
        String[] headerArray = new String[]{};
        ArrayList<String> headerList = new ArrayList<>();
        for (String headerKey : allHeaders.keySet()) {
            String headerValue = allHeaders.get(headerKey);
            String header = headerKey + ": " + headerValue;
            headerList.add(header);
        }
        headerList.add("Expect: null");
        headerArray = headerList.toArray(new String[]{});

        return headerArray;
    }

    int getTimeout() {
        return timeout;
    }

    byte[] getHttpBody() {
        return httpBody;
    }
}
