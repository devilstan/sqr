#ifndef _PIKL_JPEG_
#define _PIKL_JPEG_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <jpeglib.h>

#include "pikl.h"
#include "pikl_private.h"

/* libjpeg error handler */
struct err_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

int load_jpg_fd(PKLImage pkl, FILE *image);
int load_jpg_data(PKLImage pkl, unsigned char *image, unsigned long length);

int save_jpg_fd(PKLImage pkl, FILE *image);
unsigned char *save_jpg_data(PKLImage pkl, unsigned long *length);


#if JPEG_LIB_VERSION < 80
#include <jerror.h>

#define OUTPUT_BUF_SIZE  4096

typedef struct {
	struct jpeg_destination_mgr pub;
	unsigned char ** outbuffer;
	unsigned long * outsize;
	unsigned char * newbuffer;
	JOCTET * buffer;
	size_t bufsize;
} my_mem_destination_mgr;
typedef my_mem_destination_mgr * my_mem_dest_ptr;

typedef struct {
	struct jpeg_source_mgr pub;
	FILE * infile;
	JOCTET * buffer;
	boolean start_of_file;
} my_source_mgr;
typedef my_source_mgr * my_src_ptr;
#endif

#endif
