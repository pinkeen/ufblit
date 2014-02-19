#ifndef UFB_BLITTER_H
#define UFB_BLITTER_H

#include "ufb_image.h"
#include "ufb_types.h"

/**
 * rects must not be null
 */
void UFB_BlitImage(UFB_Image *dstImg, UFB_Rect *dstRect, UFB_Image *srcImg, UFB_Rect *srcRect);

#endif // UFB_BLITTER_H
