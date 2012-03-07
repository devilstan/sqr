//gcc metaball.c -I/usr/include/cairo -lcairo

#include <cairo.h>
#include <math.h>

int main()
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 500, 500);
	cairo_t *cr = cairo_create(surface);
	
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	
	//////////////////////////////////////////
	// 目安線
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
	
	cairo_set_line_width(cr, 0.5);
	cairo_set_source_rgba(cr, 1, 0, 0, 1);
	cairo_stroke(cr);
	//////////////////////////////////////////
	
	
	
////////////////////////////////////////////////////////////////////////////////////////////////
	// 左下(D)
	cairo_move_to(cr, 100, 50);
	cairo_arc(cr, 150, 50, 50, 180*(M_PI/180.0), 270*(M_PI/180.0));
	cairo_arc(cr, 150, 50, 50, 270*(M_PI/180.0), 360*(M_PI/180.0));
	cairo_line_to(cr, 200, 100);
	cairo_line_to(cr, 150, 100);
	
	cairo_arc_negative(cr, 150-sqrt(3)*50, 100, 50, 0*(M_PI/180.0), 330*(M_PI/180.0));
	cairo_arc(cr, 150, 50, 50, 150*(M_PI/180.0), 180*(M_PI/180.0));
	
	cairo_set_source_rgb(cr, 0, 1, 1);
	cairo_fill(cr);				//これは現在のパスをコンテキストから削除する
	
	
	
	// 左上(A)
	cairo_move_to(cr, 100, 150);
	cairo_arc(cr, 150, 150, 50, 180*(M_PI/180.0), 210*(M_PI/180.0));
	
	cairo_arc_negative(cr, 150-sqrt(3)*50, 100, 50, 30*(M_PI/180.0), 0*(M_PI/180.0));

	cairo_line_to(cr, 200, 100);
	cairo_line_to(cr, 200, 150);
	cairo_arc(cr, 150, 150, 50,  0*(M_PI/180.0),  90*(M_PI/180.0));
	cairo_arc(cr, 150, 150, 50, 90*(M_PI/180.0), 180*(M_PI/180.0));
	
	cairo_set_source_rgb(cr, 0, 1, 1);
	cairo_fill(cr);				//これは現在のパスをコンテキストから削除する


////////////////////////////////////////////////////////////////////////////////////////////////

	// 左上(A)
	cairo_move_to(cr, 300, 150);
	cairo_arc_negative(cr, 300, 150-sqrt(3)*50, 50, 90*(M_PI/180.0), 60*(M_PI/180.0));
	cairo_arc(cr, 350, 150, 50, 240*(M_PI/180.0), 270*(M_PI/180.0));

	cairo_arc(cr, 350, 150, 50, 270*(M_PI/180.0), 360*(M_PI/180.0));
	cairo_arc(cr, 350, 150, 50, 0*(M_PI/180.0), 90*(M_PI/180.0));

	cairo_line_to(cr, 300, 200);
	
	cairo_set_source_rgb(cr, 0, 1, 1);
	cairo_fill(cr);				//これは現在のパスをコンテキストから削除する


	// 右上(B)
	cairo_move_to(cr, 250, 100);
	//cairo_arc_negative(cr, 300, 100, 150-sqrt(3)*50+50-100, 180*(M_PI/180.0), 90*(M_PI/180.0));
	cairo_arc_negative(cr, 300, 100, (2-sqrt(3))*50, 180*(M_PI/180.0), 90*(M_PI/180.0));

	cairo_line_to(cr, 300, 200);
	cairo_line_to(cr, 200, 200);

	cairo_set_source_rgb(cr, 0, 1, 1);
	cairo_fill(cr);				//これは現在のパスをコンテキストから削除する


	// 左下(D)
	cairo_move_to(cr, 150, 0);
	cairo_arc(cr, 250, 50, 50, 270*(M_PI/180.0), 360*(M_PI/180.0));


	cairo_arc(cr, 250, 50, 50, 0*(M_PI/180.0), 30*(M_PI/180.0));
	cairo_arc_negative(cr, 250+sqrt(3)*50, 100, 50, 210*(M_PI/180.0), 180*(M_PI/180.0));

	cairo_line_to(cr, 250, 100);
	cairo_line_to(cr, 200, 100);

	cairo_set_source_rgb(cr, 0, 1, 1);
	cairo_fill(cr);				//これは現在のパスをコンテキストから削除する
	
	




//
//	
////////////////////////////////////////////
//	cairo_move_to(cr, 100, 300);
//	cairo_line_to(cr, 325, 300);
//	cairo_arc_negative(cr, 350, 350, 50, 270 *(M_PI/180.0), 180 *(M_PI/180.0));
//	
//double x, y;
//cairo_get_current_point(cr, &x, &y);
//printf("%f %f\n", x, y);
//
//	
//	cairo_arc(cr, 250, 350, 50, 0.0*(M_PI/180.0), 90*(M_PI/180.0));
//	cairo_line_to(cr, 150, 400);
//	cairo_arc(cr, 150, 350, 50, 90.0*(M_PI/180.0), 180*(M_PI/180.0));
//	cairo_line_to(cr, 100, 300);
////cairo_close_path (cr);
//
////cairo_set_line_width(cr, 5.0);
////cairo_set_source_rgb (cr, 0, 0, 1);
//
//cairo_set_source_rgb(cr, 0, 1, 1);
////cairo_fill_preserve(cr);	//これは現在のパスを残す
//cairo_fill(cr);				//これは現在のパスをコンテキストから削除する
////cairo_stroke(cr);


	cairo_surface_write_to_png(surface, "metaball.png");
	

	return(0);
}


