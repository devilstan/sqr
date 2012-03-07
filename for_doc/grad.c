//gcc font.c -I/usr/include/cairo -lcairo
#include <cairo.h>
#include <math.h>

#define SIZE 300

int main()
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SIZE, SIZE);
	cairo_t *cr = cairo_create(surface);
	
	cairo_set_source_rgba(cr, 0.8, 1, 0.9, 1);
	cairo_paint(cr);
	
	
	
	cairo_pattern_t *pat;


//cairo_pattern_t *   cairo_pattern_create_radial         (double cx0,
//                                                         double cy0,
//                                                         double radius0,
//                                                         double cx1,
//                                                         double cy1,
//                                                         double radius1);
	pat = cairo_pattern_create_radial(115.2, 102.4, 3.0, 102.4,  102.4, 5.0);
	
	
	cairo_pattern_add_color_stop_rgba (pat, 0, 0.8, 0.8, 0.8, 1);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
	cairo_set_source (cr, pat);
	
	
	cairo_arc (cr, 128.0, 128.0, 10, 0, 2 * M_PI);
	cairo_fill (cr);
	
	
	
	cairo_pattern_destroy(pat);
	
	
	
	cairo_surface_write_to_png(surface, "grad.png");
	
	return(0);
}

