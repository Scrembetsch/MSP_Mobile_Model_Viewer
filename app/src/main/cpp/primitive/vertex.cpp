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
