//gcc layout.c -I/usr/include/cairo -lcairo
#include <cairo.h>

#define SIZE 300

int main()
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SIZE, SIZE);
	cairo_t *cr = cairo_create(surface);
	
	cairo_set_source_rgba(cr, 0.8, 1, 0.9, 1);
	cairo_paint(cr);
	
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 18.0);


	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	
	//•¶Žš•`‰æ
	{
		cairo_text_extents_t extents;
		//typedef struct {
		//    double x_bearing;
		//    double y_bearing;
		//    double width;
		//    double height;
		//    double x_advance;
		//    double y_advance;
		//} cairo_text_extents_t;
		
		cairo_text_extents(cr, "NORTH", &extents);
		cairo_move_to(cr, 100.0 + (100-extents.width)/2, 50.0+10);
		cairo_show_text(cr, "NORTH");
	
		cairo_text_extents(cr, "CENTER", &extents);
		cairo_move_to(cr, 100.0 + (100-extents.width)/2, 150.0+10);
		cairo_show_text(cr, "CENTER");
		
		cairo_text_extents(cr, "SOUTH", &extents);
		cairo_move_to(cr, 100.0 + (100-extents.width)/2, 250.0+10);
		cairo_show_text(cr, "SOUTH");
		
		cairo_text_extents(cr, "WEST", &extents);
		cairo_move_to(cr, 0.0 + (100-extents.width)/2, 150.0+10);
		cairo_show_text(cr, "WEST");
		
		cairo_text_extents(cr, "EAST", &extents);
		cairo_move_to(cr, 200.0 + (100-extents.width)/2, 150.0+10);
		cairo_show_text(cr, "EAST");
	}
	
	cairo_stroke(cr);
	

	//ƒ‰ƒCƒ“
	cairo_set_source_rgba(cr, 0, 0, 1, 1);
	
	double dashes[] = {10.0, 5.0};
	cairo_set_dash(cr, dashes, 2, 3.0);
	cairo_set_line_width(cr, 2.0);
	
	cairo_move_to(cr,   0, 100);
	cairo_line_to(cr, 300, 100);
	
	cairo_move_to(cr,   0, 200);
	cairo_line_to(cr, 300, 200);
	
	cairo_move_to(cr, 100,   0);
	cairo_line_to(cr, 100, 300);
	
	cairo_move_to(cr, 200,   0);
	cairo_line_to(cr, 200, 300);
	
	cairo_stroke(cr);
	
	
	cairo_surface_write_to_png(surface, "layout.png");
	

	return(0);
}


