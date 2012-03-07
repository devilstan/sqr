#include "qdr_draw.h"

//=================================================================================
// qdr_set_msize
//=================================================================================
void qdr_set_msize(Qdr *qdr, unsigned int msize)
{
	if(!qdr)
		return;
	qdr->msize = msize;
	qdr->rsize = qdr_size(qdr->msize, qdr->ssize, qdr->margin);
}

//=================================================================================
// qdr_get_msize
//=================================================================================
unsigned int qdr_get_msize(Qdr *qdr)
{
	return qdr ? qdr->msize : 0;
}

//=================================================================================
// qdr_set_margin
//=================================================================================
void qdr_set_margin(Qdr *qdr, unsigned int margin)
{
	if(!qdr)
		return;
	qdr->margin = margin;
	qdr->rsize = qdr_size(qdr->msize, qdr->ssize, qdr->margin);
}

//=================================================================================
// qdr_get_margin
//=================================================================================
unsigned int qdr_get_margin(Qdr *qdr)
{
	return qdr ? qdr->margin : 0;
}

//=================================================================================
// qdr_init
//=================================================================================
int qdr_init(Qdr *qdr, unsigned int symbolsize, unsigned char data[QDR_MAXMSIZE][QDR_MAXMSIZE])
{
	if(!qdr)
		return 1;
	
	//if(qdr->surface && qdr->reuse)
	//	cairo_surface_destroy(qdr->surface);
	
	memset(qdr, 0, sizeof(Qdr));
	qdr->margin = QDR_DEFAULT_MARGIN;
	qdr->msize  = QDR_DEFAULT_MSIZE;
	qdr->ssize  = symbolsize;
	qdr->rsize  = qdr_size(qdr->msize, qdr->ssize, qdr->margin);
	memcpy(qdr->data, data, QDR_MAXMSIZE*QDR_MAXMSIZE);
	
	//�V���{���`��
	qdr->marktype = QDR_MARKTYPE_RECTANGLE;
	qdr->radius = QDR_DEFAULT_RADIUS;
	
	//�w�i
	qdr->background.type = QDR_BACKGROUND_SIMPLE;
	qdr->background.r = 1.0;
	qdr->background.g = 1.0;
	qdr->background.b = 1.0;
	qdr->background.a = 1.0;
	
	//�V���{���h����
	qdr->markpaint.type = QDR_MARKPAINT_SIMPLE;
	qdr->markpaint.r = 0.0;
	qdr->markpaint.g = 0.0;
	qdr->markpaint.b = 0.0;
	qdr->markpaint.a = 1.0;
	
	//eye_mark�Z�b�g�A�b�v
	eye_mark_init(qdr);
	
	return 0;
}

//=================================================================================
// qdr_close
//=================================================================================
void qdr_close(Qdr *qdr)
{
	if(qdr->surface){
		cairo_surface_destroy(qdr->surface);
		qdr->surface=NULL;
	}
}

//=================================================================================
// qdr_bg_color
//=================================================================================
void qdr_bg_color(Qdr *qdr, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if(!qdr)
		return;
	
	qdr->background.type = QDR_BACKGROUND_SIMPLE;
	qdr->background.r = (double)r/255;
	qdr->background.g = (double)g/255;
	qdr->background.b = (double)b/255;
	qdr->background.a = (double)a/255;
}

//=================================================================================
// qdr_bg_image
//=================================================================================
int qdr_bg_image(Qdr *qdr, const char *file, unsigned char alpha)
{
	if(!qdr)
		return 1;
	if(!file)
		return 1;
	
	qdr->background.type = QDR_BACKGROUND_IMAGE;
	qdr->background.image = file;
	qdr->background.a = (double)alpha/255;
	
	return 0;
}

//=================================================================================
// qdr_bg_grad
//=================================================================================
void qdr_bg_grad(Qdr *qdr, QDR_GRADIENT_TYPE type,
	unsigned char r0, unsigned char g0, unsigned char b0, unsigned char a0,
	unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1)
{
	if(!qdr)
		return;
	
	qdr->background.type = QDR_BACKGROUND_GRAD;
	qdr->background.grad.type = type;
	
	qdr->background.grad.r0 = (double)r0/255;
	qdr->background.grad.g0 = (double)g0/255;
	qdr->background.grad.b0 = (double)b0/255;
	qdr->background.grad.a0 = (double)a0/255;

	qdr->background.grad.r1 = (double)r1/255;
	qdr->background.grad.g1 = (double)g1/255;
	qdr->background.grad.b1 = (double)b1/255;
	qdr->background.grad.a1 = (double)a1/255;
}

