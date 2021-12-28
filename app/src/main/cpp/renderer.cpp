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
    delete[] Meshes;
}

void Renderer::Resize(int w, int h) {
    mWidth = w;
    mHeight = h;
    glViewport(0, 0, mWidth, mHeight);
    UpdateMatrices();
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
    UpdateMatrices();
}

void Renderer::MoveCamera(float x, float y)
{
    glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(x * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
    mCameraPos = glm::vec3(glm::vec4(mCameraPos, 1.0f) * rot_mat);
    rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(y * 0.1f), glm::vec3(0.0f, 0.0f, 1.0f));
    mCameraPos = glm::vec3(glm::vec4(mCameraPos, 1.0f) * rot_mat);
    UpdateMatrices();
}

void Renderer::UpdateMatrices()
{
    mProjectionMat = glm::perspective(glm::radians(mScale), float(mWidth) / float(mHeight), 0.1f, 100.0f);
    mViewMat = glm::lookAt(mCameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

float Renderer::GetDeltaTime() {
    return mDeltaTime;
}