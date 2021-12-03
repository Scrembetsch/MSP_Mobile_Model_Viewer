#ifndef MSP_MOBILE_MODEL_VIEWER_UTIL_H
#define MSP_MOBILE_MODEL_VIEWER_UTIL_H

#include <android/log.h>

#define LOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGV(TAG, ...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)

#endif //MSP_MOBILE_MODEL_VIEWER_UTIL_H
