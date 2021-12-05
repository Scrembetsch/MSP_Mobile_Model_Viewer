#include "material.h"

void Material::Apply(GLuint shader) const
{
    glUniform3fv(glGetUniformLocation(shader, "uDiffuse"), 1, &Diffuse[0]);
    Texture.Use(shader);
}