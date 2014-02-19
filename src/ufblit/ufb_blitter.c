#include "ufb_blitter.h"

#include "ufb_msgs.h"
#include "ufb_common.h"

static inline UFBbool calcRects(UFB_Image *dstImg, UFB_Rect *dstRect, UFB_Image *srcImg, UFB_Rect *srcRect)
{
	// set dimensions to default if 0
	// and sanitize them

	// src rect

	if(srcRect->w == 0)
		srcRect->w = srcImg->w;

	if(srcRect->h == 0)
		srcRect->h = srcImg->h;

	if((srcRect->w + srcRect->x) > srcImg->w)
		srcRect->w = srcImg->w - srcRect->x;

	if((srcRect->h + srcRect->y) > srcImg->h)
		srcRect->h = srcImg->h - srcRect->y;

	if(srcRect->x < 0)
	{
		srcRect->w += srcRect->x;
		srcRect->x = 0;
	}

	if(srcRect->y < 0)
	{
		srcRect->h += srcRect->y;
		srcRect->y = 0;
	}

	// dest rect

	if(dstRect->w == 0)
		dstRect->w = srcRect->w;

	if(dstRect->h == 0)
		dstRect->h = srcRect->h;

	// choose the smaller rect - gives us clipping

	if(srcRect->w != dstRect->w)
		srcRect->w = dstRect->w = srcRect->w < dstRect->w ? srcRect->w : dstRect->w;

	if(srcRect->h != dstRect->h)
		srcRect->h = dstRect->h = srcRect->h < dstRect->h ? srcRect->h : dstRect->h;

	if((dstRect->w + dstRect->x) > dstImg->w)
	{
		dstRect->w = dstImg->w - dstRect->x;
		srcRect->w = dstRect->w;
	}

	if((dstRect->h + dstRect->y) > dstImg->h)
	{
		dstRect->h = dstImg->h - dstRect->y;
		srcRect->h = dstRect->h;
	}

	if(dstRect->x < 0)
	{
		dstRect->w += dstRect->x;
		srcRect->x -= dstRect->x;
		srcRect->w = dstRect->w;
		dstRect->x = 0;

	}

	if(dstRect->y < 0)
	{
		dstRect->h += dstRect->y;
		srcRect->y -= dstRect->y;
		srcRect->h = dstRect->h;
		dstRect->y = 0;
	}

	if(dstRect->w <= 0 || dstRect->h <= 0 || srcRect->w <= 0 || srcRect->h <= 0)
		return UFBfalse;

	return UFBtrue;
}

void UFB_BlitImage(UFB_Image *dstImg, UFB_Rect *dstRect, UFB_Image *srcImg, UFB_Rect *srcRect)
{
	if(!calcRects(dstImg, dstRect, srcImg, srcRect))
		return;

	/*printf("SRC %dx%d\n", srcImg->w, srcImg->h);
	printf("DST %dx%d\n", dstImg->w, dstImg->h);

	printf("SRC %d,%d - %dx%d\n", srcRect->x, srcRect->y, srcRect->w, srcRect->h);
	printf("DST %d,%d - %dx%d\n", dstRect->x, dstRect->y, dstRect->w, dstRect->h);*/

	void *srcline = srcImg->pixels + srcImg->stride * srcRect->y;
	void *dstline = dstImg->pixels + dstImg->stride * dstRect->y;

	UFBsize srcxoffs = srcRect->x * srcImg->bytesPerPixel;
	UFBsize dstxoffs = dstRect->x * dstImg->bytesPerPixel;


	UFBuint32 y = 0;

	while(y != srcRect->h)
	{
		void *srcpix = srcline + srcxoffs;
		void *dstpix = dstline + dstxoffs;

		UFBuint32 x = 0;

		while(x != srcRect->w)
		{
			// here goes the tough part...
			// of course the ifs should be moved out of the loop

			UFBuint16 srcrgb = *((UFBuint16*)srcpix);
			UFBuint16 dstrgb = *((UFBuint16*)dstpix);

			UFBuint8 srca = 0xFF;

			if(srcImg->bytesPerPixel == 3)
				srca = *((UFBuint8*)srcpix + 2);

			UFBuint8 srcad = 0xFF - srca;

			*((UFBuint16*)dstpix) = RGB565((R565(srcrgb) * srca + R565(dstrgb) * srcad) / 0xFF,
						       (G565(srcrgb) * srca + G565(dstrgb) * srcad) / 0xFF,
						       (B565(srcrgb) * srca + B565(dstrgb) * srcad) / 0xFF);

			srcpix += srcImg->bytesPerPixel;
			dstpix += dstImg->bytesPerPixel;
			++x;
		}

		srcline += srcImg->stride;
		dstline += dstImg->stride;
		++y;
	}

}
