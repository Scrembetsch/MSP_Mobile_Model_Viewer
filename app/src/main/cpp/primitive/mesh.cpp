#include "mesh.h"

Mesh::Mesh()
: mModel(1.0f)
, mVao(0)
, mVbo(0)
, mEbo(0)
, mNumVertices(0)
{
}

Mesh::Mesh(const Vertex* model, unsigned int numVertices)
: Mesh()
{
    Init(model, numVertices);
}

Mesh::~Mesh()
{
    if(mEbo != 0)
    {
        glDeleteBuffers(1, &mEbo);
    }
    if(mVbo != 0)
    {
        glDeleteBuffers(1, &mVbo);
    }
    if(mVao != 0)
    {
        glDeleteVertexArrays(1, &mVao);
    }
}

void Mesh::Init(const Vertex* model, unsigned int numVertices)
{
    mNumVertices = numVertices;

    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*model) * numVertices, model, GL_STATIC_DRAW);

    glBindVertexArray(mVao);
    glVertexAttribPointer(Vertex::Attribute::POSITION, Vertex::GetPositionSize(), GL_FLOAT, GL_FALSE, Vertex::GetVertexSize(), Vertex::GetPositionOffset());
    glEnableVertexAttribArray(Vertex::Attribute::POSITION);

    glVertexAttribPointer(Vertex::Attribute::TEXCOORD, Vertex::GetTexCoordSize(), GL_FLOAT, GL_FALSE, Vertex::GetVertexSize(), Vertex::GetTexCoordOffset());
    glEnableVertexAttribArray(Vertex::Attribute::TEXCOORD);

    glVertexAttribPointer(Vertex::Attribute::NORMAL, Vertex::GetNormalSize(), GL_FLOAT, GL_TRUE, Vertex::GetVertexSize(), Vertex::GetNormalOffset());
    glEnableVertexAttribArray(Vertex::Attribute::NORMAL);
}

void Mesh::Init(const Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
    mNumVertices = numVertices;
    mNumIndices = numIndices;

    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);
    glGenBuffers(1, &mEbo);

    glBindVertexArray(mVao);

    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(Vertex::Attribute::POSITION);
    glVertexAttribPointer(Vertex::Attribute::POSITION, Vertex::GetPositionSize(), GL_FLOAT, GL_FALSE, Vertex::GetVertexSize(), Vertex::GetPositionOffset());

    glEnableVertexAttribArray(Vertex::Attribute::TEXCOORD);
    glVertexAttribPointer(Vertex::Attribute::TEXCOORD, Vertex::GetTexCoordSize(), GL_FLOAT, GL_FALSE, Vertex::GetVertexSize(), Vertex::GetTexCoordOffset());

    glEnableVertexAttribArray(Vertex::Attribute::NORMAL);
    glVertexAttribPointer(Vertex::Attribute::NORMAL, Vertex::GetNormalSize(), GL_FLOAT, GL_TRUE, Vertex::GetVertexSize(), Vertex::GetNormalOffset());
}

void Mesh::Draw(GLuint shader) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader, "uModel"), 1, GL_FALSE, &mModel[0][0]);
    mMaterial.Apply(shader);
    glBindVertexArray(mVao);
    if(mEbo == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, mNumVertices);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);
    }
}