#include "texture.h"

Texture::Texture(GLuint tex, GLuint texLocation)
        : mTex(tex)
        , mTexLocation(texLocation)
{
}

Texture::~Texture()
{
    if (mTex != 0)
    {
        glDeleteTextures(1, &mTex);
        mTex = 0;
    }
}

void Texture::Use(GLuint shader, GLuint overrideLocation) const
{
    if(mTex != 0)
    {
        GLuint location = mTexLocation;
        if (overrideLocation != GL_TEXTURE)
        {
            location = overrideLocation;
        }
        glActiveTexture(location);
        glUniform1i(glGetUniformLocation(shader, "uDiffuseMap"), location - GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTex);
    }
}