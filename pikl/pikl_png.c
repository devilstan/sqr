#include "pikl_png.h"

static int color_pn2pk(int color);
static int color_pk2pn(int color);
static int pkl_png_channels(int color);
static int pkl_png_depth(int color);
static int compress_pk2pn(int level);

//=================================================================================
// png_read_fn
//=================================================================================
static void png_read_fn(png_structp png_ptr, png_bytep buf, png_size_t size)
{
	struct pkl_png_data *png = (struct pkl_png_data *)png_get_io_ptr(png_ptr);
	
	if(png->offset + size <= png->length){
		memcpy(buf, png->data + png->offset, size);
		png->offset += size;
	}else{
		;;	//error
	}
}

//=============================================================================
// load_png
//=============================================================================
static int load_png(PKLImage pkl, FILE *image, unsigned char *data, unsigned long length)
{
	png_structp png_ptr;
	png_infop   info_ptr;
	int i, color_type, num_trans=0;
	unsigned char bit_depth;
	struct pkl_png_data pkpng;
	
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr) return(1);
	
	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return(1);
	}

	if( setjmp(png_jmpbuf(png_ptr)) ){
		png_destroy_info_struct(png_ptr, &info_ptr);
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return(1);
	}

	//データの入力元に応じて、srcを振り分け
	if(image){
		png_init_io(png_ptr, image);
	}else{
		pkpng.data = data;
		pkpng.length = length;
		pkpng.offset = 0;
		png_set_read_fn(png_ptr, (png_voidp)&pkpng, (png_rw_ptr)png_read_fn);
	}
	png_read_info(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	png_get_tRNS(png_ptr, info_ptr, NULL, &num_trans, NULL);

	pkl->width   = png_get_image_width(png_ptr, info_ptr);
	pkl->height  = png_get_image_height(png_ptr, info_ptr);
	pkl->color   = color_pn2pk(color_type);
	pkl->channel = pkl_png_channels(pkl->color);
	if(pkl->color == PKL_UNKNOWN) return(1);

	pkl->image = malloc(pkl->width * pkl->height * pkl->channel);
	if(!pkl->image) return(1);

	if(color_type==PNG_COLOR_TYPE_RGB_ALPHA  || color_type==PNG_COLOR_TYPE_GRAY_ALPHA || num_trans)
		png_set_strip_alpha(png_ptr);
#if PNG_LIBPNG_VER >= 10401
	if(bit_depth<8 && color_type==PNG_COLOR_TYPE_GRAY)
		png_set_expand_gray_1_2_4_to_8(png_ptr);
#else
	if(bit_depth<8 && color_type==PNG_COLOR_TYPE_GRAY)
		png_set_gray_1_2_4_to_8(png_ptr);
#endif
	if(bit_depth==16)
		png_set_strip_16(png_ptr);
	if(color_type==PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	for(i=0; i<pkl->height; i++)
		png_read_row(png_ptr, &pkl->image[i*pkl->width*pkl->channel], NULL);
	
	png_destroy_info_struct(png_ptr, &info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	
	return(0);
}

//=================================================================================
// load_png_fd
//=================================================================================
int load_png_fd(PKLImage pkl, FILE *image)
{
	if(!image)
		return(1);
	return load_png(pkl, image, NULL, 0);
}

//=============================================================================
// load_png
//=============================================================================
int load_png_data(PKLImage pkl, unsigned char *image, unsigned long length)
{
	if(!image || length<=0)
		return(1);
	return load_png(pkl, NULL, image, length);
}

//=================================================================================
// png_write_fn
//=================================================================================
static void png_write_fn(png_structp png_ptr, png_bytep buf, png_size_t size)
{
	struct pkl_png_data *png = (struct pkl_png_data *)png_get_io_ptr(png_ptr);
	unsigned long length;
	
	if(png->offset + size > png->length){
		length = 2 * png->length;
		if(png->offset + size > length)
			length = png->offset + size;
		
		png->data = realloc(png->data, length);
		png->length = length;
	}
	memcpy(png->data + png->offset, buf, size);
	png->offset += size;
}

//===================================================================
// save_png
//===================================================================
static int save_png(PKLImage pkl, FILE *image, struct pkl_png_data *pkpng)
{
	png_structp png_ptr;
	png_infop   info_ptr;
	int  i;
	
	if(pkl->color!=PKL_RGB &&
	   pkl->color!=PKL_GRAYSCALE) return(1);
	
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr) return(1);
	
	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_write_struct(&png_ptr,  (png_infopp)NULL);
		return(1);
	}
	
	if( setjmp(png_jmpbuf(png_ptr)) ){
		png_destroy_info_struct(png_ptr, &info_ptr);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return(1);
	}
	
	if(image){
		png_init_io(png_ptr, image);
	}else{
		png_set_write_fn(png_ptr, (png_voidp)pkpng, (png_rw_ptr)png_write_fn, NULL);
	}
	png_set_IHDR(png_ptr, info_ptr,
		pkl->width,
		pkl->height,
		pkl_png_depth(pkl->color),
		color_pk2pn(pkl->color),
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);
	png_set_compression_level(png_ptr, compress_pk2pn(pkl->compress));
	png_write_info(png_ptr, info_ptr);
	
	for(i=0; i<pkl->height; i++)
		png_write_row(png_ptr, &pkl->image[i*pkl->width*pkl->channel]);
	png_write_end(png_ptr, info_ptr);
	png_destroy_info_struct(png_ptr, &info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);

	return(0);
}

