#include "renderer_es_30.h"
#include "gl/gl_util.h"
#include "glm/glm.hpp"
#include "util.h"

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec2 aTexCoord;\n"
        "layout(location = 2) in vec3 aNormal;\n"
        "uniform mat4 uProjection;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uModel;\n"
        "out vec2 vTexCoords;\n"
        "void main() {\n"
        "    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);\n"
        "    vTexCoords = aTexCoord;\n"
        "}\n";

static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec2 vTexCoords;\n"
        "uniform sampler2D uDiffuseMap;\n"
        "uniform vec3 uDiffuse;\n"
        "out vec4 outColor;\n"
        "void main() {\n"
        "    outColor = texture(uDiffuseMap, vTexCoords) * vec4(uDiffuse, 1.0f);\n"
        "}\n";

Renderer* CreateES30Renderer() {
    RendererES30* renderer = new RendererES30;
    if (!renderer->Init()) {
        delete renderer;
        return nullptr;
    }
    return renderer;
}

RendererES30::RendererES30()
        : Renderer()
        , mEglContext(eglGetCurrentContext())
        , mProgram(0)
{
}

bool RendererES30::Init() {
    mProgram = GlUtil::CreateProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!mProgram)
        return false;

    glEnable(GL_DEPTH_TEST);

    LOGV("RENDERER30", "Using OpenGL ES 3.0 renderer");
    return true;
}

RendererES30::~RendererES30() {
    if (eglGetCurrentContext() != mEglContext)
        return;
    glDeleteProgram(mProgram);
}

void RendererES30::Draw() {
    glUseProgram(mProgram);
    glUniformMatrix4fv(glGetUniformLocation(mProgram, "uProjection"), 1, GL_FALSE, &mProjectionMat[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(mProgram, "uView"), 1, GL_FALSE, &mViewMat[0][0]);
//    mMesh.Draw(mProgram);

    for(int i = 0; i < mNumMeshes; i++)
    {
        mMeshes[i].Draw(mProgram);
    }
}