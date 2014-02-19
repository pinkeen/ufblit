#include <stdio.h>

#include <ufblit/ufb_image.h>
#include <ufblit/ufb_common.h>

int main()
{
	char ext[5];

	UFB_ExtractFileExt("test.image.jpegcostam", ext, sizeof(ext));

	UFB_Image *img = UFB_CreateImageFromFile("dupa.jpg");

	printf("Ext: %s\n", ext);

	return 0;
}
