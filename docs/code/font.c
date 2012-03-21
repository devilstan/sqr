//gcc font.c -I/usr/include/cairo -lcairo
#include <cairo.h>

#define SIZE 300

int main()
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SIZE, SIZE);
	cairo_t *cr = cairo_create(surface);
	
	cairo_set_source_rgba(cr, 0.8, 1, 0.9, 1);
	cairo_paint(cr);
	
	cairo_set_source_rgba(cr, 0, 0, 0, 1);
	
	
	//http://hp.vector.co.jp/authors/VA022006/css/fonts.html
	//CSS2で定義されているフォントファミリ総称に関しては描画が保証されているらしい
	//	Serif
	//	Sans-Serif(Sans)
	//	Cursive
	//	Fantasy
	//	Monospace
	
	cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 20.0);
	cairo_move_to(cr, 10.0, 20.0);
	cairo_show_text(cr, "Serif");
	
	
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 20.0);
	cairo_move_to(cr, 10.0, 50.0);
	cairo_show_text(cr, "Sans");
	
	
	cairo_select_font_face(cr, "Sans-Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 20.0);
	cairo_move_to(cr, 10.0, 80.0);
	cairo_show_text(cr, "Sans-Serif");
	
	
	cairo_select_font_face(cr, "Cursive", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 20.0);
	cairo_move_to(cr, 10.0, 110.0);
	cairo_show_text(cr, "Cursive");
	
	
	cairo_select_font_face(cr, "Fantasy", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 20.0);
	cairo_move_to(cr, 10.0, 140.0);
	cairo_show_text(cr, "Fantasy");
	
	
	cairo_select_font_face(cr, "Monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 20.0);
	cairo_move_to(cr, 10.0, 170.0);
	cairo_show_text(cr, "Monospace");
	
	
	
	//実際にはシステムにインストールされているフォントであれば
	//描画は保証されないが、出力はできるみたい
	cairo_set_source_rgba(cr, 0.9, 0.3, 0.3, 1);
	
	
//	cairo_select_font_face(cr, "Times", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
//	cairo_set_font_size(cr, 20.0);
//	cairo_move_to(cr, 10.0, 200.0);
//	cairo_show_text(cr, "Times");
//	
//	
//	cairo_select_font_face(cr, "Courier", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
//	cairo_set_font_size(cr, 20.0);
//	cairo_move_to(cr, 10.0, 230.0);
//	cairo_show_text(cr, "Courier");
	
	
	
	cairo_stroke(cr);
	cairo_surface_write_to_png(surface, "font.png");
	
	return(0);
}

