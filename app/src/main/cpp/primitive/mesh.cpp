#include "mesh.h"
#include "../glm/ext.hpp"

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
    Cleanup(true);
}

void Mesh::Cleanup(bool destructorCleanup)
{
    if(!destructorCleanup)
    {
        mModel = glm::mat4(1.0f);
        mNumIndices = 0;
        mNumVertices = 0;
    }

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
    Cleanup(false);
    mNumVertices = numVertices;

    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);

    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*model) * numVertices, model, GL_STATIC_DRAW);

    glEnableVertexAttribArray(Vertex::Attribute::POSITION);
    glVertexAttribPointer(Vertex::Attribute::POSITION, Vertex::GetPositionSize(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(Vertex::Attribute::TEXCOORD);
    glVertexAttribPointer(Vertex::Attribute::TEXCOORD, Vertex::GetTexCoordSize(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

    glEnableVertexAttribArray(Vertex::Attribute::NORMAL);
    glVertexAttribPointer(Vertex::Attribute::NORMAL, Vertex::GetNormalSize(), GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}

void Mesh::Init(const Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
    Cleanup(false);
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
    glVertexAttribPointer(Vertex::Attribute::POSITION, Vertex::GetPositionSize(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(Vertex::Attribute::TEXCOORD);
    glVertexAttribPointer(Vertex::Attribute::TEXCOORD, Vertex::GetTexCoordSize(), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

    glEnableVertexAttribArray(Vertex::Attribute::NORMAL);
    glVertexAttribPointer(Vertex::Attribute::NORMAL, Vertex::GetNormalSize(), GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
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
    glBindVertexArray(0);
}