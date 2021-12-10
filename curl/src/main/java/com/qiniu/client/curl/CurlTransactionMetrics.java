package com.qiniu.client.curl;

import java.util.Date;

class CurlTransactionMetrics {
    private long countOfRequestHeaderBytesSent;
    private long countOfRequestBodyBytesSent;

    private long countOfResponseHeaderBytesReceived;
    private long countOfResponseBodyBytesReceived;

    private String localAddress;
    private long localPort;

    private String remoteAddress;
    private long remotePort;

    private long startTimestamp;
    private long nameLookupTime;
    private long connectTime;
    private long appConnectTime;
    private long preTransferTime;
    private long startTransferTime;
    private long totalTime;
    private long redirectTime;

    long getCountOfRequestHeaderBytesSent() {
        return countOfRequestHeaderBytesSent;
    }

    void setCountOfRequestHeaderBytesSent(long countOfRequestHeaderBytesSent) {
        this.countOfRequestHeaderBytesSent = countOfRequestHeaderBytesSent;
    }

    long getCountOfRequestBodyBytesSent() {
        return countOfRequestBodyBytesSent;
    }

    void setCountOfRequestBodyBytesSent(long countOfRequestBodyBytesSent) {
        this.countOfRequestBodyBytesSent = countOfRequestBodyBytesSent;
    }

    long getCountOfResponseHeaderBytesReceived() {
        return countOfResponseHeaderBytesReceived;
    }

    void setCountOfResponseHeaderBytesReceived(long countOfResponseHeaderBytesReceived) {
        this.countOfResponseHeaderBytesReceived = countOfResponseHeaderBytesReceived;
    }

    long getCountOfResponseBodyBytesReceived() {
        return countOfResponseBodyBytesReceived;
    }

    void setCountOfResponseBodyBytesReceived(long countOfResponseBodyBytesReceived) {
        this.countOfResponseBodyBytesReceived = countOfResponseBodyBytesReceived;
    }

    String getLocalAddress() {
        return localAddress;
    }

    void setLocalAddress(String localAddress) {
        this.localAddress = localAddress;
    }

    long getLocalPort() {
        return localPort;
    }

    void setLocalPort(long localPort) {
        this.localPort = localPort;
    }

    String getRemoteAddress() {
        return remoteAddress;
    }

    void setRemoteAddress(String remoteAddress) {
        this.remoteAddress = remoteAddress;
    }

    long getRemotePort() {
        return remotePort;
    }

    void setRemotePort(long remotePort) {
        this.remotePort = remotePort;
    }

    Date getDnsStartDate() {
        if (startTimestamp < 10000) {
            return null;
        }
        return new Date(startTimestamp);
    }

    Date getDnsEndDate() {
        if (startTimestamp < 10000) {
            return null;
        }
        return new Date(startTimestamp + nameLookupTime);
    }

    Date getRequestStartDate() {
        if (startTimestamp < 10000) {
            return null;
        }
        return new Date(startTimestamp);
    }

    Date getRequestEndDate() {
        if (startTimestamp < 10000) {
            return null;
        }
        return new Date(startTimestamp + totalTime);
    }

    Date getConnectStartDate() {
        return getDnsEndDate();
    }

    Date getConnectEndDate() {
        if (startTimestamp < 10000) {
            return null;
        }
        return new Date(startTimestamp + nameLookupTime + connectTime);
    }

    Date getSecureConnectionStartDate() {
        return getConnectEndDate();
    }

    Date getSecureConnectionEndDate() {
        if (startTimestamp < 10000) {
            return null;
        }
        return new Date(startTimestamp + nameLookupTime + connectTime + appConnectTime);
    }

    Date getResponseStartDate() {
        return getSecureConnectionEndDate();
    }

    Date getResponseEndDate() {
        if (startTimestamp < 10000) {
            return null;
        }
        return new Date(startTimestamp + nameLookupTime + connectTime + appConnectTime +
                preTransferTime + startTransferTime + redirectTime);
    }

    void setStartTimestamp() {
        this.startTimestamp = new Date().getTime();
    }

    void setNameLookupTime(long lookupTime) {
        this.nameLookupTime = lookupTime;
    }

    void setConnectTime(long connectTime) {
        this.connectTime = connectTime;
    }

    void setAppConnectTime(long appConnectTime) {
        this.appConnectTime = appConnectTime;
    }

    void setPreTransferTime(long preTransferTime) {
        this.preTransferTime = preTransferTime;
    }

    void setStartTransferTime(long startTransferTime) {
        this.startTransferTime = startTransferTime;
    }

    void setTotalTime(long totalTime) {
        this.totalTime = totalTime;
    }

    void setRedirectTime(long redirectTime) {
        this.redirectTime = redirectTime;
    }
}
