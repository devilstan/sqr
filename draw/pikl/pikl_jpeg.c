#include "pikl_jpeg.h"

static int color_p2j(int color);
static int color_j2p(int color);
static int compress_p2j(int level);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if JPEG_LIB_VERSION < 80
//=================================================================================
// init_mem_source
//=================================================================================
static void init_mem_source(j_decompress_ptr cinfo)
{
}

//=================================================================================
// fill_mem_input_buffer
//=================================================================================
static boolean fill_mem_input_buffer(j_decompress_ptr cinfo)
{
	static JOCTET mybuffer[4];

	WARNMS(cinfo, JWRN_JPEG_EOF);
	mybuffer[0] = (JOCTET) 0xFF;
	mybuffer[1] = (JOCTET) JPEG_EOI;

	cinfo->src->next_input_byte = mybuffer;
	cinfo->src->bytes_in_buffer = 2;
	return TRUE;
}

//=================================================================================
// skip_input_data
//=================================================================================
static void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
	struct jpeg_source_mgr * src = cinfo->src;

	if(num_bytes > 0){
		while(num_bytes > (long) src->bytes_in_buffer){
			num_bytes -= (long) src->bytes_in_buffer;
			(void) (*src->fill_input_buffer) (cinfo);
		}
		src->next_input_byte += (size_t) num_bytes;
		src->bytes_in_buffer -= (size_t) num_bytes;
	}
}

//=================================================================================
// term_source
//=================================================================================
static void term_source(j_decompress_ptr cinfo)
{
}

//=================================================================================
// jpeg_mem_src
//=================================================================================
static void jpeg_mem_src(j_decompress_ptr cinfo, unsigned char * inbuffer, unsigned long insize)
{
	struct jpeg_source_mgr * src;

	if(inbuffer == NULL || insize == 0)
		ERREXIT(cinfo, JERR_INPUT_EMPTY);

	if(cinfo->src == NULL)
		cinfo->src = (struct jpeg_source_mgr *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(struct jpeg_source_mgr));

	src = cinfo->src;
	src->init_source = init_mem_source;
	src->fill_input_buffer = fill_mem_input_buffer;
	src->skip_input_data = skip_input_data;
	src->resync_to_restart = jpeg_resync_to_restart;
	src->term_source = term_source;
	src->bytes_in_buffer = (size_t) insize;
	src->next_input_byte = (JOCTET *) inbuffer;
}

//=================================================================================
// init_mem_destination
//=================================================================================
static void init_mem_destination(j_compress_ptr cinfo)
{
}

//=================================================================================
// empty_mem_output_buffer
//=================================================================================
static boolean empty_mem_output_buffer(j_compress_ptr cinfo)
{
	size_t nextsize;
	JOCTET * nextbuffer;
	my_mem_dest_ptr dest = (my_mem_dest_ptr)cinfo->dest;

	nextsize = dest->bufsize * 2;
	nextbuffer = malloc(nextsize);
	if(nextbuffer == NULL)
		ERREXIT1(cinfo, JERR_OUT_OF_MEMORY, 10);

	memcpy(nextbuffer, dest->buffer, dest->bufsize);
	if(dest->newbuffer != NULL)
		free(dest->newbuffer);

	dest->newbuffer = nextbuffer;
	dest->pub.next_output_byte = nextbuffer + dest->bufsize;
	dest->pub.free_in_buffer = dest->bufsize;
	dest->buffer = nextbuffer;
	dest->bufsize = nextsize;
	return TRUE;
}

//=================================================================================
// term_mem_destination
//=================================================================================
static void term_mem_destination(j_compress_ptr cinfo)
{
	my_mem_dest_ptr dest = (my_mem_dest_ptr)cinfo->dest;

	*dest->outbuffer = dest->buffer;
	*dest->outsize = dest->bufsize - dest->pub.free_in_buffer;
}

