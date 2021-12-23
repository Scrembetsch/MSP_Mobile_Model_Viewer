#ifndef MSP_MOBILE_MODEL_VIEWER_ETC_HEADER_H
#define MSP_MOBILE_MODEL_VIEWER_ETC_HEADER_H

#include "gl.h"

// Taken from https://github.com/ARM-software/opengl-es-sdk-for-android/blob/master/samples/tutorials/EtcTexture/jni/ETCHeader.h
class EtcHeader {
private:
    unsigned char paddedWidthMSB;
    unsigned char paddedWidthLSB;
    unsigned char paddedHeightMSB;
    unsigned char paddedHeightLSB;
    unsigned char widthMSB;
    unsigned char widthLSB;
    unsigned char heightMSB;
    unsigned char heightLSB;
public:
    EtcHeader();
    EtcHeader(unsigned char *data);

    unsigned short GetWidth(void);
    unsigned short GetHeight(void);
    unsigned short GetPaddedWidth(void);
    unsigned short GetPaddedHeight(void);
    GLsizei GetSize(GLenum internalFormat);
};

#endif //MSP_MOBILE_MODEL_VIEWER_ETC_HEADER_H
