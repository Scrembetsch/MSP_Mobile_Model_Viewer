#include "vertex.h"

GLint Vertex::GetPositionSize()
{
    return sizeof(Position) / sizeof(Position[0]);
}

GLint Vertex::GetColorSize()
{
    return sizeof(Color) / sizeof(Color[0]);
}

GLint Vertex::GetVertexSize()
{
    return sizeof(Position) + sizeof(Color);
}

void* Vertex::GetPositionOffset()
{
    return (void*)0;
}

void* Vertex::GetColorOffset()
{
    return (void*)sizeof(Position);
}