//=================================================================================
// qdr_set_mark_color
//=================================================================================
void qdr_set_mark_color(Qdr *qdr, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if(!qdr)
		return;
	
	qdr->markpaint.type = QDR_MARKPAINT_SIMPLE;
	qdr->markpaint.r = (double)r/255;
	qdr->markpaint.g = (double)g/255;
	qdr->markpaint.b = (double)b/255;
	qdr->markpaint.a = (double)a/255;
}

//=================================================================================
// qdr_set_mark_grad
//=================================================================================
void qdr_set_mark_grad(Qdr *qdr, QDR_GRADIENT_TYPE type,
	unsigned char r0, unsigned char g0, unsigned char b0, unsigned char a0,
	unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1)
{
	if(!qdr)
		return;
	
	qdr->markpaint.type = QDR_MARKPAINT_GRAD;
	qdr->markpaint.grad.type = type;
	
	qdr->markpaint.grad.r0 = (double)r0/255;
	qdr->markpaint.grad.g0 = (double)g0/255;
	qdr->markpaint.grad.b0 = (double)b0/255;
	qdr->markpaint.grad.a0 = (double)a0/255;

	qdr->markpaint.grad.r1 = (double)r1/255;
	qdr->markpaint.grad.g1 = (double)g1/255;
	qdr->markpaint.grad.b1 = (double)b1/255;
	qdr->markpaint.grad.a1 = (double)a1/255;
}

//=================================================================================
// qdr_set_mark_image
//=================================================================================
void qdr_set_mark_image(Qdr *qdr, const char *file, unsigned char alpha)
{
	if(!qdr)
		return;
	
	qdr->markpaint.type = QDR_MARKPAINT_IMAGE;
	qdr->markpaint.image = file;
	qdr->markpaint.a = (double)alpha/255;
}

//=================================================================================
// grad_pattern
//=================================================================================
static cairo_pattern_t *grad_pattern(Qdr *qdr, struct QDRGrad *grad, int x, int y, int radius)
{
	cairo_pattern_t *pattern;
	int scope = radius * 4;

	switch(grad->type){
		case QDR_GRADIENT_T2B:
			pattern = cairo_pattern_create_linear(x, y, x, scope);
			break;
		case QDR_GRADIENT_TL2BR:
			pattern = cairo_pattern_create_linear(x, y, scope, scope);
			break;
		case QDR_GRADIENT_BL2TR:
			pattern = cairo_pattern_create_linear(x, scope, scope, y);
			break;
		case QDR_GRADIENT_RADIAL:
			pattern = cairo_pattern_create_radial(x+radius, y+radius, 0, x+radius, y+radius, scope);
			break;
		case QDR_GRADIENT_L2R:
		default:
			pattern = cairo_pattern_create_linear(x, y, scope, y);
	}
	
	cairo_pattern_add_color_stop_rgba(pattern, 0.0, grad->r0, grad->g0, grad->b0, grad->a0);
	cairo_pattern_add_color_stop_rgba(pattern, 1.0, grad->r1, grad->g1, grad->b1, grad->a1);

	return pattern;
}


//=================================================================================
// background
//=================================================================================
static void background(Qdr *qdr, cairo_t *cr)
{
	if(qdr->background.type == QDR_BACKGROUND_IMAGE){
		cairo_surface_t *image;
		cairo_matrix_t   matrix;
		int w, h, s;
		
		image = cairo_image_surface_create_from_png(qdr->background.image);
		if(!image){
			fprintf(stderr, "%s(%d) cairo_image_surface_create_from_png faild.\n", __func__, __LINE__);
			return;
		}
		
		w = cairo_image_surface_get_width(image);
		h = cairo_image_surface_get_height(image);
		
		//���Ԃ��ł��Ȃ��悤�ɃX�P�[�����O����
		cairo_save(cr);
			s = w<h ? w : h;
			cairo_matrix_init_scale(&matrix, (double)qdr->rsize/s, (double)qdr->rsize/s);
			cairo_set_matrix(cr, &matrix);

			cairo_set_source_surface(cr, image, 0, 0);
			cairo_paint_with_alpha(cr, qdr->background.a);
			
			cairo_surface_destroy(image);
		cairo_restore(cr);
	}else
	if(qdr->background.type == QDR_BACKGROUND_GRAD){
		cairo_pattern_t *pattern = grad_pattern(qdr, &qdr->background.grad, 0, 0, qdr->rsize/4);
		cairo_set_source(cr, pattern);
		cairo_paint(cr);
		cairo_pattern_destroy(pattern);
	}else{
		//QDR_BACKGROUND_SIMPLE
		cairo_set_source_rgba(cr, qdr->background.r, qdr->background.g, qdr->background.b, qdr->background.a);
		cairo_paint(cr);
	}
}

