#include "qdr_text.h"

//=================================================================================
// text_paint
//=================================================================================
void text_paint(Qdr *qdr, cairo_t *cr)
{
	cairo_text_extents_t extents;
	
	if(!qdr->text.utf8)
		return;
	
	cairo_save(cr);
	
	cairo_select_font_face(cr, qdr->text.font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, qdr->text.height);
	cairo_set_source_rgba(cr, qdr->text.r, qdr->text.g, qdr->text.b, qdr->text.a);
	
	cairo_text_extents(cr, qdr->text.utf8, &extents);
	
	//オペレータは要検討(1.10以下の場合は全てのオペレータが未対応)
	//  http://cairographics.org/operators/
	#if CAIRO_VERSION >= 11000
		cairo_set_operator(cr, CAIRO_OPERATOR_HARD_LIGHT);
		//cairo_set_operator(cr, CAIRO_OPERATOR_DARKEN);
	#endif
	
	int x = (qdr->rsize - extents.width) / 2;
	int y = (qdr->rsize + qdr->text.height) / 2;
	
	cairo_move_to(cr, x, y);
	cairo_show_text(cr, qdr->text.utf8);
	
	cairo_restore(cr);
}

//=================================================================================
// qdr_text
//=================================================================================
int qdr_text(Qdr *qdr, const char *font, const char *utf8, unsigned int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if(!qdr)
		return 1;
	if(height > (unsigned)qdr->msize*3 || height<=0)	//フォントがデカスギル!
		return 1;
	
	qdr->text.font = font ? font : "serif";
	qdr->text.utf8 = utf8;
	qdr->text.height = height;
	qdr->text.r = (double)r/255;
	qdr->text.g = (double)g/255;
	qdr->text.b = (double)b/255;
	qdr->text.a = (double)a/255;
	return 0;
}
