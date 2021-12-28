#ifndef MSP_ANDROID_OPENGL_ES_MESH_H
#define MSP_ANDROID_OPENGL_ES_MESH_H

#include "vertex.h"
#include "material.h"

class Mesh {
public:
    Mesh();
    Mesh(const Vertex* model, unsigned int numVertices);
    ~Mesh();

    void Init(const Vertex* model, unsigned int numVertices);
    void Init(const Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
    void Draw(GLuint shader) const;

    glm::mat4 mModel;
    Material mMaterial;

protected:
    void Cleanup(bool destructorCleanup = false);

    GLuint mVao;
    GLuint mVbo;
    GLuint mEbo;

    unsigned int mNumVertices;
    unsigned int mNumIndices;
};

#endif //MSP_ANDROID_OPENGL_ES_MESH_H
