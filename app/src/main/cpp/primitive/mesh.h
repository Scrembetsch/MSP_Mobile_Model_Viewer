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
    void Draw(GLuint shader) const;

    glm::mat4 mModel;
    Material mMaterial;

protected:
    GLuint mVao;
    GLuint mVbo;

    unsigned int mNumVertices;
};

#endif //MSP_ANDROID_OPENGL_ES_MESH_H
