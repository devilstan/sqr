#include "qdr_test.h"

//=================================================================================
// qdr_test
//=================================================================================
int qdr_test(Qdr *qdr)
{
#ifdef DEBUG
	const char *utf8 = "sns.creyle.jp";
	int height = 40;
	unsigned char r = 0x00,
				  g = 0xf0,
				  b = 0xff,
				  a = 0xff;
	cairo_t *cr;
	
	//ひとまずdraw
	qdr->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, qdr->rsize, qdr->rsize);
	qdr_draw(qdr);
	
	cr = cairo_create(qdr->surface);
	
	
	cairo_select_font_face(cr, "Fantasy", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, height);
	
	
	cairo_set_source_rgba(cr, (double)r/255, (double)g/255, (double)b/255, (double)a/255);
	
	
	cairo_text_extents_t extents;
	cairo_text_extents(cr, utf8, &extents);
	
	//オペレータは要検討
	//  http://cairographics.org/operators/
	cairo_set_operator(cr, CAIRO_OPERATOR_HARD_LIGHT);
	//cairo_set_operator(cr, CAIRO_OPERATOR_DARKEN);
	
	int x = (qdr->rsize - extents.width) / 2;
	int y = (qdr->rsize + height) / 2;
	
	cairo_move_to(cr, x, y);
	cairo_show_text(cr, utf8);
	
	cairo_destroy(cr);

	cairo_surface_write_to_png(qdr->surface, "test.png");
#endif
	return 0;
}
