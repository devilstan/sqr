#ifndef __QDR_IMPORT_IMAGE__
#define __QDR_IMPORT_IMAGE__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <gif_lib.h>
#include "qdr_private.h"

struct QDRBindImage {
	unsigned char *data;
	unsigned int width;
	unsigned int height;
};

cairo_surface_t *image_surface_create(struct QDRBindImage *b, const char *file);

#endif
