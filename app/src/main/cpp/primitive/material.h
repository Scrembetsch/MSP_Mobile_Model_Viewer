#ifndef MSP_MOBILE_MODEL_VIEWER_MATERIAL_H
#define MSP_MOBILE_MODEL_VIEWER_MATERIAL_H

#include "../gl/gl.h"
#include "texture.h"

class Material {
public:
    void Apply(GLuint shader) const;

    GLfloat Diffuse[3];
    Texture Texture;
};

#endif //MSP_MOBILE_MODEL_VIEWER_MATERIAL_H
