#ifndef MSP_MOBILE_MODEL_VIEWER_GL_H
#define MSP_MOBILE_MODEL_VIEWER_GL_H

// Include the latest possible header file( GL version header )
#if __ANDROID_API__ >= 24
    #include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
    #include <GLES3/gl31.h>
#elif __ANDROID_API__ >= 18
    #include <GLES3/gl3.h>
#endif

#include <EGL/egl.h>

#endif //MSP_MOBILE_MODEL_VIEWER_GL_H
