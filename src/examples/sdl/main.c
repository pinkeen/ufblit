#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <math.h>

#include <SDL/SDL.h>

#include <ufblit/ufb.h>

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
	int starCount = 100;

	if(argc == 2)
		starCount = atoi(argv[1]);

	srandom(1000);

	printf("***UFBTEST***\n");

	SDL_Surface *screen;

	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Something went wrong when trying to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	atexit(SDL_Quit);

	if((screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE)) == NULL)
	{
		printf("Could not set the desired video mode: %s\n", SDL_GetError());
		return 1;

	}


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

		memcpy(screen->pixels, screenImg->pixels, screenImg->size);

		SDL_Flip(screen);

		time += lag;

		if(!counter)
		{
			printf("FPS: %f\n", 30.0f / time);
			time = 0.0f;
		}

		++counter;
		counter %= 30;
	}

	return 0;
}