//=================================================================================
// mark_paint_set_source
//=================================================================================
static void mark_paint_set_source(Qdr *qdr, cairo_t *cr, cairo_pattern_t *pattern, cairo_surface_t *image)
{
	//�O���f�[�V����
	if(qdr->markpaint.type == QDR_MARKPAINT_GRAD){
		pattern = grad_pattern(qdr, &qdr->markpaint.grad, qdr->margin*qdr->msize, qdr->margin*qdr->msize, qdr->ssize*qdr->msize/2);
		cairo_set_source(cr, pattern);
		return;
	}
	
	//�摜
	if(qdr->markpaint.type == QDR_MARKPAINT_IMAGE){
		cairo_matrix_t matrix;
		int w, h, c, s;
		
		image = cairo_image_surface_create_from_png(qdr->markpaint.image);
		w = cairo_image_surface_get_width(image);
		h = cairo_image_surface_get_height(image);
		c = qdr->ssize * qdr->msize;
		
		//scaling
		s = w<h ? w : h;
		cairo_matrix_init_scale(&matrix, (double)c/s, (double)c/s);
		cairo_set_matrix(cr, &matrix);
		
		cairo_set_source_surface(cr, image, qdr->margin*qdr->msize*(double)s/c, qdr->margin*qdr->msize*(double)s/c);
		
		cairo_matrix_init_scale(&matrix, 1.0, 1.0);
		cairo_set_matrix(cr, &matrix);
		return;
	}
	
	if(qdr->markpaint.type == QDR_MARKPAINT_SIMPLE){
		cairo_set_source_rgba(cr, qdr->markpaint.r, qdr->markpaint.g, qdr->markpaint.b, qdr->markpaint.a);
		return;
	}
	
	if(qdr->markpaint.type == QDR_MARKPAINT_PALETTE){
		;;
	}
}

//=================================================================================
// qdr_draw
//=================================================================================
void qdr_draw(Qdr *qdr)
{
	cairo_t         *cr;
	cairo_pattern_t *pattern=NULL;
	cairo_surface_t *image=NULL;
	cairo_pattern_t *group_pattern=NULL;
	int i, j, is_shadow;
	void (*mark)(Qdr *qdr, cairo_t *cr, int x, int y);
	
	cr = cairo_create(qdr->surface);

	//�w�i�̃Z�b�g
	background(qdr, cr);
	
	//�O���[�o���}�[�N�y�C���g�ݒ�
	mark_paint_set_source(qdr, cr, pattern, image);
	
	//�}�[�N�`��ɉ����ă��\�b�h�I��
	switch(qdr->marktype){
		case QDR_MARKTYPE_ARC:
			mark = mark_arc;
			break;
		case QDR_MARKTYPE_BLOCKARC:
			mark = mark_blockarc;
			break;
		case QDR_MARKTYPE_METABALL:
			mark = mark_metaball;
			break;
		case QDR_MARKTYPE_RECTANGLE:
		default:
			mark = mark_rectangle;
	}
	
	//�e
	is_shadow = (qdr->shadow.offsetx!=0 || qdr->shadow.offsety!=0);
	
	//�}�[�N�`��
	for(i=0; i<qdr->ssize; i++){
		for(j=0; j<qdr->ssize; j++){
			if(!qdr->data[j][i])
				continue;
			
			if(is_shadow){
				cairo_save(cr);
				
				cairo_translate(cr, qdr->shadow.offsetx, qdr->shadow.offsety);
				cairo_set_source_rgba(cr, qdr->shadow.r, qdr->shadow.g, qdr->shadow.b, qdr->shadow.a);
				mark(qdr, cr, j, i);
				cairo_fill(cr);
				//cairo_translate(cr, -qdr->shadow.offsetx, -qdr->shadow.offsety);
				
				cairo_restore(cr);
			}
			
			mark(qdr, cr, j, i);
			
			//eye
			if(eye_check(qdr, j, i)){
				eye_paint(qdr, cr, j, i);
			}else{
				if(qdr->markpaint.type == QDR_MARKPAINT_PALETTE)
					mark_paint_group(qdr, cr, group_pattern, j, i);
				cairo_fill(cr);
			}
		}
	}
	
	//�摜�\��t��
	paste_paint(qdr, cr);
	
	//�e�L�X�g(�摜���O�ɂ��ׂ����H)
	text_paint(qdr, cr);
	
	//for pdf
	cairo_show_page(cr);
	
	if(group_pattern)
		cairo_pattern_destroy(group_pattern);
	if(pattern)
		cairo_pattern_destroy(pattern);
	if(image)
		cairo_surface_destroy(image);
	
	cairo_destroy(cr);
}

