#include "renderer.h"

class RendererES3: public Renderer {
public:
    RendererES3();
    ~RendererES3() override;
    bool Init();

private:
    void Draw() override;

    const EGLContext mEglContext;
    GLuint mProgram;
};
