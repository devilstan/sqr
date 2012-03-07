#include "pikl_io.h"

//=============================================================================
// pkl_canvas
//=============================================================================
PKLExport PKLImage pkl_canvas(int width, int height, PKL_COLOR_SPACE color, PKLColor backcolor)
{
	PKLImage pkl;
	PKL_COLOR_SPACE colorspace;
	int channel, i;
	unsigned char back[PKL_CHANNEL];
	
	switch(color){
		case PKL_GRAYSCALE:
			colorspace = PKL_GRAYSCALE;
			channel = 1;
			break;
		case PKL_RGB:
			colorspace = PKL_RGB;
			channel = 3;
			break;
		case PKL_CMYK:
			colorspace = PKL_CMYK;
			channel = 4;
			break;
		default:
			return(NULL);
	}
	
	pkl = malloc( sizeof(struct _PKLImage) );
	if(!pkl) return(NULL);
	memset(pkl, 0, sizeof(struct _PKLImage));
	
	pkl->image = malloc(width*height*channel);
	if(!pkl->image){
		free(pkl);
		return(NULL);
	}
	
	pkl->width = width;
	pkl->height = height;
	pkl->color = colorspace;
	pkl->channel = channel;
	pkl->format = PKL_FORMAT_ERROR;
	pkl->compress = -1;
	
	//for(i=0; i<pkl->channel; i++)
	//	back[i] = (backcolor>>((pkl->channel-i)*8) & 0xFF);
	if(backcolor)
		memcpy(back, backcolor->color, PKL_CHANNEL);
	else
		memset(back, 0xff, PKL_CHANNEL);
	
	for(i=0; i<pkl->width*pkl->height*pkl->channel; i+=pkl->channel)
		memcpy(&pkl->image[i], back, pkl->channel);
	return(pkl);
}

//=============================================================================
// pkl_close
//=============================================================================
PKLExport void pkl_close(PKLImage pkl)
{
	if(!pkl) return;
	if(pkl->image) free(pkl->image);
	free(pkl);
}

//=============================================================================
// pkl_dup
//=============================================================================
PKLExport PKLImage pkl_dup(PKLImage pkl)
{
	PKLImage dst;
	
	if(!pkl) return(NULL);
	
	dst = malloc(sizeof(struct _PKLImage));
	if(!dst) return(NULL);
	memset(dst, 0, sizeof(struct _PKLImage));
	
	dst->width = pkl->width;
	dst->height = pkl->height;
	dst->color = pkl->color;
	dst->channel = pkl->channel;
	dst->format = pkl->format;
	dst->compress = pkl->compress;
	
	if(pkl->image){
		dst->image = malloc(pkl->width*pkl->height*pkl->channel);
		if(!dst->image){
			free(dst);
			return(NULL);
		}
		memcpy(dst->image, pkl->image, pkl->width*pkl->height*pkl->channel);
	}
	return(dst);
}

//=============================================================================
// pkl_format
//=============================================================================
PKLExport PKL_FORMAT pkl_format(PKLImage pkl)
{
	return(pkl->format);
}

//=================================================================================
// pkl_width
//=================================================================================
PKLExport int pkl_width(PKLImage pkl)
{
	return(pkl->width);
}

//=================================================================================
// pkl_height
//=================================================================================
PKLExport int pkl_height(PKLImage pkl)
{
	return(pkl->height);
}

//=============================================================================
// pkl_colorspace
//=============================================================================
PKLExport PKL_COLOR_SPACE pkl_colorspace(PKLImage pkl)
{
	return(pkl->color);
}

//=============================================================================
// pkl_compress
//=============================================================================
PKLExport int pkl_compress(PKLImage pkl, int level)
{
	if(level < 0 || level > 10) return(pkl->compress);
	pkl->compress = level;
	return(pkl->compress);
}

//=============================================================================
// pkl_count
//=============================================================================
PKLExport int pkl_count(PKLImage pkl)
{
	unsigned char *colors;
	int i, j, byte, bit, count=0, channel;
	union {
		int color;
		unsigned char row[PKL_CHANNEL];
	} pixel;
	
	switch(pkl->channel){
		case 1: channel=1; break;
		case 3:
		case 4: channel=3; break;
		default: return(0);
	}

	colors = malloc(256 * 256 * 32);
	memset(colors, 0, 256 * 256 * 32);

	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			pixel.color=0;
			memcpy(pixel.row, &pkl->image[(i*pkl->width+j)*pkl->channel], channel);
			byte = pixel.color / 8;
			bit = pixel.color  % 8;
			
			if((colors[byte]&(1<<bit))) continue;
			colors[byte] = colors[byte]|(1<<bit);
			count++;
		}
	}
	free(colors);
	return(count);
}

//=============================================================================
// pkl_open
//=============================================================================
PKL_DEPRECATED PKLExport PKLImage pkl_open(const char *in)
{
	return pkl_open_file(in);
}

//=================================================================================
// pkl_fdopen
//=================================================================================
PKL_DEPRECATED PKLExport PKLImage pkl_fdopen(FILE *in)
{
	return pkl_open_fd(in);
}

