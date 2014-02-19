/*
 *  2007?, 2010 (c) Filip Sobalski <pinkeen@gmail.com>
 *
 *  Loading adapted from NeHe's Opengl Tutorial Lesson 24.
 *
 *  Written years ago, now only dusted off for testing of
 *  ufblit lib.
 */

#include "tga.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct __attribute__((__packed__))
{
    uint8_t identsize;
    uint8_t colourmaptype;
    uint8_t imagetype;

    uint16_t colourmapstart;
    uint16_t colourmaplength;
    uint8_t colourmapbits;

    uint16_t xstart;
    uint16_t ystart;
    uint16_t width;
    uint16_t height;
    uint8_t bits;
    uint8_t descriptor;
} TGA_Header;

TGA_Image TGA_LoadFromFile(const char *filename)
{
    TGA_Image image;
    TGA_Header header;
    unsigned int bytesPerPixel;
    unsigned int imageSize;
    unsigned int temp;

    image.error = 0;
    FILE *file = fopen(filename, "rb");

    if(file == NULL)
    {
        image.error = 1;
        fprintf(stderr, "[TGA] Error opening file %s\n", filename);
        return image;
    }

    if(fread(&header, 1, sizeof(TGA_Header), file) != sizeof(TGA_Header))
    {
        fclose(file);
        image.error = 1;
        fprintf(stderr, "[TGA] Could not read header magic in %s\n", filename);
        return image;
    }


    if(header.identsize != 0 || header.colourmaptype != 0 || header.imagetype != 2)
    {
        fclose(file);
        image.error = 1;
        fprintf(stderr, "[TGA] Unsupported format in %s\n", filename);
        return image;
    }

    image.width  = header.width;
    image.height = header.height;

    if(header.bits != 24 && header.bits != 32)
    {
        fclose(file);
        image.error = 1;
        fprintf(stderr, "[TGA] Unsupported bpp (%u) in %s\n", header.bits, filename);
        return image;
    }

    if((image.width <= 0) || (image.height <= 0))
    {
        fclose(file);
        image.error = 1;
        fprintf(stderr, "[TGA] Bad width in %s\n", filename);
        return image;
    }

    image.bpp = header.bits;
    bytesPerPixel = image.bpp / 8;
    imageSize = image.width * image.height * bytesPerPixel;
    image.data = (unsigned char*)malloc(imageSize);
    image.xorigin = header.xstart;
    image.yorigin = header.ystart;

    if((image.data == NULL) || (fread(image.data, 1, imageSize, file) != imageSize))
    {
        if(image.data != NULL)
            free(image.data);

        fclose(file);

        image.error = 1;

        fprintf(stderr, "[TGA] Error allocating/reading data %s\n", filename);

        return image;
    }

    unsigned int i;

    for(i = 0; i < imageSize; i += bytesPerPixel)
    {
        temp = image.data[i];
        image.data[i] = image.data[i + 2];
        image.data[i + 2] = temp;
    }

    fclose (file);

    return image;
}

