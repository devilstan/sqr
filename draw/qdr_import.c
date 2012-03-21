#include "qdr_import.h"

//=================================================================================
// bind_jpg
//=================================================================================
static void bind_jpg(struct QDRBindImage *b, const char *file)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	unsigned char *p;
	FILE *fp;
	
	fp = fopen(file, "r");
	if(!fp)
		return;
	
	//エラーハンドルはデフォルトに任せる
	cinfo.err = jpeg_std_error(&jerr);
	
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fp);

	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	
	if(cinfo.out_color_space!=JCS_GRAYSCALE && cinfo.out_color_space!=JCS_RGB && cinfo.out_color_space!=JCS_CMYK)
		return;
	
	b->width = cinfo.output_width;
	b->height = cinfo.output_height;
	b->data = malloc(b->height * b->width * 4);
	memset(b->data, 0xff, b->height * b->width * 4);

	//1行分
	p = malloc(cinfo.output_width * cinfo.output_components);
	
	int i=0, j;
	int channel = cinfo.out_color_space==JCS_RGB ? 3 : 4;
	
	//JPEG -> CAIRO_FORMAT_ARGB32
	//	GRAY:  G    -> GGGA
	//	RGB:   RGB  -> BGRA
	//	CMYK:  CMYK -> YMCA
	while(cinfo.output_scanline < cinfo.output_height){
		jpeg_read_scanlines(&cinfo, &p, 1);
		
		//grayscale
		if(cinfo.out_color_space==JCS_GRAYSCALE){
			for(j=0; j<b->width; j++)
				memset(&b->data[(i*b->width+j)*4], p[j], 3);
		}else{
			//rgb or cmyk
			for(j=0; j<b->width; j++){
				b->data[(i*b->width+j)*4  ] = p[j*channel + 2];
				b->data[(i*b->width+j)*4+1] = p[j*channel + 1];
				b->data[(i*b->width+j)*4+2] = p[j*channel];
				//b->data[(i*b->width+j)*4+3] = 0xff;
			}
		}
		i++;
	}
	free(p);
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(fp);
}

//=================================================================================
// bind_gif
//=================================================================================
static void bind_gif(struct QDRBindImage *b, const char *file)
{
	GifFileType *gif;
	SavedImage g;
	ColorMapObject *colormap;
	unsigned char trans[256];
	int i, j, s, t;

	gif = DGifOpenFileName(file);
	if(!gif)
		return;
	if(DGifSlurp(gif) == GIF_ERROR)
		return;

	//アニメーションGIFの場合でも、1枚目の画像のみ使う
	g = gif->SavedImages[0];
	if(g.ImageDesc.Left + g.ImageDesc.Width  > gif->SWidth || g.ImageDesc.Top  + g.ImageDesc.Height > gif->SHeight){
		DGifCloseFile(gif);
		return;
	}
	
	//カラーマップ
	colormap = g.ImageDesc.ColorMap;
	if(!colormap)
		colormap = gif->SColorMap;
	
	//透過情報の取得
	memset(trans, 0, 256);
	if(g.ExtensionBlockCount!=0 && g.ExtensionBlocks){
		ExtensionBlock *block;
		
		for(i=0; i<g.ExtensionBlockCount; i++){
			block = &(g.ExtensionBlocks[i]);
			
			if(block->Function == GRAPHICS_EXT_FUNC_CODE){
				if(block->Bytes[0] & 0x01)
					trans[(int)block->Bytes[3]] = 1;
			}
		}
	}
	
	//イメージを格納する領域
	b->width = g.ImageDesc.Width;
	b->height = g.ImageDesc.Height;
	b->data = malloc(b->height * b->width * 4);
	
	if(g.ImageDesc.Interlace){
		int ifset[4] = { 0, 4, 2, 1 };	//InterlacedOffset
		int jumps[4] = { 8, 8, 4, 2 };	//InterlacedJumps
		i=0;
		
		for(t=0; t<4; t++){
			for(s=ifset[t]; s<b->height; s+=jumps[t]){
				
				for(j=0; j<b->width; j++){
					if(trans[g.RasterBits[i*b->width+j]]){
						memset(&b->data[(s*b->width+j)*4], 0, 4);
					}else{
						//RGB -> BGRA
						GifColorType *ColorMapEntry = &colormap->Colors[g.RasterBits[i*b->width+j]];
						b->data[(s*b->width+j)*4  ] = ColorMapEntry->Blue;
						b->data[(s*b->width+j)*4+1] = ColorMapEntry->Green;
						b->data[(s*b->width+j)*4+2] = ColorMapEntry->Red;
						b->data[(s*b->width+j)*4+3] = 0xff;
					}
				}
				i++;
			}
		}
	}else{
		for(i=0; i<b->height; i++){
			for(j=0; j<b->width; j++){
				if(trans[g.RasterBits[i*b->width+j]]){
					memset(&b->data[(i*b->width+j)*4], 0, 4);
				}else{
					//RGB -> BGRA
					GifColorType *ColorMapEntry = &colormap->Colors[g.RasterBits[i*b->width+j]];
					b->data[(i*b->width+j)*4  ] = ColorMapEntry->Blue;
					b->data[(i*b->width+j)*4+1] = ColorMapEntry->Green;
					b->data[(i*b->width+j)*4+2] = ColorMapEntry->Red;
					b->data[(i*b->width+j)*4+3] = 0xff;
				}
			}
		}
	}
	
	DGifCloseFile(gif);
}

//=================================================================================
// image_surface_create
//=================================================================================
cairo_surface_t *image_surface_create(struct QDRBindImage *b, const char *file)
{
	cairo_surface_t *image=NULL;
	
	if(!file)
		goto LAST;
	
	if(!b)
		goto LAST;
	memset(b, 0, sizeof(struct QDRBindImage));

	image = cairo_image_surface_create_from_png(file);
	if(cairo_surface_status(image) == CAIRO_STATUS_SUCCESS)
		goto LAST;

	{
		int i;
		unsigned char d[8];
		struct {
			void (*binder)(struct QDRBindImage *b, const char *file);
			int size;
			unsigned char magic[8];
		} format[] = {
			{bind_jpg, 2, {0xFF, 0xD8}},
			{bind_gif, 6, {0x47, 0x49, 0x46, 0x38, 0x37, 0x61}},
			{bind_gif, 6, {0x47, 0x49, 0x46, 0x38, 0x39, 0x61}},
			//{NULL,     8, {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}},
		};

		FILE *fp = fopen(file, "r");
		if(!fp)
			goto LAST;
		
		fread(d, 1, 8, fp);
		fclose(fp);
		
		for(i=0; i<sizeof(format)/sizeof(format[0]); i++){
			if(!memcmp(d, format[i].magic, format[i].size)){
				format[i].binder(b, file);
				break;
			}
		}
	}
	
	if(b->data)
		image = cairo_image_surface_create_for_data(b->data, CAIRO_FORMAT_ARGB32, b->width, b->height, b->width*4);

LAST:
	return image;
}
