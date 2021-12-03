#ifndef MSP_ANDROID_OPENGL_ES_CUBE_H
#define MSP_ANDROID_OPENGL_ES_CUBE_H

#include "mesh.h"

class Cube : public Mesh{
public:
    Cube();
    ~Cube();

private:
    static const unsigned int cNumVertices = 36;
    static const Vertex cBaseCube[cNumVertices];
};

#endif //MSP_ANDROID_OPENGL_ES_CUBE_H
