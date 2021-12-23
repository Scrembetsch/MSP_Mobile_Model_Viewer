#include "etc_header.h"

EtcHeader::EtcHeader()
{}

EtcHeader::EtcHeader(unsigned char *data)
{
    paddedWidthMSB  = data[8];
    paddedWidthLSB  = data[9];
    paddedHeightMSB = data[10];
    paddedHeightLSB = data[11];
    widthMSB        = data[12];
    widthLSB        = data[13];
    heightMSB       = data[14];
    heightLSB       = data[15];
}

unsigned short EtcHeader::GetWidth(void)
{
    return (widthMSB << 8) | widthLSB;
}

unsigned short EtcHeader::GetHeight(void)
{
    return (heightMSB << 8) | heightLSB;
}

unsigned short EtcHeader::GetPaddedWidth(void)
{
    return (paddedWidthMSB << 8) | paddedWidthLSB;
}

unsigned short EtcHeader::GetPaddedHeight(void)
{
    return (paddedHeightMSB << 8) | paddedHeightLSB;
}

GLsizei EtcHeader::GetSize(GLenum internalFormat)
{
    if (internalFormat != GL_COMPRESSED_RG11_EAC       && internalFormat != GL_COMPRESSED_SIGNED_RG11_EAC &&
        internalFormat != GL_COMPRESSED_RGBA8_ETC2_EAC && internalFormat != GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC)
    {
        return (GetPaddedWidth() * GetPaddedHeight()) >> 1;
    }
    else
    {
        return (GetPaddedWidth() * GetPaddedHeight());
    }
}