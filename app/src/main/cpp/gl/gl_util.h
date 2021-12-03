#ifndef MSP_MOBILE_MODEL_VIEWER_GL_UTIL_H
#define MSP_MOBILE_MODEL_VIEWER_GL_UTIL_H

#include "gl.h"
#include "../primitive/mesh.h"

#include <string>
#include <android/asset_manager.h>

namespace GlUtil {
    bool CheckGlError(const char* funcName);
    void PrintGlString(const char* name, GLenum s);
    GLuint CreateShader(GLenum shaderType, const char* src);
    GLuint CreateProgram(const char* vtxSrc, const char* fragSrc);

    void LoadObjMeshes(AAssetManager* assetManager, const std::string& path, Mesh*& meshes, unsigned int& numMeshes);
};

#endif //MSP_MOBILE_MODEL_VIEWER_GL_UTIL_H
