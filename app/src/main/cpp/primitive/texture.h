#ifndef MSP_MOBILE_MODEL_VIEWER_TEXTURE_H
#define MSP_MOBILE_MODEL_VIEWER_TEXTURE_H

#include "../gl/gl.h"

class Texture
{
public:
    Texture(GLuint tex = 0, GLuint texLocation = GL_TEXTURE0);

    ~Texture();

    void Use(GLuint shader, GLuint overrideLocation = GL_TEXTURE) const;

    GLuint mTex;
    GLuint mTexLocation;
};

#endif //MSP_MOBILE_MODEL_VIEWER_TEXTURE_H
