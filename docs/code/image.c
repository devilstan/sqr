//gcc image.c -I/usr/include/cairo -lcairo -ljpeg
#include <cairo.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jpeglib.h>

#define SIZE 300




int main()
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SIZE, SIZE);
	cairo_t *cr = cairo_create(surface);
	
	cairo_surface_t *image;
	
	cairo_set_source_rgba(cr, 0.8, 1, 0.9, 1);
	cairo_paint(cr);
	
	
	
	{
		unsigned char *data;
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
		unsigned char *p;
		
		FILE *x = fopen("Image867-00.jpg", "rb");
		
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		jpeg_stdio_src(&cinfo, x);

		jpeg_read_header(&cinfo, TRUE);
		jpeg_start_decompress(&cinfo);
		
		int width   = cinfo.output_width;
		int height  = cinfo.output_height;
		//いずれにしても4バイト領域を使うのでARGBにする！
		//		case JCS_GRAYSCALE:	//ARGB32
		//		case JCS_RGB:		//ARGB32	cairoの世界では実際はBGRAで並んでいる！?
		//		case JCS_CMYK:		//ARGB32
		
		data = malloc(cinfo.output_height * cinfo.output_width * 4);

		//1行分
		p = malloc(cinfo.output_width * 4);	//4は本当はいらないがーー＞components分だけでOK。
		
		int i=0, j;
		while(cinfo.output_scanline < cinfo.output_height){
			jpeg_read_scanlines(&cinfo, &p, 1);
			
			for(j=0; j<cinfo.output_width; j++){
				data[(i*cinfo.output_width+j) * 4]     = p[j*3 + 2];
				data[(i*cinfo.output_width+j) * 4 + 1] = p[j*3 + 1];
				data[(i*cinfo.output_width+j) * 4 + 2] = p[j*3];
				data[(i*cinfo.output_width+j) * 4 + 3] = 0xff;
			}
			i++;
		}
		free(p);
		
		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		
		fclose(x);
		
		
		
		image = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_ARGB32, 
										cinfo.output_width, cinfo.output_height, cinfo.output_width * 4);
		
		//マニュアルにある通り、destroyするまでは解放してはいけないらしい
		//free(data);
	}
	
	
//typedef enum _cairo_format {
//    CAIRO_FORMAT_INVALID   = -1,
//    CAIRO_FORMAT_ARGB32    = 0,
//    CAIRO_FORMAT_RGB24     = 1,
//    CAIRO_FORMAT_A8        = 2,
//    CAIRO_FORMAT_A1        = 3,
//    CAIRO_FORMAT_RGB16_565 = 4
//} cairo_format_t;
//cairo_image_surface_create_for_data(unsigned char *data, cairo_format_t format, int width, int height, int stride);



	cairo_set_source_surface(cr, image, 0, 0);
	cairo_paint_with_alpha(cr, 1);
	
	
	cairo_surface_write_to_png(surface, "image.png");
	
	cairo_surface_destroy(image);
	cairo_surface_destroy(surface);
	
	return(0);
}

