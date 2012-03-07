#include "qdr_paste.h"

//=================================================================================
// paste_paint
//=================================================================================
void paste_paint(Qdr *qdr, cairo_t *cr)
{
	int x, y, sw, sh;
	
	if(!qdr->paste.image)
		return;
	
	cairo_save(cr);
	
	cairo_surface_t *surface = cairo_image_surface_create_from_png(qdr->paste.image);
	cairo_pattern_t *pattern = cairo_pattern_create_for_surface(surface);
	
	int w = cairo_image_surface_get_width(surface);
	int h = cairo_image_surface_get_height(surface);
	
	if(qdr->paste.type == QDR_PASTETYPE_LAYOUT){
		//���������x���ɉ��������e�T�C�Y(0.8=�����ɓǂ߂Ȃ����Ƃ�����̂ŕ␳)
		int lm = sqrt(qdr->ssize*qdr->msize * qdr->ssize*qdr->msize * qdr->paste.level/100) * 0.8;
		//�G���A
		int s = (qdr->ssize * qdr->msize) / 3;
		
		//�}�[�N�͈̔͂�D�悷��
		if(lm > s)
			lm = s;
		
		//�}�[�W��
		int m = qdr->margin * qdr->msize;
		
		switch(qdr->paste.layout){
			case QDR_LAYOUT_NORTH:	//�㕔
				x = s + m;
				y = m;
				break;
			case QDR_LAYOUT_SOUTH:	//����
				x = s + m;
				y = s*2 + m;
				break;
			case QDR_LAYOUT_EAST:	//�E
				x = s*2 + m;
				y = s + m;
				break;
			case QDR_LAYOUT_WEST:	//��
				x = m;
				y = s + m;
				break;
			case QDR_LAYOUT_CENTER:	//����
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
		
		//�ɗ͒��S��
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
	
	//���������x���ɉ��������e�T�C�Y(5px��菬�����̂͂ǂ����݂��Ȃ��̂ŏ��O)
	lm = sqrt(qdr->ssize*qdr->msize * qdr->ssize*qdr->msize * level/100) * 0.8;
	if(lm < 5)
		return 1;
	
	//�G���A
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