//=================================================================================
// save_png_fd
//=================================================================================
int save_png_fd(PKLImage pkl, FILE *image)
{
	if(!image)
		return 1;
	
	return save_png(pkl, image, NULL);
}

//=================================================================================
// save_png_data
//=================================================================================
unsigned char *save_png_data(PKLImage pkl, unsigned long *length)
{
	struct pkl_png_data pkpng;
	
	if(!length)
		return NULL;
	
	*length = 0;
	memset(&pkpng, 0, sizeof(struct pkl_png_data));
	if(save_png(pkl, NULL, &pkpng))
		return NULL;
	
	*length = pkpng.offset;
	return(pkpng.data);
}

//=============================================================================
// color_pn2pk
//=============================================================================
static int color_pn2pk(int color)
{
	switch(color){
		case PNG_COLOR_TYPE_GRAY:
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			return(PKL_GRAYSCALE);
		case PNG_COLOR_TYPE_PALETTE:
		case PNG_COLOR_TYPE_RGB:
		case PNG_COLOR_TYPE_RGB_ALPHA:
			return(PKL_RGB);
		default:
			return(PKL_UNKNOWN);
	}
	return(PKL_UNKNOWN);
}

//=============================================================================
// pkl_png_channels
//=============================================================================
static int pkl_png_channels(int color)
{
	switch(color){
		//case PKL_BLACKWHITE:
		//	return ;
		case PKL_GRAYSCALE:
			return 1;
		case PKL_RGB:
			return 3;
		//case PKL_RGBA:
		//case PKL_CMYK:
		//	return 4;
		default:
			return 0;
	}
	return 0;
}

//=============================================================================
// pkl_png_depth
//=============================================================================
static int pkl_png_depth(int color)
{
	switch(color){
		//case PKL_BLACKWHITE:
		//	return 1;
		case PKL_GRAYSCALE:
		case PKL_RGB:
		//case PKL_CMYK:
		//case PKL_RGBA:
			return 8;
		default:
			return 0;
	}
	return 0;
}

//=============================================================================
// color_pk2pn
//=============================================================================
static int color_pk2pn(int color)
{
	switch(color){
		//case PKL_BLACKWHITE:
		//	return PNG_COLOR_TYPE_GRAY;
		case PKL_GRAYSCALE:
			return PNG_COLOR_TYPE_GRAY;
		case PKL_RGB:
			return PNG_COLOR_TYPE_RGB;
		//case PKL_CMYK:
		//case PKL_RGBA:
		default:
			return -1;
	}
	return -1;
}

//=============================================================================
// compress_pk2pn
//=============================================================================
static int compress_pk2pn(int level)
{
	if(level < 0) return(Z_BEST_COMPRESSION);
	if(level > 9) return(9);
	return(level);
}
