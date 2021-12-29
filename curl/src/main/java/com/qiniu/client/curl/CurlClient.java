package com.qiniu.client.curl;

import android.util.Log;

import com.qiniu.android.http.ProxyConfiguration;
import com.qiniu.android.http.ResponseInfo;
import com.qiniu.android.http.metrics.UploadSingleRequestMetrics;
import com.qiniu.android.http.request.IRequestClient;
import com.qiniu.android.http.request.IUploadServer;
import com.qiniu.android.http.request.Request;

import org.json.JSONObject;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

public class CurlClient extends IRequestClient {

    private Curl curl = null;
    private Request request = null;
    private CurlResponse curlResponse = null;
    private UploadSingleRequestMetrics metrics = new UploadSingleRequestMetrics();
    private ByteArrayInputStream requestDataStream = null;
    private ByteArrayOutputStream responseDataStream = null;

    @Override
    public void request(Request request,
                        Options options,
                        RequestClientProgress progress,
                        RequestClientCompleteHandler complete) {

        IUploadServer server = null;
        ProxyConfiguration connectionProxy = null;
        boolean isAsync = true;
        if (options != null) {
            server = options.server;
            connectionProxy = options.connectionProxy;
            isAsync = options.isAsync;
        }

        this.request = request;
        metrics.setRequest(request);

        String host = server != null ? server.getHost() : null;
        String ip = server != null ? server.getIp() : null;
        CurlConfiguration.Builder builder = new CurlConfiguration.Builder();
        if (host != null && ip != null) {
            int port = request.urlString.contains("https://") ? 443 : 80;
            CurlConfiguration.DnsResolver resolver = new CurlConfiguration.DnsResolver(host, ip, port);
            builder.setDnsResolverArray(new CurlConfiguration.DnsResolver[]{resolver});

            metrics.setRemoteAddress(ip);
            metrics.setRemotePort(port);
        }
        if (connectionProxy != null && connectionProxy.hostAddress != null) {
            String proxy = String.format(Locale.ENGLISH, "%s:%d", connectionProxy.hostAddress, connectionProxy.port);
            builder.setProxy(proxy);
            if (connectionProxy.user != null && connectionProxy.password != null) {
                String userPwd = String.format(Locale.ENGLISH, "%s:%s", connectionProxy.user, connectionProxy.password);
                builder.setProxyUserPwd(userPwd);
            }
        }
        final CurlConfiguration curlConfiguration = builder.build();

        int httpMethod = CurlRequest.HttpMethodGet;
        if (request.httpMethod.equals(Request.HttpMethodHEAD)) {
            httpMethod = CurlRequest.HttpMethodHead;
        } else if (request.httpMethod.equals(Request.HttpMethodGet)) {
            httpMethod = CurlRequest.HttpMethodGet;
        } else if (request.httpMethod.equals(Request.HttpMethodPOST)) {
            httpMethod = CurlRequest.HttpMethodPost;
        } else if (request.httpMethod.equals(Request.HttpMethodPUT)) {
            httpMethod = CurlRequest.HttpMethodPut;
        }

        int httpVersion = CurlRequest.HttpVersionCurlMatch;
        if (server != null && server.isHttp3() && curlConfiguration.getDnsResolverArray() != null) {
            httpVersion = CurlRequest.HttpVersion3;
        }

        if (request.httpBody != null && request.httpBody.length > 0) {
            requestDataStream = new ByteArrayInputStream(request.httpBody);
            request.allHeaders.put("Content-Length", request.httpBody.length + "");
        }
        final CurlRequest curlRequest = new CurlRequest(request.urlString, httpVersion, httpMethod,
                request.allHeaders, request.httpBody, request.timeout);
        userAgentAddCurlVersion(curlRequest);
        if (isAsync) {
            CurlThreadPool.run(new Runnable() {
                @Override
                public void run() {
                    request(curlRequest, curlConfiguration, progress, complete);
                }
            });
        } else {
            request(curlRequest, curlConfiguration, progress, complete);
        }
    }

