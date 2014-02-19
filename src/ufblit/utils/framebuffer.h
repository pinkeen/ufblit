#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

int FB_SwitchTTYToGraphicsMode(int ttynum);
int FB_SwitchTTYToTextMode();

void *FB_AcquirePixelBuffer(const char *fbdev);
void FB_ReleasePixelBuffer();

int FB_GetBitsPerPixel();
int FB_GetWidth();
int FB_GetHeight();

void FB_ClearScreen();

#endif // FRAMEBUFFER_H
