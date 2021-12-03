#ifndef MSP_MOBILE_MODEL_VIEWER_RENDERER_H
#define MSP_MOBILE_MODEL_VIEWER_RENDERER_H

#include "glm/glm.hpp"

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

private:
    void Step();

    float mDeltaTime;
    std::chrono::time_point<std::chrono::system_clock> mLastFrameTime;
};

#endif //MSP_MOBILE_MODEL_VIEWER_RENDERER_H
