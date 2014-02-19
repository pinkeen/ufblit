#include "ufb_common.h"

#include <string.h>

void UFB_ExtractFileExt(const char *fname, char *ext, UFBsize bufSz)
{
	char *dot = strrchr(fname, '.');

	if(dot == NULL)
	{
		ext[0] = '\0';
		return;
	}

	if(*(dot + 1) == '\0')
	{
		ext[0] = '\0';
		return;
	}

	char *tmp = dot + 1;
	int len = strlen(tmp);

	strncpy(ext, tmp, bufSz);

	if(len < (bufSz - 1))
	{
		ext[len] = '\0';
		return;
	}

	ext[bufSz - 1] = '\0';
}

UFB_ImageType UFB_GuessImageTypeFromExt(const char *fname)
{
	char ext[50];

	UFB_ExtractFileExt(fname, ext, sizeof(ext));

	if(!strcmp(ext, "png"))
		return UFB_IT_PNG;

	if(!strcmp(ext, "tga"))
		return UFB_IT_TGA;

	if(!strcmp(ext, "jpg") || !strcmp(ext, "jpeg"))
		return UFB_IT_JPEG;

	return UFB_IT_UNKNOWN;
}
