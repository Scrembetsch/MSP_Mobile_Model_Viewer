#ifndef MSP_ANDROID_OPENGL_ES_GL_UTIL_H
#define MSP_ANDROID_OPENGL_ES_GL_UTIL_H

#include "gl.h"

namespace GlUtil {
    bool CheckGlError(const char* funcName);
    void PrintGlString(const char* name, GLenum s);
    GLuint CreateShader(GLenum shaderType, const char* src);
    GLuint CreateProgram(const char* vtxSrc, const char* fragSrc);
};

#endif //MSP_ANDROID_OPENGL_ES_GL_UTIL_H