//=================================================================================
// pkl_save
//=================================================================================
PKL_DEPRECATED PKLExport int pkl_save(PKLImage pkl, const char *out, PKL_FORMAT format)
{
	return pkl_save_file(pkl, format, out);
}

//=================================================================================
// pkl_output
//=================================================================================
PKL_DEPRECATED PKLExport int pkl_output(PKLImage pkl, FILE *out, PKL_FORMAT format)
{
	return pkl_save_fd(pkl, format, out);
}

//=============================================================================
// pkl_create
//=============================================================================
static PKLImage pkl_create(unsigned char *data)
{
	PKLImage pkl;
	PKL_FORMAT format=PKL_FORMAT_ERROR;
	int count, i;
	
	count = sizeof(pklheader) /sizeof(struct PKL_HEADER);
	for(i=0; i<count; i++){
		if(!memcmp(data, pklheader[i].mark, pklheader[i].size)){
			format=pklheader[i].format;
			break;
		}
	}
	if(format==PKL_FORMAT_ERROR)
		return NULL;
	
	pkl = malloc(sizeof(struct _PKLImage));
	if(!pkl)
		return(NULL);
	memset(pkl, 0, sizeof(struct _PKLImage));
	pkl->compress = -1;
	pkl->format = format;
	return(pkl);
}

//=================================================================================
// pkl_open_file
//=================================================================================
PKLExport PKLImage pkl_open_file(const char *image_file)
{
	FILE *image;
	PKLImage pkl;
	
	image = fopen(image_file, "rb");
	if(!image)
		return NULL;
	
	pkl=pkl_open_fd(image);
	fclose(image);
	return(pkl);
}

//=================================================================================
// pkl_open_fd
//=================================================================================
PKLExport PKLImage pkl_open_fd(FILE *image_fd)
{
	PKLImage pkl;
	unsigned char header[PKL_HEADER_SIZE];
	int result;
	
	if(!image_fd)
		return NULL;

	if(fread(header, 1, PKL_HEADER_SIZE, image_fd) < PKL_HEADER_SIZE)
		return NULL;

	pkl=pkl_create(header);
	if(!pkl)
		return NULL;

	fseek(image_fd, 0, SEEK_SET);
	switch(pkl->format){
		case PKL_FORMAT_JPEG:
			result = load_jpg_fd(pkl, image_fd);
			break;
		case PKL_FORMAT_PNG:
			result = load_png_fd(pkl, image_fd);
			break;
		case PKL_FORMAT_BITMAP:
			result = load_bmp_fd(pkl, image_fd);
			break;
		default:
			result=1;
	}
	if(result){
		free(pkl);
		pkl=NULL;
	}
	return(pkl);
}

//=================================================================================
// pkl_open_mem
//=================================================================================
PKLExport PKLImage pkl_open_mem(unsigned char *image, unsigned long length)
{
	PKLImage pkl;
	int result;
	
	if(!image)
		return NULL;
	if(length < PKL_HEADER_SIZE)
		return NULL;
	
	pkl=pkl_create(image);
	if(!pkl)
		return NULL;
	
	switch(pkl->format){
		case PKL_FORMAT_JPEG:
			result = load_jpg_data(pkl, image, length);
			break;
		case PKL_FORMAT_PNG:
			result = load_png_data(pkl, image, length);
			break;
		case PKL_FORMAT_BITMAP:
			result = load_bmp_data(pkl, image, length);
			break;
		default:
			result=1;
	}
	if(result){
		free(pkl);
		pkl=NULL;
	}
	return(pkl);
}

//=================================================================================
// pkl_save_file
//=================================================================================
PKLExport int pkl_save_file(PKLImage pkl, PKL_FORMAT format, const char *image_file)
{
	FILE *image;
	int result;
	
	if(!pkl)
		return(1);
	if(!image_file)
		return(1);
	
	image = fopen(image_file, "wb");
	if(!image)
		return(1);
	
	result = pkl_save_fd(pkl, format, image);
	fclose(image);
	return(result);
}

//=================================================================================
// pkl_save_fd
//=================================================================================
PKLExport int pkl_save_fd(PKLImage pkl, PKL_FORMAT format, FILE *image_fd)
{
	switch(format){
		case PKL_FORMAT_JPEG:
			return save_jpg_fd(pkl, image_fd);
		case PKL_FORMAT_PNG:
			return save_png_fd(pkl, image_fd);
		case PKL_FORMAT_BITMAP:
			return save_bmp_fd(pkl, image_fd);
		default:
			;;
	}
	return(1);
}

//=================================================================================
// pkl_save_mem
//=================================================================================
PKLExport unsigned char *pkl_save_mem(PKLImage pkl, PKL_FORMAT format, unsigned long *length)
{
	unsigned char *image=NULL;
	unsigned long image_len=0;
	
	switch(format){
		case PKL_FORMAT_JPEG:
			image=save_jpg_data(pkl, &image_len);
			break;
		case PKL_FORMAT_PNG:
			image=save_png_data(pkl, &image_len);
			break;
		case PKL_FORMAT_BITMAP:
			return save_bmp_data(pkl, &image_len);
		default:
			;;
	}
	if(length)
		*length=image_len;
	return(image);
}
