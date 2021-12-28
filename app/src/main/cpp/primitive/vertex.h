#ifndef MSP_ANDROID_OPENGL_ES_VERTEX_H
#define MSP_ANDROID_OPENGL_ES_VERTEX_H

#include "../gl/gl.h"
#include "../glm/glm.hpp"

class Vertex {
public:
    enum Attribute
    {
        POSITION,
        TEXCOORD,
        NORMAL,

        NUM_ATTRIBUTES
    };

    static GLint GetPositionSize();
    static GLint GetTexCoordSize();
    static GLint GetNormalSize();

    GLfloat Position[3];
    GLfloat TexCoord[2];
    GLfloat Normal[3];
};

#endif //MSP_ANDROID_OPENGL_ES_VERTEX_H
