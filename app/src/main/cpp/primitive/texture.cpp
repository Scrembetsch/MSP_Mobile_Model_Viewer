#include "texture.h"
#include "../gl/gl_util.h"

GLuint Texture::sWhite = 0;

void Texture::Init(AAssetManager* assetManager, const std::string& whitePath)
{
    sWhite = GlUtil::LoadTexture(assetManager, whitePath);
}

Texture::Texture(GLuint tex, GLuint texLocation)
        : mTex(tex)
        , mTexLocation(texLocation)
{
}

Texture::~Texture()
{
    if (mTex != 0
        && mTex != sWhite)
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