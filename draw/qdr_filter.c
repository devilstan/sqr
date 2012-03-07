#include "qdr_filter.h"

//=================================================================================
// c2p
//=================================================================================
static int c2p(cairo_surface_t *surface, PKLImage pk)
{
	if(!surface)
		return 1;
	if(!pk)
		return 1;
	
	//CAIRO_FORMAT_RGB24とCAIRO_FORMAT_ARGB32以外は対応しない
	//但し、piklワールドはRGBなので、cairoワールドのAには不可侵
	//typedef enum _cairo_format {
	//    CAIRO_FORMAT_INVALID   = -1,
	//    CAIRO_FORMAT_ARGB32    = 0,
	//    CAIRO_FORMAT_RGB24     = 1,
	//    CAIRO_FORMAT_A8        = 2,
	//    CAIRO_FORMAT_A1        = 3,
	//    CAIRO_FORMAT_RGB16_565 = 4
	//} cairo_format_t;
	if(cairo_image_surface_get_format(surface) != CAIRO_FORMAT_RGB24 &&
	   cairo_image_surface_get_format(surface) != CAIRO_FORMAT_ARGB32) return 1;
	
	unsigned char *image = cairo_image_surface_get_data(surface);
	int stride = cairo_image_surface_get_stride(surface);

	pk->width  = cairo_image_surface_get_width(surface);
	pk->height = cairo_image_surface_get_height(surface);
	pk->channel = 3;
	pk->color = PKL_RGB;
	pk->image = (unsigned char *)malloc(pk->width * pk->height * pk->channel);
	
	int i, j;
	for(i=0; i<pk->height; i++){
		for(j=0; j<pk->width; j++)
			memcpy(&pk->image[(i*pk->width+j)*pk->channel], &image[i*stride+j*4], pk->channel);
	}

	return 0;
}

//=================================================================================
// p2c
//=================================================================================
static int p2c(PKLImage pk, cairo_surface_t *surface)
{
	if(!pk)
		return 1;
	if(!surface)
		return 1;
	
	//cairoワールドも戻さずに、piklワールドで保存できる
	//pkl_save_file(&pk, PKL_FORMAT_PNG, "test_p.png");
	
	unsigned char *image = cairo_image_surface_get_data(surface);
	int stride = cairo_image_surface_get_stride(surface);
	
	int i, j;
	for(i=0; i<pk->height; i++){
		for(j=0; j<pk->width; j++)
			memcpy(&image[i*stride+j*4], &pk->image[(i*pk->width+j)*pk->channel], pk->channel);
	}
	
	free(pk->image);
	pk->image=NULL;
	
	return 0;
}

//=================================================================================
// create_surface
//=================================================================================
static int create_surface(Qdr *qdr)
{
	if(qdr->surface){
		if(cairo_surface_get_type(qdr->surface) == CAIRO_SURFACE_TYPE_IMAGE){
			return 0;
		}else{
			fprintf(stderr, "[warn] %s(%d): invalid type\n", __func__, __LINE__);
			return 1;
		}
	}
	
	//サーフィスが存在しない場合は、Imageで作成してdraw
	qdr->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, qdr->rsize, qdr->rsize);
	qdr_draw(qdr);
	
	return 0;
}

//=================================================================================
// qdr_filter_invert
//=================================================================================
int qdr_filter_invert(Qdr *qdr)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_invert(&pk);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_crayon
//=================================================================================
int qdr_filter_crayon(Qdr *qdr, double factor, int bv1, int bv2, int alpha)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_crayon(&pk, factor, bv1, bv2, alpha);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_circle
//=================================================================================
int qdr_filter_circle(Qdr *qdr, int radius)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_circle(&pk, radius, 0);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_grid
//=================================================================================
int qdr_filter_grid(Qdr *qdr, int ms, int color)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_grid(&pk, ms, ms, color);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_voronoi
//=================================================================================
int qdr_filter_voronoi(Qdr *qdr, int zone)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_voronoi_zone(&pk, zone, NULL, 0);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_dots
//=================================================================================
int qdr_filter_dots(Qdr *qdr, int zone)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_dots(&pk, zone, NULL, NULL);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_hexagon
//=================================================================================
int qdr_filter_hexagon(Qdr *qdr, int s)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_pattern(&pk, s, s*3, PKL_PATTERN_HEXAGON, PKL_PAINT_AVE);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_diamond
//=================================================================================
int qdr_filter_diamond(Qdr *qdr, int s)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_pattern(&pk, s, s*2, PKL_PATTERN_DIAMOND, PKL_PAINT_AVE);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_tile
//=================================================================================
int qdr_filter_tile(Qdr *qdr, int s)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_tile(&pk, PKL_TILE_RECT, s, s, 30);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_noise
//=================================================================================
int qdr_filter_noise(Qdr *qdr, int n)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_noise(&pk, n);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_hydrangea
//=================================================================================
int qdr_filter_hydrangea(Qdr *qdr, int s)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_hydrangea(&pk, s, 9.0, 9.0, 1.0, 50, 70);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_wave
//=================================================================================
int qdr_filter_wave(Qdr *qdr, double factor, double frequency)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_wave(&pk, PKL_WAVE_HORZ, factor, frequency, NULL, PKL_SAMPLE_BL);
	
	p2c(&pk, qdr->surface);
	return 0;
}

//=================================================================================
// qdr_filter_ball
//=================================================================================
int qdr_filter_ball(Qdr *qdr)
{
	struct _PKLImage pk;
	
	if(create_surface(qdr))
		return 1;
	if(c2p(qdr->surface, &pk))
		return 1;
	
	pkl_ball(&pk, NULL, PKL_SAMPLE_BL);
	
	p2c(&pk, qdr->surface);
	return 0;
}

