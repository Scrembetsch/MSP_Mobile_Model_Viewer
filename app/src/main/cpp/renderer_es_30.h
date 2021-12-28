#ifndef MSP_MOBILE_MODEL_VIEWER_RENDERER_ES_30_H
#define MSP_MOBILE_MODEL_VIEWER_RENDERER_ES_30_H

#include "renderer.h"
#include "gl/gl.h"

class RendererES30: public Renderer {
public:
    RendererES30();
    ~RendererES30() override;
    bool Init();

private:
    void Draw() override;

    const EGLContext mEglContext;
    GLuint mProgram;

    std::vector<float> mMeasurments;
    int mCurrentMeasurment;
};

#endif //MSP_MOBILE_MODEL_VIEWER_RENDERER_ES_30_H
