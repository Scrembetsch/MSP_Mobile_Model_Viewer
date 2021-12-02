#include "mesh.h"

Mesh::Mesh(const Vertex* model, unsigned int numVertices)
: mModel(1.0f)
, mVao(0)
, mVbo(0)
, mNumVertices(numVertices)
{
    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*model) * numVertices, model, GL_STATIC_DRAW);

    glBindVertexArray(mVao);
    glVertexAttribPointer(Vertex::Attribute::POSITION, Vertex::GetPositionSize(), GL_FLOAT, GL_FALSE, Vertex::GetVertexSize(), Vertex::GetPositionOffset());
    glEnableVertexAttribArray(Vertex::Attribute::POSITION);
    glVertexAttribPointer(Vertex::Attribute::COLOR, Vertex::GetColorSize(), GL_FLOAT, GL_FALSE, Vertex::GetVertexSize(), Vertex::GetColorOffset());
    glEnableVertexAttribArray(Vertex::Attribute::COLOR);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &mVbo);
    glDeleteVertexArrays(1, &mVao);
}

void Mesh::Draw(GLuint shader) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, &mModel[0][0]);
    glBindVertexArray(mVao);
    glDrawArrays(GL_TRIANGLES, 0, mNumVertices);
}