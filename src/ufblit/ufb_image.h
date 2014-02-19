#ifndef UFB_IMAGE_H
#define UFB_IMAGE_H

#include "ufb_types.h"

typedef struct
{
	UFBuint8 bytesPerPixel;
	UFBsize stride;
	UFBsize size;
	UFBuint16 w;
	UFBuint16 h;
	void *pixels;
} UFB_Image;

UFB_Image *UFB_CreateImage(UFBuint16 w, UFBuint16 h, UFBbool withAlpha);
void UFB_FreeImage(UFB_Image *image);

UFB_Image *UFB_CreateImageFromFile(const char *filename);
UFB_Image *UFB_CreateImageFromFileTGA(const char *filename);
UFB_Image *UFB_CreateImageFromFileJPEG(const char *filename);
UFB_Image *UFB_CreateImageFromFilePNG(const char *filename);
UFB_Image *UFB_ConvertRGB24PixelsToImage(void *pixels, UFBuint16 w, UFBuint16 h);
UFB_Image *UFB_ConvertRGBA32PixelsToImage(void *pixels, UFBuint16 w, UFBuint16 h);

void UFB_ZeroImage(UFB_Image *image);
void UFB_FillImage(UFB_Image *image, UFBuint32 color);

#endif // UFB_IMAGE_H
