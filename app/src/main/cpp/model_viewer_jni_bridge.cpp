#include "model_viewer_jni_bridge.h"

#include "gl/gl_util.h"
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

    GlUtil::LoadObjMeshes(g_AssetManager, "box_stack.obj", g_Renderer->mMeshes, g_Renderer->mNumMeshes);
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