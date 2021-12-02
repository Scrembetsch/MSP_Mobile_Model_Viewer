#ifndef MSP_ANDROID_OPENGL_ES_VERTEX_H
#define MSP_ANDROID_OPENGL_ES_VERTEX_H

#include "../gl/gl.h"
#include "../glm/glm.hpp"

class Vertex {
public:
    enum Attribute
    {
        POSITION,
        COLOR,

        NUM_ATTRIBUTES
    };

    static GLint GetPositionSize();
    static GLint GetColorSize();
    static GLint GetVertexSize();

    static void* GetPositionOffset();
    static void* GetColorOffset();

    GLfloat Position[3];
    GLfloat Color[3];
};

#endif //MSP_ANDROID_OPENGL_ES_VERTEX_H
