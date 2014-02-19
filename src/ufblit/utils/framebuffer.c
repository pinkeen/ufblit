#include "framebuffer.h"

#include <unistd.h>
#include <fcntl.h>

#include <sys/kd.h>
#include <sys/vt.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <linux/fb.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fbfd = -1;
static void *pixels = NULL;
static struct fb_var_screeninfo screenInfo;
static size_t size;

static int graphicsMode = 0;
static char ttydev[256];

int FB_GetBitsPerPixel()
{
	if(fbfd == -1)
		return 0;

	return screenInfo.bits_per_pixel;
}

int FB_GetWidth()
{
	if(fbfd == -1)
		return 0;

	return screenInfo.xres;
}

int FB_GetHeight()
{
	if(fbfd == -1)
		return 0;

	return screenInfo.yres;
}

void *FB_AcquirePixelBuffer(const char *fbdev)
{
	if(fbfd != -1)
	{
		fprintf(stderr, "[FB] Cannot acquire pixel buffer - a framebuffer is already open\n");
		return NULL;
	}

	fbfd = open(fbdev, O_RDWR);

	if(fbfd == -1)
	{
		perror("[FB] Could not open framebuffer");
		return NULL;
	}

	if(ioctl(fbfd, FBIOGET_VSCREENINFO, &screenInfo) == -1)
	{
		perror("Could not get screen info");
		close(fbfd);
		fbfd = -1;
		return NULL;
	}

	size = screenInfo.xres * screenInfo.yres * (screenInfo.bits_per_pixel / 8);

	pixels = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if(!pixels)
	{
		perror("Could not mmap framebuffer");
		close(fbfd);
		fbfd = -1;
		return NULL;
	}

	return pixels;
}

void FB_ReleasePixelBuffer()
{
	if(fbfd == -1)
	{
		fprintf(stderr, "[FB] Cannot release pixel buffer - framebuffer isn't open\n");
		return;
	}

	munmap(pixels, size);
	close(fbfd);

	fbfd = -1;
	pixels = NULL;
}

int FB_SwitchTTYToGraphicsMode(int ttynum)
{
	if(graphicsMode)
	{
		fprintf(stderr, "[FB] Could not switch to graphics mode - already in it\n");
		return 0;
	}

	snprintf(ttydev, sizeof(ttydev), "/dev/tty%d", ttynum);

	int fda = open(ttydev, O_WRONLY);

	if(fda == -1)
	{
		perror("[FB] Could not open tty");
		return 0;
	}


	if(ioctl(fda, VT_ACTIVATE, ttynum) == -1)
	{
		perror("[FB] Could not activate vt");
		close(fda);
		return 0;
	}

	if(ioctl(fda, VT_WAITACTIVE, ttynum) == -1)
	{
		perror("[FB] Could wait for vt to become active? WTF?");
		close(fda);
		return 0;
	}

	if(ioctl(fda, KDSETMODE, KD_GRAPHICS) == -1)
	{
		perror("[FB] Could not set graphics mode");
		close(fda);
		return 0;
	}

	close(fda);

	graphicsMode = 1;

	return 1;
}

int FB_SwitchTTYToTextMode()
{
	if(!graphicsMode)
	{
		fprintf(stderr, "[FB] Could not switch to text mode - already in it\n");
		return 0;
	}

	int fd = open(ttydev, O_WRONLY);

	if(fd == -1)
	{
		perror("[FB] Could not open tty dev");
		return 0;
	}

	if(ioctl(fd, KDSETMODE, KD_TEXT) == -1)
	{
		perror("Could not set text mode");
		close(fd);
		return 0;
	}

	close(fd);

	graphicsMode = 0;

	return 1;
}

void FB_ClearScreen()
{
	if(!pixels)
		return;

	memset(pixels, 0, size);
}