    private void request(CurlRequest curlRequest,
                         CurlConfiguration configuration,
                         final RequestClientProgress progress,
                         final RequestClientCompleteHandler complete) {

        metrics.start();
        responseDataStream = new ByteArrayOutputStream();
        curl = new Curl();
        curl.request(curlRequest, configuration, new Curl.Handler() {

            @Override
            public void receiveResponse(CurlResponse response) {
                curlResponse = response;
                if (response != null) {
                    metrics.setHttpVersion(response.httpVersion);
                }
                Log.d("Curl", "====== Response: url:" + response.getUrl() + " statusCode:" + response.getStatusCode() + " headerInfo:" + response.getAllHeaderFields());
            }

            @Override
            public byte[] sendData(long dataLength) {
                // 设置了 body 不需要在设置 此回调
                Log.d("Curl", "====== sendData:");
                byte[] data = new byte[(int) dataLength];
                try {
                    int length = requestDataStream.read(data);
                    data = Arrays.copyOf(data, length);
                } catch (Exception e) {
                    data = new byte[0];
                }
                return data;
            }

            @Override
            public void receiveData(byte[] data) {
                try {
                    responseDataStream.write(data);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void completeWithError(int errorCode, String errorInfo) {
                metrics.end();

                int statusCode = errorCode;
                Map<String, String> header = null;
                JSONObject response = null;
                if (curlResponse != null) {
                    if (statusCode == 0) {
                        statusCode = curlResponse.statusCode;
                    }
                    header = curlResponse.allHeaderFields;
                }

                byte[] responseData = responseDataStream.toByteArray();
                if (responseData.length > 0) {
                    try {
                        String responseString = new String(responseData, "utf-8");
                        response = new JSONObject(responseString);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

                releaseResource();

                ResponseInfo responseInfo = ResponseInfo.create(request, statusCode, header, response, errorInfo);
                metrics.setResponse(responseInfo);
                if (complete != null) {
                    complete.complete(responseInfo, metrics, response);
                }
                Log.d("Curl", "====== completeWithError errorCode:" + errorCode + " errorInfo:" + errorInfo);
            }

            @Override
            public void sendProgress(long bytesSent, long totalBytesSent, long totalBytesExpectedToSend) {
                if (progress != null) {
                    progress.progress(totalBytesSent, totalBytesExpectedToSend);
                }
                Log.d("Curl", "====== sendProgress bytesSent:" + bytesSent + " totalBytesSent:" + totalBytesSent + " totalBytesExpectedToSend:" + totalBytesExpectedToSend);
            }

            @Override
            public void receiveProgress(long bytesReceive, long totalBytesReceive, long totalBytesExpectedToReceive) {
//                Log.i("Curl", "====== receiveProgress bytesReceive:" + bytesReceive + " totalBytesReceive:" + totalBytesReceive + " totalBytesExpectedToReceive:" + totalBytesExpectedToReceive);
            }

            @Override
            public void didFinishCollectingMetrics(CurlTransactionMetrics transactionMetrics) {
                metrics.end();
                metrics.setClientName(getClientId());
                metrics.setClientVersion(getCurlVersionInfo());
                if (transactionMetrics != null) {
                    metrics.setCountOfRequestHeaderBytesSent(transactionMetrics.getCountOfRequestHeaderBytesSent());
                    metrics.setCountOfRequestBodyBytesSent(transactionMetrics.getCountOfRequestBodyBytesSent());
                    metrics.setCountOfResponseHeaderBytesReceived(transactionMetrics.getCountOfResponseHeaderBytesReceived());
                    metrics.setCountOfResponseBodyBytesReceived(transactionMetrics.getCountOfResponseBodyBytesReceived());
                    if (transactionMetrics.getRemoteAddress() != null && transactionMetrics.getRemoteAddress().length() > 0) {
                        metrics.setRemoteAddress(transactionMetrics.getRemoteAddress());
                    }
                    if (transactionMetrics.getRemotePort() > 0) {
                        metrics.setRemotePort((int) transactionMetrics.getRemotePort());
                    }
                    metrics.setLocalAddress(transactionMetrics.getLocalAddress());
                    metrics.setLocalPort((int) transactionMetrics.getLocalPort());
                    metrics.setRequestStartDate(transactionMetrics.getRequestStartDate());
                    metrics.setRequestEndDate(transactionMetrics.getRequestEndDate());
                    metrics.setDomainLookupStartDate(transactionMetrics.getDnsStartDate());
                    metrics.setDomainLookupEndDate(transactionMetrics.getDnsEndDate());
                    metrics.setConnectStartDate(transactionMetrics.getConnectStartDate());
                    metrics.setConnectEndDate(transactionMetrics.getConnectEndDate());
                    metrics.setSecureConnectionStartDate(transactionMetrics.getSecureConnectionStartDate());
                    metrics.setSecureConnectionEndDate(transactionMetrics.getSecureConnectionEndDate());
                    metrics.setResponseStartDate(transactionMetrics.getResponseStartDate());
                    metrics.setResponseEndDate(transactionMetrics.getResponseEndDate());
                }
                Log.d("Curl", "====== didFinishCollectingMetrics metrics:" + metrics);
            }
        });
    }

    @Override
    public void cancel() {
        if (curl != null) {
            curl.cancel();
        }
    }

    @Override
    public String getClientId() {
        return "qn-curl";
    }

    String getCurlVersionInfo() {
        return config.version + ";" + Curl.getCurlVersion();
    }

    void userAgentAddCurlVersion(CurlRequest request) {
        Map<String, String> headers = request.getAllHeaders();
        Set<String> headerKeys = headers.keySet();
        String userAgentKey = "User-Agent";
        for (String key : headerKeys) {
            if (key.equalsIgnoreCase(userAgentKey)) {
                userAgentKey = key;
                break;
            }
        }
        String userAgent = headers.get(userAgentKey);
        userAgent = (userAgent != null ? (userAgent + ";") : "") + getClientId() + ":" + getCurlVersionInfo();
        headers.put(userAgentKey, userAgent);
    }

    private void releaseResource() {
        if (requestDataStream != null) {
            try {
                requestDataStream.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        if (responseDataStream != null) {
            try {
                responseDataStream.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
