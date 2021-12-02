#ifndef MSP_ANDROID_OPENGL_ES_GL_H
#define MSP_ANDROID_OPENGL_ES_GL_H

#if DYNAMIC_ES3
//    #include "gl3stub.h"
#else
    // Include the latest possible header file( GL version header )
    #if __ANDROID_API__ >= 24
        #include <GLES3/gl32.h>
    #elif __ANDROID_API__ >= 21
        #include <GLES3/gl31.h>
    #elif __ANDROID_API__ >= 18
        #include <GLES3/gl3.h>
    #endif
#endif

#include <EGL/egl.h>

#endif //MSP_ANDROID_OPENGL_ES_GL_H
