//gcc cairo.c -I/usr/include/cairo -lcairo

#include <cairo.h>
#include <cairo-svg.h>
#include <math.h>

int main()
{
	//cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 500, 500);
	//cairo_t *cr = cairo_create(surface);
	//
	//cairo_set_antialias(cr, CAIRO_ANTIALIAS_SUBPIXEL);
	//
	////背景色
	//cairo_set_source_rgb(cr, 1, 1, 1);
	//cairo_paint(cr);
	//
	//cairo_set_source_rgb(cr, 0, 0, 1);
	//cairo_set_line_width(cr, 1);
	//
	//cairo_move_to(cr, 150, 260);
	//cairo_line_to(cr, 300, 250);
	//cairo_stroke(cr);
	//
	//
	//cairo_move_to(cr, 100, 200);
	//cairo_line_to(cr, 190, 100);
	//cairo_stroke(cr);
	//
	//
	//cairo_surface_write_to_png(surface, "out2.png");


cairo_surface_t *surface = cairo_svg_surface_create("test.svg", 500, 500);
	//cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 500, 500);
	cairo_t *cr = cairo_create(surface);
	
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	
//////////////////////////////////////////
	cairo_move_to(cr, 100, 0);
	cairo_line_to(cr, 100, 500);
	cairo_move_to(cr, 200, 0);
	cairo_line_to(cr, 200, 500);
	cairo_move_to(cr, 300, 0);
	cairo_line_to(cr, 300, 500);
	cairo_move_to(cr, 400, 0);
	cairo_line_to(cr, 400, 500);
	
	cairo_move_to(cr, 0, 100);
	cairo_line_to(cr, 500, 100);
	cairo_move_to(cr, 0, 200);
	cairo_line_to(cr, 500, 200);
	cairo_move_to(cr, 0, 300);
	cairo_line_to(cr, 500, 300);
	cairo_move_to(cr, 0, 400);
	cairo_line_to(cr, 500, 400);
	
	cairo_set_line_width(cr, 1.0);
	cairo_set_source_rgb (cr, 1, 0, 0);
	cairo_stroke(cr);
//////////////////////////////////////////
	
	
	cairo_move_to(cr, 100, 100);
	cairo_line_to(cr, 325, 100);
	cairo_arc_negative(cr, 350, 150, 50, 270 *(M_PI/180.0), 180 *(M_PI/180.0));
	cairo_arc(cr, 250, 150, 50, 0.0*(M_PI/180.0), 90*(M_PI/180.0));
	cairo_line_to(cr, 150, 200);
	cairo_arc(cr, 150, 150, 50, 90.0*(M_PI/180.0), 180*(M_PI/180.0));
	cairo_line_to(cr, 100, 100);
//cairo_close_path (cr);

//cairo_set_line_width(cr, 5.0);
//cairo_set_source_rgb (cr, 0, 0, 1);

cairo_set_source_rgb(cr, 0, 1, 1);
//cairo_fill_preserve(cr);	//これは現在のパスを残す
cairo_fill(cr);				//これは現在のパスをコンテキストから削除する
//cairo_stroke(cr);

	
//////////////////////////////////////////
	cairo_move_to(cr, 100, 300);
	cairo_line_to(cr, 325, 300);
	cairo_arc_negative(cr, 350, 350, 50, 270 *(M_PI/180.0), 180 *(M_PI/180.0));
	
double x, y;
cairo_get_current_point(cr, &x, &y);
printf("%f %f\n", x, y);

	
	cairo_arc(cr, 250, 350, 50, 0.0*(M_PI/180.0), 90*(M_PI/180.0));
	cairo_line_to(cr, 150, 400);
	cairo_arc(cr, 150, 350, 50, 90.0*(M_PI/180.0), 180*(M_PI/180.0));
	cairo_line_to(cr, 100, 300);
//cairo_close_path (cr);

//cairo_set_line_width(cr, 5.0);
//cairo_set_source_rgb (cr, 0, 0, 1);

cairo_set_source_rgb(cr, 0, 1, 1);
//cairo_fill_preserve(cr);	//これは現在のパスを残す
cairo_fill(cr);				//これは現在のパスをコンテキストから削除する
//cairo_stroke(cr);


cairo_surface_finish(surface);	//svg output
	//cairo_surface_write_to_png(surface, "out2.png");
	

	return(0);
}


