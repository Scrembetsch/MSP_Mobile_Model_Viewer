#include "gl_util.h"

#include "obj_loader.h"
#include "bin_loader.h"
#include "etc_header.h"

#include "../primitive/texture.h"

#include "../util.h"
#include "../timer.h"

#include <cmath>
#include <malloc.h>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

GLuint GlUtil::LoadTexture(AAssetManager* assetManager, const std::string& filepath)
{
    if(Util::EndsWith(filepath, ".pkm"))
    {
        return LoadPkmTexture(assetManager, filepath);
    }


    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    AAsset * file = AAssetManager_open (assetManager, filepath.c_str(), AASSET_MODE_UNKNOWN);
    off_t assetLength = AAsset_getLength (file);
    unsigned char * fileData = (unsigned char *) AAsset_getBuffer (file);

    unsigned char * data = stbi_load_from_memory (fileData, assetLength, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        LOGE("GL_UTIL", "Texture failed to load at path: %s", filepath.c_str());
        stbi_image_free(data);
    }
    return textureID;
}

GLuint GlUtil::LoadPkmTexture(AAssetManager* assetManager, const std::string& filepath)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    AAsset * file = AAssetManager_open (assetManager, filepath.c_str(), AASSET_MODE_UNKNOWN);
    char * fileData = (char *) AAsset_getBuffer (file);

    EtcHeader header(fileData);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB8_ETC2, header.GetWidth(), header.GetHeight(), 0, header.GetSize(GL_COMPRESSED_RGB8_ETC2), fileData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

void GlUtil::LoadMeshes(AAssetManager* assetManager, const std::string& path, const std::string& filename, Mesh*& meshes, unsigned int& numMeshes)
{
    if(Util::EndsWith(filename, ".obj"))
    {
        LoadObjMeshes(assetManager, path, filename, meshes, numMeshes);
    }
    else if(Util::EndsWith(filename, ".leb"))
    {
        LoadBinMeshes(assetManager, path, filename, meshes, numMeshes);
    }
    else
    {
        LOGE("GL_UTIL", "Unsupported filetype: %s", filename.c_str());
    }
}

void GlUtil::LoadObjMeshes(AAssetManager* assetManager, const std::string& path, const std::string& filename, Mesh*& meshes, unsigned int& numMeshes)
{
    Timer::GetInstance()->StartRecord("LoadObj");

    objl::Loader loader;
    loader.LoadFile(assetManager, path + filename);

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
            vertices[j].TexCoord[0] = loadedVertices[loadedIndices[j]].TextureCoordinate.X;
            vertices[j].TexCoord[1] = loadedVertices[loadedIndices[j]].TextureCoordinate.Y;
            vertices[j].Normal[0] = loadedVertices[loadedIndices[j]].Normal.X;
            vertices[j].Normal[1] = loadedVertices[loadedIndices[j]].Normal.Y;
            vertices[j].Normal[2] = loadedVertices[loadedIndices[j]].Normal.Z;
        }
        meshes[i].Init(vertices, loadedIndices.size());

        meshes[i].mMaterial.Diffuse[0] = loader.LoadedMeshes[i].MeshMaterial.Kd.X;
        meshes[i].mMaterial.Diffuse[1] = loader.LoadedMeshes[i].MeshMaterial.Kd.Y;
        meshes[i].mMaterial.Diffuse[2] = loader.LoadedMeshes[i].MeshMaterial.Kd.Z;

        if(!loader.LoadedMeshes[i].MeshMaterial.map_Kd.empty())
        {
            meshes[i].mMaterial.Texture.mTex = LoadTexture(assetManager, path + loader.LoadedMeshes[i].MeshMaterial.map_Kd);
        }
        else
        {
            meshes[i].mMaterial.Texture.mTex = Texture::sWhite;
        }

        delete[] vertices;
    }
    Timer::GetInstance()->StopRecord("LoadObj");
    LOGE("TIMER", "%s", Timer::GetInstance()->ToString().c_str());
}

void GlUtil::LoadBinMeshes(AAssetManager* assetManager, const std::string& path, const std::string& filename, Mesh*& meshes, unsigned int& numMeshes)
{
    Timer::GetInstance()->StartRecord("LoadBin");

    binl::Loader loader;
    loader.LoadFile(assetManager, path + filename);

    meshes = new Mesh[loader.NumMeshes];
    numMeshes = loader.NumMeshes;

    for(int i = 0; i < loader.NumMeshes; i++) {
        Vertex *vertices = new Vertex[loader.Meshes[i].NumVertices];
        for (int j = 0; j < loader.Meshes[i].NumVertices; j++) {
//            Vertices[j].Position[0] = loader.mVertPos[loader.Meshes[i].Vertices[j].PositionIndex].X;
//            Vertices[j].Position[1] = loader.mVertPos[loader.Meshes[i].Vertices[j].PositionIndex].Y;
//            Vertices[j].Position[2] = loader.mVertPos[loader.Meshes[i].Vertices[j].PositionIndex].Z;
//            Vertices[j].TexCoord[0] = loader.mTexCoords[loader.Meshes[i].Vertices[j].TexCoordIndex].X;
//            Vertices[j].TexCoord[1] = loader.mTexCoords[loader.Meshes[i].Vertices[j].TexCoordIndex].Y;
//            Vertices[j].Normal[0] = loader.mNormals[loader.Meshes[i].Vertices[j].NormalIndex].X;
//            Vertices[j].Normal[1] = loader.mNormals[loader.Meshes[i].Vertices[j].NormalIndex].Y;
//            Vertices[j].Normal[2] = loader.mNormals[loader.Meshes[i].Vertices[j].NormalIndex].Z;

            vertices[j].Position[0] = loader.Meshes[i].Vertices[j].Position.X;
            vertices[j].Position[1] = loader.Meshes[i].Vertices[j].Position.Y;
            vertices[j].Position[2] = loader.Meshes[i].Vertices[j].Position.Z;
            vertices[j].TexCoord[0] = loader.Meshes[i].Vertices[j].TextureCoordinate.X;
            vertices[j].TexCoord[1] = loader.Meshes[i].Vertices[j].TextureCoordinate.Y;
            vertices[j].Normal[0] = loader.Meshes[i].Vertices[j].Normal.X;
            vertices[j].Normal[1] = loader.Meshes[i].Vertices[j].Normal.Y;
            vertices[j].Normal[2] = loader.Meshes[i].Vertices[j].Normal.Z;
        }
        meshes[i].Init(vertices, loader.Meshes[i].NumVertices, loader.Meshes[i].Indices, loader.Meshes[i].NumIndices);

        meshes[i].mMaterial.Diffuse[0] = loader.Materials[loader.Meshes[i].MaterialIndex].Kd.X;
        meshes[i].mMaterial.Diffuse[1] = loader.Materials[loader.Meshes[i].MaterialIndex].Kd.Y;
        meshes[i].mMaterial.Diffuse[2] = loader.Materials[loader.Meshes[i].MaterialIndex].Kd.Z;

        if(!loader.Materials[loader.Meshes[i].MaterialIndex].map_Kd.empty()) {
            meshes[i].mMaterial.Texture.mTex = LoadTexture(assetManager, path + loader.Materials[loader.Meshes[i].MaterialIndex].map_Kd);
        }else{
            meshes[i].mMaterial.Texture.mTex = Texture::sWhite;
        }
        delete[] vertices;
    }
    Timer::GetInstance()->StopRecord("LoadBin");
    LOGE("TIMER", "%s", Timer::GetInstance()->ToString().c_str());
}