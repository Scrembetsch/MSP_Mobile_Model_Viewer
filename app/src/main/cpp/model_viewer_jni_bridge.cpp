#include "model_viewer_jni_bridge.h"

#include "gl/gl_util.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "util.h"

#include <cstring>

extern "C" JNIEXPORT void JNICALL Java_at_tributsch_msp_1mobile_1model_1viewer_ModelViewerJniBridge_Init(JNIEnv* env, jclass obj, jobject assetManager)
{
    if (g_Renderer) {
        delete g_Renderer;
        g_Renderer = NULL;
    }

    g_AssetManager = AAssetManager_fromJava(env, assetManager);

    GlUtil::PrintGlString("Version", GL_VERSION);
    GlUtil::PrintGlString("Vendor", GL_VENDOR);
    GlUtil::PrintGlString("Renderer", GL_RENDERER);
    GlUtil::PrintGlString("Extensions", GL_EXTENSIONS);

    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.")) {
        g_Renderer = CreateES30Renderer();
    } else {
        LOGE("JNI_BRIDGE", "Unsupported OpenGL ES version");
    }

    Texture::Init(g_AssetManager, "default/white.png");

    GlUtil::LoadMeshes(g_AssetManager, "models/texcube/", "test.leb", g_Renderer->mMeshes, g_Renderer->mNumMeshes);
//    GlUtil::LoadMeshes(g_AssetManager, "models/cube/", "cube.obj", g_Renderer->mMeshes, g_Renderer->mNumMeshes);
//    GlUtil::LoadMeshes(g_AssetManager, "models/simple_cube/", "cube.obj", g_Renderer->mMeshes, g_Renderer->mNumMeshes);
//    GlUtil::LoadMeshes(g_AssetManager, "models/tank/", "old_tank.leb", g_Renderer->mMeshes, g_Renderer->mNumMeshes);
//    GlUtil::LoadMeshes(g_AssetManager, "models/box/", "box_stack.obj", g_Renderer->mMeshes, g_Renderer->mNumMeshes);
//    GlUtil::LoadMeshes(g_AssetManager, "models/tree/", "objSignal.obj", g_Renderer->mMeshes, g_Renderer->mNumMeshes);

    for(int i = 0; i < g_Renderer->mNumMeshes; i++)
    {
        float scale = 1.01f;
        g_Renderer->mMeshes[i].mModel = glm::scale(g_Renderer->mMeshes[i].mModel, glm::vec3(scale, scale, scale));
    }
}

extern "C" JNIEXPORT void JNICALL Java_at_tributsch_msp_1mobile_1model_1viewer_ModelViewerJniBridge_Resize(JNIEnv* env, jclass obj, jint width, jint height)
{
    if (g_Renderer) {
        g_Renderer->Resize(width, height);
    }
}

extern "C" JNIEXPORT void JNICALL Java_at_tributsch_msp_1mobile_1model_1viewer_ModelViewerJniBridge_Step(JNIEnv* env, jclass obj)
{
    if (g_Renderer) {
        g_Renderer->Render();
    }
}

extern "C" JNIEXPORT void JNICALL Java_at_tributsch_msp_1mobile_1model_1viewer_ModelViewerJniBridge_SetScale(JNIEnv* env, jclass obj, jfloat scale)
{
    if(g_Renderer)
    {
        g_Renderer->SetScale(scale);
    }
}

extern "C" JNIEXPORT void JNICALL Java_at_tributsch_msp_1mobile_1model_1viewer_ModelViewerJniBridge_Move(JNIEnv* env, jclass obj, jfloat moveX, jfloat moveY)
{
    if(g_Renderer)
    {
        g_Renderer->MoveCamera(moveX, moveY);
    }
}