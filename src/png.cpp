#include <stdio.h>
#include <string.h>
#include <setjmp.h>

#include <new>

#include "lodepng.h"

unsigned char *png_load(const char *path, int *width, int *height, int *channels)
{
	unsigned char *image;
	unsigned char *file;
	size_t outsize;

	LodePNGState state;

	lodepng_state_init(&state);

	if (!lodepng_load_file(&file, &outsize, path))
	{
		if (!lodepng_decode(&image, (unsigned int *) width, (unsigned int *) height, &state, file, outsize))
			*channels = (int) lodepng_get_channels(&state.info_raw);
	}

	free(file);
	lodepng_state_cleanup(&state);

	return image;
}

bool png_save(const char *filename, int width, int height, unsigned char *data)
{
	return lodepng_encode32_file(filename, data, (unsigned int) width, (unsigned int) height) == 0;
}
