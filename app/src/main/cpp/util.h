#ifndef MSP_ANDROID_OPENGL_ES_UTIL_H
#define MSP_ANDROID_OPENGL_ES_UTIL_H

#include <android/log.h>

#define LOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGV(TAG, ...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)

namespace Util {

};

#endif //MSP_ANDROID_OPENGL_ES_UTIL_H
