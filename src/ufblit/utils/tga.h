#ifndef TGA_H
#define TGA_H

typedef struct                                      
{
    unsigned char *data;
    unsigned int width;
    unsigned int height;
    unsigned int bpp;
    unsigned char error;
    unsigned int xorigin;
    unsigned int yorigin;
} TGA_Image;

TGA_Image TGA_LoadFromFile(const char *filename);

#endif
