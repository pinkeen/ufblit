#include "ufb_image.h"

#include <stdlib.h>
#include <string.h>

#include "utils/tga.h"
#include "ufb_common.h"
#include "ufb_msgs.h"

/*
 *  Since we will use XBurst for some operations, pixels
 *  must be aligned to 4-byte boundary. As per glibc
 *  docs addresses returned by malloc are guaranteed to be
 *  multiple of 8 (or 16 on 64-bit systems) so no special
 *  aligned mallocs needed here.
 */

UFB_Image *UFB_CreateImage(UFBuint16 w, UFBuint16 h, UFBbool withAlpha)
{
    UFB_Image *image = malloc(sizeof(UFB_Image));

    image->bytesPerPixel = 2;

    if(withAlpha)
        image->bytesPerPixel = 3;

    image->size = w * h * image->bytesPerPixel;
    image->stride = w * image->bytesPerPixel;

    image->w = w;
    image->h = h;

    image->pixels = malloc(image->size);

    return image;
}

void UFB_FreeImage(UFB_Image *image)
{
    free(image->pixels);
    free(image);
}

UFB_Image *UFB_CreateImageFromFile(const char *filename)
{
    UFB_ImageType type = UFB_GuessImageTypeFromExt(filename);

    switch(type)
    {
        case UFB_IT_TGA:
            return UFB_CreateImageFromFileTGA(filename);

        case UFB_IT_PNG:
            return UFB_CreateImageFromFilePNG(filename);

        case UFB_IT_JPEG:
            return UFB_CreateImageFromFileJPEG(filename);

        default:
            xwarn("Could not guess image type from filename '%s'", filename);
            return NULL;
    }
}

UFB_Image *UFB_CreateImageFromFileTGA(const char *filename)
{
    TGA_Image tgaimg = TGA_LoadFromFile(filename);

    xassert(tgaimg.error == 0, "Error loading tga image '%s'", filename);

    if(tgaimg.yorigin == 0)
        xwarn("Tga image will be flipped, save with origin at upper-left corner!");

    UFB_Image *img;

    if(tgaimg.bpp == 32)
    {
        img = UFB_ConvertRGBA32PixelsToImage(tgaimg.data, tgaimg.width, tgaimg.height);
    }
    else if(tgaimg.bpp == 24)
    {
        img = UFB_ConvertRGB24PixelsToImage(tgaimg.data, tgaimg.width, tgaimg.height);
    }

    free(tgaimg.data);

    return img;
}

UFB_Image *UFB_CreateImageFromFileJPEG(const char *filename)
{
    xfatal("Reading JPEG files is not implemented yet ('%s')", filename);
    return NULL;
}

UFB_Image *UFB_CreateImageFromFilePNG(const char *filename)
{
    xfatal("Reading PNG files is not implemented yet ('%s')", filename);
    return NULL;
}

UFB_Image *UFB_ConvertRGB24PixelsToImage(void *pixels, UFBuint16 w, UFBuint16 h)
{
    UFB_Image *img = UFB_CreateImage(w, h, UFBfalse);

    UFBuint8 *end = (UFBuint8*)pixels + w * h * 3;

    UFBuint8 *ir = (UFBuint8*)pixels;
    UFBuint16 *op = (UFBuint16*)img->pixels;

    while(ir != end)
    {

        *op = RGB888TO565(*ir, *(ir + 1), *(ir + 2));

        ir += 3;
        ++op;
    }


    return img;
}

UFB_Image *UFB_ConvertRGBA32PixelsToImage(void *pixels, UFBuint16 w, UFBuint16 h)
{
    UFB_Image *img = UFB_CreateImage(w, h, UFBtrue);

    UFBuint32 *end = (UFBuint32*)pixels + w * h;

    UFBuint32 *ip = (UFBuint32*)pixels;
    UFBuint8 *op = (UFBuint8*)img->pixels;

    UFB_RGBA8888 *irgba;
    UFB_RGBA5658 *orgba;

    int c = 0;
    while(ip != end)
    {
        irgba = (UFB_RGBA8888*)ip;
        orgba = (UFB_RGBA5658*)op;

        orgba->rgb = RGB888TO565(irgba->r, irgba->g, irgba->b);
        orgba->a = irgba->a;

        ++ip;
        op += 3;
        ++c;
    }

    return img;
}

void UFB_ZeroImage(UFB_Image *image)
{
    memset(image->pixels, 0, image->size);
}

void UFB_FillImage(UFB_Image *image, UFBuint32 color)
{
    if(image->bytesPerPixel == 2)
    {
        UFBuint16 c = color & 0x0000FFFF;
        UFBuint16 *end = (UFBuint16*)image->pixels + image->w * image->h;
        UFBuint16 *p = (UFBuint16*)image->pixels;

        while(p != end)
        {
            *p = c;
            ++p;
        }

        return;
    }

    UFBuint8 *p = (UFBuint8*)image->pixels;
    UFBuint8 *end = (UFBuint8*)image->pixels + image->size - 3;

    UFBuint32 *tmp;

    while(p != end)
    {
        tmp = (UFBuint32*)p;
        *tmp = color;
        p += 3;
    }
}
