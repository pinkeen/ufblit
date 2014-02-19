#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string.h>
#include <math.h>

#include <SDL/SDL.h>

#include <ufblit/ufb.h>
#include <ufblit/utils/framebuffer.h>

#define SPEED 100.0f

typedef struct
{
	float x;
	float y;
	float vx;
	float vy;
} Star;

int main(int argc, char **argv)
{
	FB_SwitchTTYToGraphicsMode(1);
	void *screenPixels = FB_AcquirePixelBuffer("/dev/fb0");
	FB_ClearScreen();

	if(!screenPixels)
	{
		fprintf(stderr, "Could not acquire framebuffer pixels!\n");
		return 1;
	}

	int screenWidth = FB_GetWidth();
	int screenHeight = FB_GetHeight();
	int screenDepth = FB_GetBitsPerPixel();

	if(screenDepth != 16 || screenWidth != 320 || screenHeight != 240)
	{
		fprintf(stderr, "This application requires 320x240x16bit framebuffer. You have %dx%dx%dbit.\n",
			screenWidth, screenHeight, screenDepth);
		//return 1;
	}

	int starCount = 100;

	if(argc == 2)
		starCount = atoi(argv[1]);

	srandom(1000);

	printf("***UFB-FB-TEST***\n");

	UFB_Image *starImg = UFB_CreateImageFromFile("fav24.tga");
	UFB_Image *screenImg = UFB_CreateImage(320, 240, UFBfalse);

	Star stars[starCount];

	float maxx = 320 - starImg->w;
	float maxy = 240 - starImg->h;

	int i;
	for(i = 0; i < starCount; ++i)
	{
		Star *star = stars + i;
		star->x = random() % (int)maxx;
		star->y = random() % (int)maxy;

		float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * M_PI;

		star->vx = cosf(angle) * SPEED;
		star->vy = sinf(angle) * SPEED;
	}

	Uint32 old_ticks = SDL_GetTicks();
	float lag = 0.0f;
	int exit = 0;
	int counter = 0;
	float time = 0.0f;

	while(!exit)
	{
		lag = (double)(SDL_GetTicks() - old_ticks) / 1000.0;
		old_ticks = SDL_GetTicks();

		UFB_ZeroImage(screenImg);

		int i;
		for(i = 0; i < starCount; ++i)
		{
			Star *star = stars + i;

			star->x += lag * star->vx;
			star->y += lag * star->vy;

			if(star->x < 0.0f)
			{
				star->x = 0.0f;
				star->vx = -star->vx;
			}

			if(star->y < 0.0f)
			{
				star->y = 0.0f;
				star->vy = -star->vy;
			}

			if(star->x > maxx)
			{
				star->x = maxx;
				star->vx = -star->vx;
			}

			if(star->y > maxy)
			{
				star->y = maxy;
				star->vy = -star->vy;
			}


			UFB_Rect srcRect = {0, 0, 0, 0};
			UFB_Rect dstRect = {star->x, star->y, 0, 0};
			UFB_BlitImage(screenImg, &dstRect, starImg, &srcRect);
		}

		memcpy(screenPixels, screenImg->pixels, screenImg->size);

		time += lag;

		if(!counter)
		{
			printf("FPS: %f\n", 30.0f / time);
			time = 0.0f;
		}

		++counter;
		counter %= 30;
	}

	FB_ClearScreen();
	FB_ReleasePixelBuffer();
	FB_SwitchTTYToTextMode();

	return 0;
}
