#include "qdr_eye.h"

//=================================================================================
// eye_mark_init
//=================================================================================
void eye_mark_init(Qdr *qdr)
{
	int px[3], py[3];
	int i, x, y, mx, my;
	unsigned char m[7][7] = {
		{ 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1 }
	};
	
	memset(qdr->eyes.data, 0xff, QDR_MAXMSIZE*QDR_MAXMSIZE);
	
	//マーク外側
	i=0;
	px[0]=0,            py[0]=0;
	px[1]=qdr->ssize-7, py[1]=0;
	px[2]=0,            py[2]=qdr->ssize - 7;
	do {
		for(y=py[i],my=0; y<py[i]+7; y++,my++){
			for(x=px[i],mx=0; x<px[i]+7; x++,mx++){
				if(m[my][mx])
					qdr->eyes.data[x][y] = i;
			}
		}
		i++;
	}while(i < 3);
	
	//マーク内側
	i=0;
	px[0]=2,            py[0]=2;
	px[1]=qdr->ssize-5, py[1]=2;
	px[2]=2,            py[2]=qdr->ssize-5;
	do {
		for(y=py[i]; y<py[i]+3; y++){
			for(x=px[i]; x<px[i]+3; x++){
				qdr->eyes.data[x][y] = i+3;
			}
		}
		i++;
	}while(i < 3);
	
	//マーク内側の最終セル
	qdr->eyes.lastcell[0].x = 4;
	qdr->eyes.lastcell[0].y = 4;
	qdr->eyes.lastcell[1].x = qdr->ssize - 3;
	qdr->eyes.lastcell[1].y = 4;
	qdr->eyes.lastcell[2].x = 4;
	qdr->eyes.lastcell[2].y = qdr->ssize - 3;
}

//=================================================================================
// eye_check
//=================================================================================
int eye_check(Qdr *qdr, int x, int y)
{
	if(qdr->eyes.data[x][y] != 0xff){
		//色が有効な時
		if(qdr->eyes.eye[qdr->eyes.data[x][y]].enable)
			return 1;
		
		//画像が有効な時
		if(qdr->eyes.eye[qdr->eyes.data[x][y]].image)
			return 1;
	}
	
	return 0;
}

//=================================================================================
// eye_paint
//=================================================================================
void eye_paint(Qdr *qdr, cairo_t *cr, int x, int y)
{
	struct QDREye *e = &qdr->eyes.eye[qdr->eyes.data[x][y]];

	cairo_save(cr);
	
	if(e->enable){
		//色が有効な時
		cairo_set_source_rgba(cr, e->r, e->g, e->b, e->a);
	}else{
		//色が無効な時 ==> pathを殺すために透明にしてしまう。
		cairo_set_source_rgba(cr, e->r, e->g, e->b, 0x00);
	}
	cairo_fill(cr);
	
	//画像が有効な時
	if(e->image){
		//最終セルであるか？
		int i;
		for(i=0; i<3; i++){
			if(qdr->eyes.lastcell[i].x==x && qdr->eyes.lastcell[i].y==y){
				//画像有設定でかつinner領域の最終セルなので画像を貼る
				//cairo_surface_t *surface = cairo_image_surface_create_from_png(e->image);
				struct QDRBindImage b;
				cairo_surface_t *surface = image_surface_create(&b, e->image);
				if(!surface)
					return;
				
				cairo_pattern_t *pattern = cairo_pattern_create_for_surface(surface);
				
				int w = cairo_image_surface_get_width(surface);
				int h = cairo_image_surface_get_height(surface);
				int t = qdr->msize * 3;	//実際に貼り付ける領域の一辺のサイズ

				//scaling
				if(w>t || h>t){
					int big = w>h ? w : h;
					double scaling = (double)t/big;
					
					w *= scaling;
					h *= scaling;
					
					cairo_matrix_t matrix;
					cairo_matrix_init_scale(&matrix, 1/scaling, 1/scaling);
					cairo_pattern_set_matrix(pattern, &matrix);
				}
				
				cairo_translate(cr, (x-2+qdr->margin)*qdr->msize+(t-w)/2, (y-2+qdr->margin)*qdr->msize+(t-h)/2);
				cairo_set_source(cr, pattern);
				cairo_rectangle(cr, 0, 0, t, t);
				cairo_fill(cr);
				
				cairo_pattern_destroy(pattern);
				cairo_surface_destroy(surface);
				if(b.data)
					free(b.data);
				break;
			}
		}
	}
	
	cairo_restore(cr);
}

//=================================================================================
// qdr_eye_color_reset
//=================================================================================
static int eye_index(Qdr *qdr, int index, int under, int upper)
{
	if(!qdr)
		return 1;
	if(index<under || index>upper){
		fprintf(stderr, "[warn] %s(%d) out of index(%d: %d..%d).\n", __func__, __LINE__, index, under, upper);
		return 1;
	}
	return 0;
}

//=================================================================================
// eye_color
//=================================================================================
static void eye_color(Qdr *qdr, int index, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	qdr->eyes.eye[index].enable = 1;
	qdr->eyes.eye[index].r = (double)r/255;
	qdr->eyes.eye[index].g = (double)g/255;
	qdr->eyes.eye[index].b = (double)b/255;
	qdr->eyes.eye[index].a = (double)a/255;
}

//=================================================================================
// qdr_eye_outer_color
//=================================================================================
void qdr_eye_outer_color(Qdr *qdr, int index, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if(eye_index(qdr, index, 0, 2))
		return;
	eye_color(qdr, index, r, g, b, a);
}

//=================================================================================
// qdr_eye_inner_color
//=================================================================================
void qdr_eye_inner_color(Qdr *qdr, int index, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if(eye_index(qdr, index, 0, 2))
		return;
	eye_color(qdr, index+3, r, g, b, a);
}

//=================================================================================
// qdr_eye_image
//=================================================================================
void qdr_eye_image(Qdr *qdr, int index, const char *file)
{
	if(eye_index(qdr, index, 0, 2))
		return;
	if(!file)
		return;
		
#ifdef QDR_SWIG_PERL
	if(qdr->eyes.eye[index+3].image)
		free(qdr->eyes.eye[index+3].image);
	qdr->eyes.eye[index+3].image = file ? strdup(file) : NULL;
#else
	qdr->eyes.eye[index+3].image = file;
#endif
}

//=================================================================================
// qdr_eye_outer_color_reset
//=================================================================================
void qdr_eye_outer_color_reset(Qdr *qdr, int index)
{
	if(eye_index(qdr, index, 0, 2))
		return;
	
	qdr->eyes.eye[index].enable = 0;
}

//=================================================================================
// qdr_eye_inner_color_reset
//=================================================================================
void qdr_eye_inner_color_reset(Qdr *qdr, int index)
{
	if(eye_index(qdr, index, 0, 2))
		return;
	qdr->eyes.eye[index+3].enable = 0;
}

//=================================================================================
// qdr_eye_image_reset
//=================================================================================
void qdr_eye_image_reset(Qdr *qdr, int index)
{
	if(eye_index(qdr, index, 0, 2))
		return;
	qdr->eyes.eye[index+3].image = NULL;
}
