#include "renderer.h"

#include "gl/gl_util.h"
#include "glm/gtc/matrix_transform.hpp"

#include <ctime>

Renderer::Renderer()
        : mProjectionMat(1.0f)
        , mViewMat(1.0f)
        , mDeltaTime(0.0f)
        , mLastFrameTime(std::chrono::system_clock::now())
{
}

Renderer::~Renderer() {
    delete[] mMeshes;
}

void Renderer::Resize(int w, int h) {
    mWidth = w;
    mHeight = h;
    glViewport(0, 0, mWidth, mHeight);
    mProjectionMat = glm::perspective(glm::radians(mScale), float(mWidth) / float(mHeight), 0.1f, 100.0f);
    mViewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Renderer::Step() {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_seconds = now - mLastFrameTime;
    mDeltaTime = elapsed_seconds.count();

    mMesh.mModel = glm::rotate(mMesh.mModel, glm::radians(90.0f) * mDeltaTime, glm::vec3(5.0f, 5.0f, 1.0f));

    mLastFrameTime = now;
}

void Renderer::Render() {
    Step();

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Draw();
    GlUtil::CheckGlError("Renderer::Render");
}

void Renderer::SetScale(float scale)
{
    mScale *= scale;
    mProjectionMat = glm::perspective(glm::radians(mScale), float(mWidth) / float(mHeight), 0.1f, 100.0f);
    mViewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

float Renderer::GetDeltaTime() {
    return mDeltaTime;
}