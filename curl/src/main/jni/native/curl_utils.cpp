//
// Created by yangsen on 2020/10/22.
//

#include "curl_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
