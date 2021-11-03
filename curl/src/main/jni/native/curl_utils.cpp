//
// Created by yangsen on 2020/10/22.
//

#include "curl_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char *curlUtilStrToLower(char *str) {
    if (str == NULL) {
        return str;
    }
    int i = 0;
    while (i < strlen(str)) {
        str[i] = static_cast<char>(tolower(str[i]));
        i++;
    }
    return str;
}

char *curlUtilStrStr(char *ps, char *pd) {
    char *pt = pd;
    int c = 0;
    while (*ps != '\0') {
        if (*ps == *pd) {
            while (*ps == *pd && *pd != '\0') {
                ps++;
                pd++;
                c++;
            }
        } else {
            ps++;
        }
        if (*pd == '\0') {
            return (ps - c);
        }
        c = 0;
        pd = pt;
    }
    return 0;
}

int curlUtilStrReplace(char *p_result, char *p_source, char *p_seach, char *p_repstr) {
    int c = 0;
    int repstr_leng = 0;
    int searchstr_leng = 0;

    char *p1;
    char *presult = p_result;
    char *psource = p_source;
    char *prep = p_repstr;
    char *pseach = p_seach;
    int nLen = 0;

    repstr_leng = strlen(prep);
    searchstr_leng = strlen(pseach);

    do {
        p1 = curlUtilStrStr(psource, p_seach);

        if (p1 == 0) {
            strcpy(presult, psource);
            return c;
        }
        c++;

        nLen = p1 - psource;
        memcpy(presult, psource, nLen);
        memcpy(presult + nLen, p_repstr, repstr_leng);

        psource = p1 + searchstr_leng;
        presult = presult + nLen + repstr_leng;
    } while (p1);

    return c;
}

long long curlUtilGetRequestContentLength(JNIEnv *env, jbyteArray body, jobjectArray header) {
    if (body != NULL) {
        return env->GetArrayLength(body);
    }

    if (header == NULL) {
        return 0;
    }

    char contentTypeKey[] = "content-length";
    char headerSegmentation[] = ":";
    long long contentLength = 0;
    for (int i = 0; i < env->GetArrayLength(header); ++i) {
        jstring headerField = (jstring) env->GetObjectArrayElement(header, i);
        jboolean isHeaderFieldCopy;
        const char *headerFieldChar = env->GetStringUTFChars(headerField, &isHeaderFieldCopy);
        if (strlen(headerFieldChar) < 30) {
            headerFieldChar = curlUtilStrToLower(const_cast<char *>(headerFieldChar));
        } else {
            continue;
        }

        if (strstr(headerFieldChar, contentTypeKey) != NULL &&
            strstr(headerFieldChar, headerSegmentation) != NULL) {

            char spaceChar[] = " ";
            char emptyChar[] = "";

            char *contentLengthSource = const_cast<char *>(headerFieldChar);
            char contentLengthChar0[30] = {0};
            curlUtilStrReplace(contentLengthChar0, contentLengthSource, spaceChar, emptyChar);

            contentLengthSource = contentLengthChar0;
            char contentLengthChar1[30] = {0};
            curlUtilStrReplace(contentLengthChar1, contentLengthSource, contentTypeKey, emptyChar);

            contentLengthSource = contentLengthChar1;
            char contentLengthChar2[30] = {0};
            curlUtilStrReplace(contentLengthChar2, contentLengthSource, headerSegmentation,
                               emptyChar);

            contentLength = atol(contentLengthChar2);
        }

        env->ReleaseStringUTFChars(headerField, headerFieldChar);
    }
    return contentLength;
}

// 返回的char *需要delete
char* curlUtilConvertJByteArrayToChars(JNIEnv *env, jbyteArray byteArray) {
    if (env == NULL) {
        return 0;
    }

    char *chars = NULL;
    jbyte *bytes = env->GetByteArrayElements(byteArray, 0);

    int chars_len = env->GetArrayLength(byteArray);
    chars = new char[chars_len + 1];
    memset(chars, 0, static_cast<size_t>(chars_len + 1));
    memcpy(chars, bytes, static_cast<size_t>(chars_len));
    chars[chars_len] = 0;

    env->ReleaseByteArrayElements(byteArray, bytes, 0);

    return chars;
}
