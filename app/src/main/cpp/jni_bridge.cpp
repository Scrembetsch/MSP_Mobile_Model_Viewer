#include "jni_bridge.h"
#include "gl/gl_util.h"
#include "util.h"

#include <jni.h>
#include <cstdlib>
#include <cstring>

static Renderer* g_Renderer = NULL;

#if !defined(DYNAMIC_ES3)
static GLboolean gl3stubInit() {
    return GL_TRUE;
}
#endif

extern "C" JNIEXPORT void JNICALL
Java_at_tributsch_msp_1android_1opengl_1es_GlEs30JniLib_init(JNIEnv* env, jclass obj) {
    if (g_Renderer) {
        delete g_Renderer;
        g_Renderer = NULL;
    }

    GlUtil::PrintGlString("Version", GL_VERSION);
    GlUtil::PrintGlString("Vendor", GL_VENDOR);
    GlUtil::PrintGlString("Renderer", GL_RENDERER);
    GlUtil::PrintGlString("Extensions", GL_EXTENSIONS);

    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.") && gl3stubInit()) {
        g_Renderer = CreateES3Renderer();
    } else {
        LOGE("JNI_BRIDGE", "Unsupported OpenGL ES version");
    }
}

extern "C" JNIEXPORT void JNICALL
Java_at_tributsch_msp_1android_1opengl_1es_GlEs30JniLib_resize(JNIEnv* env, jclass obj, jint width, jint height) {
    if (g_Renderer) {
        g_Renderer->Resize(width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_at_tributsch_msp_1android_1opengl_1es_GlEs30JniLib_step(JNIEnv* env, jclass obj) {
    if (g_Renderer) {
        g_Renderer->Render();
    }
}