//
// Created by yangsen on 2020/11/16.
//

#include <jni.h>

#ifndef QNCURLANDROID_CUEL_ERROR_H
#define QNCURLANDROID_CUEL_ERROR_H

static int kRequestSuccess = 200;
static int kZeroSizeFile = -6;
static int kInvalidFile = -3;
static int kCancelled = -999;
static int kNetworkError = -1;
static int kLocalIOError = -7;
static int kMaliciousResponseError = -8;

static int kTimedOut = -1001;
static int kUnknownHost = -1003;
static int kCannotConnectToHost = -1004;
static int kNetworkConnectionLost = -1005;
static int kNetworkSSLError = -1200;
static int kNetworkProtocolError = -1300;
static int kNetworkSlow = -1009;
static int kParseError = -1015;

int transformCurlStatusCode(int statusCode);


#endif //QNCURLANDROID_CUEL_ERROR_H
