#ifndef _PIKL_BITMAP_
#define _PIKL_BITMAP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pikl.h"
#include "pikl_private.h"

#define BF_SIZE 14		//BITMAPFILEHEADER size
#define BI_SIZE 40		//BITMAPINFOHEADER size

#define PKL_BMP_ALLOCATE 1024

struct pkl_bmp_data {
	unsigned char *data;
	unsigned long length;
	unsigned long offset;
};

int load_bmp_fd(PKLImage pkl, FILE *image);
int load_bmp_data(PKLImage pkl, unsigned char *image, unsigned long length);

int save_bmp_fd(PKLImage pkl, FILE *image);
unsigned char *save_bmp_data(PKLImage pkl, unsigned long *length);

#endif

