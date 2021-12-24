#ifndef MSP_MOBILE_MODEL_VIEWER_UTIL_H
#define MSP_MOBILE_MODEL_VIEWER_UTIL_H

#include <android/log.h>
#include <string>

#define LOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGV(TAG, ...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)

namespace Util{
    bool EndsWith (std::string const &fullString, std::string const &ending);
}


#endif //MSP_MOBILE_MODEL_VIEWER_UTIL_H
