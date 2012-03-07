#ifndef _PIKL_PNG_
#define _PIKL_PNG_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <png.h>

#include "pikl.h"
#include "pikl_private.h"

struct pkl_png_data {
	unsigned char *data;
	unsigned long length;
	unsigned long offset;
};

int load_png_fd(PKLImage pkl, FILE *image);
int load_png_data(PKLImage pkl, unsigned char *image, unsigned long length);

int save_png_fd(PKLImage pkl, FILE *image);
unsigned char *save_png_data(PKLImage pkl, unsigned long *length);

#endif

