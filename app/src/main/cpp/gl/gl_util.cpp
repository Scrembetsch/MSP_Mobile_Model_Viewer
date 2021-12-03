#include "gl_util.h"

#include "obj_loader_.h"

#include "../util.h"

#include <malloc.h>
#include <filesystem>

bool GlUtil::CheckGlError(const char* funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGE("GL_UTIL", "GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

void GlUtil::PrintGlString(const char* name, GLenum s) {
    const char* v = (const char*)glGetString(s);
    LOGV("GL_UTIL", "GL %s: %s\n", name, v);
}

GLuint GlUtil::CreateShader(GLenum shaderType, const char* src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        CheckGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                LOGE("GL_UTIL", "Could not compile %s shader:\n%s\n",
                     shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                     infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint GlUtil::CreateProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = CreateShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = CreateShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        CheckGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        LOGE("GL_UTIL", "Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar* infoLog = (GLchar*)malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                LOGE("GL_UTIL", "Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

void GlUtil::LoadObjMeshes(AAssetManager* assetManager, const std::string& path, Mesh*& meshes, unsigned int& numMeshes)
{
    objl::Loader loader;
    loader.LoadFile(assetManager, path);

    meshes = new Mesh[loader.LoadedMeshes.size()];
    numMeshes = loader.LoadedMeshes.size();

    for(int i = 0; i < loader.LoadedMeshes.size(); i++) {
        std::vector<unsigned int>& loadedIndices = loader.LoadedMeshes[i].Indices;
        std::vector<objl::Vertex>& loadedVertices = loader.LoadedMeshes[i].Vertices;

        Vertex *vertices = new Vertex[loadedIndices.size()];
        for (int j = 0; j < loadedIndices.size(); j++) {
            vertices[j].Position[0] = loadedVertices[loadedIndices[j]].Position.X;
            vertices[j].Position[1] = loadedVertices[loadedIndices[j]].Position.Y;
            vertices[j].Position[2] = loadedVertices[loadedIndices[j]].Position.Z;
            vertices[j].Color[0] = 0.0f;
            vertices[j].Color[1] = 0.0f;
            vertices[j].Color[2] = 0.0f;
        }
        meshes[i].Init(vertices, loadedIndices.size());
        delete[] vertices;
    }
}