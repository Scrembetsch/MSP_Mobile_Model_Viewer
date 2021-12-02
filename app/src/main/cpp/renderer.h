#ifndef MSP_ANDROID_OPENGL_ES_RENDERER_H
#define MSP_ANDROID_OPENGL_ES_RENDERER_H

#include "glm/glm.hpp"
#include "primitive/cube.h"

#include <chrono>

class Renderer {
public:
    virtual ~Renderer();
    void Resize(int w, int h);
    void Render();

protected:
    Renderer();
    virtual void Draw() = 0;
    virtual float GetDeltaTime();

    glm::mat4 mProjectionMat;
    glm::mat4 mViewMat;
    Cube mMesh;

private:
    void Step();

    float mDeltaTime;
    std::chrono::time_point<std::chrono::system_clock> mLastFrameTime;
};

#endif //MSP_ANDROID_OPENGL_ES_RENDERER_H
