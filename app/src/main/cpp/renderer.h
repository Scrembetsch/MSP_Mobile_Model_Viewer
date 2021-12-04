#ifndef MSP_MOBILE_MODEL_VIEWER_RENDERER_H
#define MSP_MOBILE_MODEL_VIEWER_RENDERER_H

#include "primitive/cube.h"

#include <chrono>
#include <vector>

class Renderer {
public:
    virtual ~Renderer();
    void Resize(int w, int h);
    void Render();
    void SetScale(float scale);
    void MoveCamera(float x, float y);

    Mesh* mMeshes;
    unsigned int mNumMeshes;

protected:
    Renderer();
    virtual void Draw() = 0;
    float GetDeltaTime();

    glm::mat4 mProjectionMat;
    glm::mat4 mViewMat;

    Cube mMesh;

private:
    void Step();
    void UpdateMatrices();

    float mDeltaTime;
    float mScale = 45.0f;
    float mWidth = 1080;
    float mHeight = 1920;

    glm::vec3 mCameraPos = glm::vec3 (0.0f, 0.0f, 5.0f);

    std::chrono::time_point<std::chrono::system_clock> mLastFrameTime;
};

#endif //MSP_MOBILE_MODEL_VIEWER_RENDERER_H
