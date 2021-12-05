#include "vertex.h"

GLint Vertex::GetPositionSize()
{
    return sizeof(Position) / sizeof(Position[0]);
}

GLint Vertex::GetTexCoordSize()
{
    return sizeof(TexCoord) / sizeof(TexCoord[0]);
}

GLint Vertex::GetNormalSize()
{
    return sizeof(Normal) / sizeof(Normal[0]);
}

GLint Vertex::GetVertexSize()
{
    return sizeof(Position) + sizeof(TexCoord) + sizeof(Normal);
}

void* Vertex::GetPositionOffset()
{
    return (void*)0;
}

void* Vertex::GetTexCoordOffset()
{
    return (void*)sizeof(Position);
}

void* Vertex::GetNormalOffset()
{
    return (void*)(sizeof(Position) + sizeof(TexCoord));
}