//=================================================================================
// jpeg_mem_dest
//=================================================================================
static void jpeg_mem_dest(j_compress_ptr cinfo, unsigned char ** outbuffer, unsigned long * outsize)
{
	my_mem_dest_ptr dest;

	if(outbuffer == NULL || outsize == NULL)
		ERREXIT(cinfo, JERR_BUFFER_SIZE);

	if(cinfo->dest == NULL)
		cinfo->dest = (struct jpeg_destination_mgr *)(*cinfo->mem->alloc_small)((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_mem_destination_mgr));

	dest = (my_mem_dest_ptr)cinfo->dest;
	dest->pub.init_destination = init_mem_destination;
	dest->pub.empty_output_buffer = empty_mem_output_buffer;
	dest->pub.term_destination = term_mem_destination;
	dest->outbuffer = outbuffer;
	dest->outsize = outsize;
	dest->newbuffer = NULL;

	if(*outbuffer == NULL || *outsize == 0){
		dest->newbuffer = *outbuffer = malloc(OUTPUT_BUF_SIZE);
		if (dest->newbuffer == NULL)
			ERREXIT1(cinfo, JERR_OUT_OF_MEMORY, 10);
		*outsize = OUTPUT_BUF_SIZE;
	}
	dest->pub.next_output_byte = dest->buffer = *outbuffer;
	dest->pub.free_in_buffer = dest->bufsize = *outsize;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//=============================================================================
// pkljpeg_error_exit
//=============================================================================
static void pkljpeg_error_exit(j_common_ptr cinfo)
{
	struct err_mgr *jp_err = (struct err_mgr *)cinfo->err;
	longjmp(jp_err->setjmp_buffer, 1);
}

//=================================================================================
// load_jpg
//=================================================================================
static int load_jpg(PKLImage pkl, FILE *image, unsigned char *data, unsigned long length)
{
	struct jpeg_decompress_struct cinfo;
	struct err_mgr jerr;
	unsigned char *p;
	
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = pkljpeg_error_exit;
	
	if( setjmp(jerr.setjmp_buffer) ){
		jpeg_destroy_decompress(&cinfo);
		return(1);
	}

	jpeg_create_decompress(&cinfo);
	if(image){
		//ファイルディスクリプタから読み込み
		jpeg_stdio_src(&cinfo, image);
	}else{
		//メモリから読み込み
		jpeg_mem_src(&cinfo, data, length);
	}
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	
	pkl->width   = cinfo.output_width;
	pkl->height  = cinfo.output_height;
	pkl->color   = color_j2p(cinfo.out_color_space);
	pkl->channel = cinfo.output_components;
	if(pkl->color == PKL_UNKNOWN) return(1);
	
	pkl->image = malloc(cinfo.output_height * cinfo.output_width * cinfo.output_components);
	if(!pkl->image) return(1);
	
	p = pkl->image;
	while(cinfo.output_scanline < cinfo.output_height){
		jpeg_read_scanlines(&cinfo, &p, 1);
		p += (cinfo.output_width * cinfo.output_components);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	return(0);
}

//=============================================================================
// load_jpg_fd
//=============================================================================
int load_jpg_fd(PKLImage pkl, FILE *image)
{
	if(!image)
		return(1);
	
	return load_jpg(pkl, image, NULL, 0);
}

//=================================================================================
// load_jpg_data
//=================================================================================
int load_jpg_data(PKLImage pkl, unsigned char *image, unsigned long length)
{
	if(!image || length<=0)
		return(1);
	
	return load_jpg(pkl, NULL, image, length);
}

//=================================================================================
// save_jpg
//=================================================================================
static int save_jpg(PKLImage pkl, FILE *image, unsigned char **data, unsigned long *length)
{
	struct jpeg_compress_struct oinfo;
	struct err_mgr jerr;
	JSAMPROW row_pointer[1];
	int i;
	
	if(pkl->color==PKL_UNKNOWN)
		return(1);
	
	oinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = pkljpeg_error_exit;
	
	if( setjmp(jerr.setjmp_buffer) ){
		jpeg_destroy_compress(&oinfo);
		return(1);
	}

	jpeg_create_compress(&oinfo);
	if(image){
		//ファイルディスクリプタに書き込み
		jpeg_stdio_dest(&oinfo, image);
	}else{
		//メモリに書き込み
		jpeg_mem_dest(&oinfo, data, length);
	}
	
	oinfo.image_width      = pkl->width;
	oinfo.image_height     = pkl->height;
	oinfo.in_color_space   = color_p2j(pkl->color);
	oinfo.input_components = pkl->channel;
	jpeg_set_defaults(&oinfo);
	jpeg_set_quality(&oinfo, compress_p2j(pkl->compress), TRUE);
	oinfo.write_JFIF_header= TRUE;
	oinfo.density_unit     = 1;
	jpeg_start_compress(&oinfo, TRUE);
	
	for(i=0; i<pkl->height ; i++){
		row_pointer[0] = &pkl->image[i*pkl->width*pkl->channel];
		jpeg_write_scanlines(&oinfo, row_pointer, 1);
	}
	
	jpeg_finish_compress(&oinfo);
	jpeg_destroy_compress(&oinfo);
	return(0);
}

//=================================================================================
// save_jpg_fd
//=================================================================================
int save_jpg_fd(PKLImage pkl, FILE *image)
{
	if(!image)
		return(1);
	
	return save_jpg(pkl, image, NULL, 0);
}

//=================================================================================
// save_jpg_data
//=================================================================================
unsigned char *save_jpg_data(PKLImage pkl, unsigned long *length)
{
	unsigned char *image=NULL;
	unsigned long image_len=0;
	
	if(!length)
		return NULL;
	if(save_jpg(pkl, NULL, &image, &image_len))
		return NULL;
	
	*length=image_len;
	return(image);
}

//=============================================================================
// color_p2j
//=============================================================================
static int color_p2j(int color)
{
	switch(color){
		case PKL_GRAYSCALE:
			return JCS_GRAYSCALE;
		case PKL_RGB:
			return JCS_RGB;
		//case PKL_YCbCr:
		//	return JCS_YCbCr;
		case PKL_CMYK:
			return JCS_CMYK;
		//case PKL_YCCK:
		//	return JCS_YCCK;
		//case PKL_RGBA:
		default:
			return JCS_UNKNOWN;
	}
	return JCS_UNKNOWN;
}

//=============================================================================
// color_j2p
//=============================================================================
static int color_j2p(int color)
{
	switch(color){
		case JCS_GRAYSCALE:
			return PKL_GRAYSCALE;
		case JCS_RGB:
			return PKL_RGB;
		//case JCS_YCbCr:
		//	return PKL_CMYK;
		case JCS_CMYK:
			return PKL_CMYK;
		//case JCS_YCCK:
		//	return PKL_YCCK;
		default:
			return PKL_UNKNOWN;
	}
	return PKL_UNKNOWN;
}

//=============================================================================
// compress_p2j
//=============================================================================
static int compress_p2j(int level)
{
	int comp;
	
	if(level < 0) return(90);
	comp = 100 - (level*10);
	if(comp <= 0) return(1);
	return(comp);
}