//=================================================================================
// qdr_save
//=================================================================================
int qdr_save(Qdr *qdr, QDR_FORMAT format, const char *file)
{
	if(!qdr)
		return 1;
	if(!file)
		return 1;
	
	if(qdr->surface){
		//surface�����݂��Ă��āAtype=Image�ł����filter�K�p���ꂽ�\��������̂�draw���Ȃ�
		if(cairo_surface_get_type(qdr->surface)==CAIRO_SURFACE_TYPE_IMAGE && format==QDR_FORMAT_PNG)
			goto PNG;
		
		//surface.type��format������Ă���ꍇ�́Afilter�������̂ɈႤformat�ŏ������悤�Ƃ��Ă���?
		fprintf(stderr, "[warn] %s(%d): filter destroy!\n", __func__, __LINE__);
		cairo_surface_destroy(qdr->surface);
	}
	
	switch(format){
		case QDR_FORMAT_PNG:
			qdr->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, qdr->rsize, qdr->rsize);
			break;
		case QDR_FORMAT_SVG:
			qdr->surface = cairo_svg_surface_create(file, qdr->rsize, qdr->rsize);
			break;
		case QDR_FORMAT_PDF:
			qdr->surface = cairo_pdf_surface_create(file, qdr->rsize, qdr->rsize);
			break;
		default:
			fprintf(stderr, "[warn] %s(%d): unknown format(%d)\n", __func__, __LINE__, format);
			return 1;
	}
	
	qdr_draw(qdr);

PNG:
	if(format == QDR_FORMAT_PNG)
		cairo_surface_write_to_png(qdr->surface, file);
	
	cairo_surface_destroy(qdr->surface);
	qdr->surface=NULL;
	return 0;
}

//=================================================================================
// qdr_set_mark_shadow
//=================================================================================
void qdr_set_mark_shadow(Qdr *qdr, int offsetx, int offsety, 
		unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	if(!qdr)
		return;
	if(offsetx==0 && offsety==0)
		return;
	
	qdr->shadow.offsetx = offsetx;
	qdr->shadow.offsety = offsety;
	qdr->shadow.r = (double)red/255;
	qdr->shadow.g = (double)green/255;
	qdr->shadow.b = (double)blue/255;
	qdr->shadow.a = (double)alpha/255;
}

//=================================================================================
// qdr_hole
//=================================================================================
int qdr_hole(Qdr *qdr, QDR_LEVEL level)
{
	int i, j, lm, s;
	
	if(!qdr)
		return 1;
	if(level<=0 || level>30)
		return 1;
	
	//��ӂ̃T�C�Y
	lm = sqrt(qdr->ssize * qdr->ssize * level/100);
	lm -= 4;	//�ڂ�����󂢂ĔF���ł��Ȃ��\��������̂�
	
	//�z�[�����Z���^�[�ɃL���C�ɔz�u�����悤�ɒ���
	if((qdr->ssize - lm) % 2)
		lm--;
	
	s = (qdr->ssize - lm)/ 2;
	
	//�^�[�Q�b�g�̃V���{�������N���A����
	for(i=s; i<s+lm; i++)
		for(j=s; j<s+lm; j++)
			qdr->data[j][i] = 0;
	
	return 0;
}
