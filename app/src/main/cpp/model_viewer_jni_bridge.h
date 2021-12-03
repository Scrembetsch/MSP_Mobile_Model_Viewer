#ifndef MSP_MOBILE_MODEL_VIEWER_MODEL_VIEWER_JNI_BRIDGE_H
#define MSP_MOBILE_MODEL_VIEWER_MODEL_VIEWER_JNI_BRIDGE_H
#include <jni.h>
#include "renderer.h"

static Renderer* g_Renderer = nullptr;
extern Renderer* CreateES30Renderer();

extern "C" JNIEXPORT void JNICALL Java_at_tributsch_msp_1mobile_1model_1viewer_ModelViewerJniBridge_Init(JNIEnv* env, jclass obj);
extern "C" JNIEXPORT void JNICALL Java_at_tributsch_msp_1mobile_1model_1viewer_ModelViewerJniBridge_Resize(JNIEnv* env, jclass obj, jint width, jint height);
extern "C" JNIEXPORT void JNICALL Java_at_tributsch_msp_1mobile_1model_1viewer_ModelViewerJniBridge_Step(JNIEnv* env, jclass obj);

#endif //MSP_MOBILE_MODEL_VIEWER_MODEL_VIEWER_JNI_BRIDGE_H
