//gcc image2.c -I/usr/include/cairo -lcairo -lgif
#include <cairo.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gif_lib.h>


#define SIZE 300


int main()
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SIZE, SIZE);
	cairo_t *cr = cairo_create(surface);
	
	cairo_surface_t *image;
	
	cairo_set_source_rgba(cr, 0.8, 1, 0.9, 1);
	cairo_paint(cr);
	
	
//////////////////////////////////////////////////////////////////////////////////
	{
		unsigned char *data;
		
		GifFileType *gif;
		SavedImage g;
		ColorMapObject *colormap;
		unsigned char trans[256];
		int i, j, t;
		
		gif = DGifOpenFileName("trnsN.gif");
		if(!gif)
			return 1;
		if(DGifSlurp(gif) == GIF_ERROR)
			return(1);
		
		//アニメーションGIFの場合もあるが、1枚目の画像のみ使う
		g = gif->SavedImages[0];
		if(g.ImageDesc.Left + g.ImageDesc.Width  > gif->SWidth || g.ImageDesc.Top  + g.ImageDesc.Height > gif->SHeight){
			DGifCloseFile(gif);
			return 1;
		}

		//colormap
		colormap = g.ImageDesc.ColorMap;
		if(!colormap)
			colormap = gif->SColorMap;
		
		//透過
		int trns = 0;
		{
			ExtensionBlock *block;
			
			memset(trans, 0, 256);
			
			if(g.ExtensionBlockCount!=0 && g.ExtensionBlocks){
				for(i=0; i<g.ExtensionBlockCount; i++){
					block = &(g.ExtensionBlocks[i]);
					
					if(block->Function == GRAPHICS_EXT_FUNC_CODE){
						if(block->Bytes[0] & 0x01){
							trans[(int)block->Bytes[3]] = 1;
							trns = 1;
						}
					}
				}
			}
		}
		
		
		
		data = malloc(g.ImageDesc.Height * g.ImageDesc.Width * 4);
		
		
		//RGB -> BGRA
		if(g.ImageDesc.Interlace){
			int ifset[4] = { 0, 4, 2, 1 };	//InterlacedOffset
			int jumps[4] = { 8, 8, 4, 2 };	//InterlacedJumps
			
			//for(t=0; t<4; t++){
			//	for(i=ifset[t]; i<g.ImageDesc.Height; i+=jumps[t],j++)
			//		memcpy(&ls->image[scanline*i], &g.RasterBits[j*ls->width], ls->width);
			//}
		}else{
			//マニュアルを見ると、アルファは全色に適用しなければいけないとのことなので
			//考慮が必要！
			for(i=0; i<g.ImageDesc.Height; i++){
				for(j=0; j<g.ImageDesc.Width; j++){
					GifColorType *ColorMapEntry = &colormap->Colors[g.RasterBits[i*g.ImageDesc.Width+j]];
					data[(i*g.ImageDesc.Height+j) * 4   ] = ColorMapEntry->Blue;
					data[(i*g.ImageDesc.Height+j) * 4 +1] = ColorMapEntry->Green;
					data[(i*g.ImageDesc.Height+j) * 4 +2] = ColorMapEntry->Red;
					data[(i*g.ImageDesc.Height+j) * 4 +3] = trans[g.RasterBits[i*g.ImageDesc.Width+j]] ? 0 : 0xff;

				}
			}
		}
		
		DGifCloseFile(gif);
		
		image = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_ARGB32, 
										g.ImageDesc.Width, g.ImageDesc.Height, g.ImageDesc.Width * 4);
		
		//マニュアルにある通り、destroyするまでは解放してはいけないらしい
		//free(data);
	}
//////////////////////////////////////////////////////////////////////////////////


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
	
	
	cairo_surface_write_to_png(surface, "image2.png");
	
	cairo_surface_destroy(image);
	cairo_surface_destroy(surface);
	
	return(0);
}

