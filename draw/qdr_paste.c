#include "qdr_paste.h"

//=================================================================================
// paste_paint
//=================================================================================
void paste_paint(Qdr *qdr, cairo_t *cr)
{
	int x, y, sw, sh;
	struct QDRBindImage b;
	
	if(!qdr->paste.image)
		return;
	
	cairo_save(cr);
	
	//cairo_surface_t *surface = cairo_image_surface_create_from_png(qdr->paste.image);
	cairo_surface_t *surface = image_surface_create(&b, qdr->paste.image);
	if(!surface)
		return;
	cairo_pattern_t *pattern = cairo_pattern_create_for_surface(surface);
	
	int w = cairo_image_surface_get_width(surface);
	int h = cairo_image_surface_get_height(surface);
	
	if(qdr->paste.type == QDR_PASTETYPE_LAYOUT){
		//誤り訂正レベルに応じた許容サイズ(0.8=微妙に読めないことがあるので補正)
		int lm = sqrt(qdr->ssize*qdr->msize * qdr->ssize*qdr->msize * qdr->paste.level/100) * 0.8;
		//エリア
		int s = (qdr->ssize * qdr->msize) / 3;
		
		//マークの範囲を優先する
		if(lm > s)
			lm = s;
		
		//マージン
		int m = qdr->margin * qdr->msize;
		
		switch(qdr->paste.layout){
			case QDR_LAYOUT_NORTH:	//上部
				x = s + m;
				y = m;
				break;
			case QDR_LAYOUT_SOUTH:	//下部
				x = s + m;
				y = s*2 + m;
				break;
			case QDR_LAYOUT_EAST:	//右
				x = s*2 + m;
				y = s + m;
				break;
			case QDR_LAYOUT_WEST:	//左
				x = m;
				y = s + m;
				break;
			case QDR_LAYOUT_CENTER:	//中央
			default:
				x = s + m;
				y = s + m;
		}
		
		//scaling
		if(w>lm || h>lm){
			int big = w>h ? w : h;
			double scaling = (double)lm/big;
			
			w *= scaling;
			h *= scaling;
			
			cairo_matrix_t matrix;
			cairo_matrix_init_scale(&matrix, 1/scaling, 1/scaling);
			cairo_pattern_set_matrix(pattern, &matrix);
		}
		
		//極力中心へ
		x += (s - w)/2;
		y += (s - h)/2;
		sw = sh = s;
	}else{
		x = qdr->paste.x;
		y = qdr->paste.y;
		sw = w;
		sh = h;
	}
	
	cairo_translate(cr, x, y);
	cairo_set_source(cr, pattern);
	cairo_rectangle(cr, 0, 0, sw, sh);
	cairo_fill(cr);
	
	cairo_pattern_destroy(pattern);
	cairo_surface_destroy(surface);
	if(b.data)
		free(b.data);
	
	cairo_restore(cr);
}

//=================================================================================
// qdr_paste_layout
//=================================================================================
int qdr_paste_layout(Qdr *qdr, const char *file, QDR_LAYOUT layout, QDR_LEVEL level)
{
	int lm;
	
	if(!qdr)
		return 1;
	if(level<=0 || level>30)
		return 1;
	
	//reset
	if(!file){
		qdr->paste.image  = NULL;
		qdr->paste.layout = QDR_LAYOUT_UNSET;
		qdr->paste.level  = QDR_LEVEL_N;
		return 0;
	}
	
	//誤り訂正レベルに応じた許容サイズ(5pxより小さいのはどうせみえないので除外)
	lm = sqrt(qdr->ssize*qdr->msize * qdr->ssize*qdr->msize * level/100) * 0.8;
	if(lm < 5)
		return 1;
	
	//エリア
	//s = (qdr->ssize * qdr->msize) / 3;
	//if(lm > s){
	//	fprintf(stderr, "[warn] %s(%d) level invalid.\n", __func__, __LINE__);
	//	return 1;
	//}
	
	qdr->paste.type = QDR_PASTETYPE_LAYOUT;
	qdr->paste.image = file;
	qdr->paste.layout = layout;
	qdr->paste.level = level;
	
	return 0;
}

//=================================================================================
// qdr_paste
//=================================================================================
int qdr_paste(Qdr *qdr, const char *file, int x, int y)
{
	if(!qdr)
		return 1;
	
	qdr->paste.type = QDR_PASTETYPE_POSITION;
	qdr->paste.image  = file;
	qdr->paste.x = x;
	qdr->paste.y = y;
	
	return 0;
}

