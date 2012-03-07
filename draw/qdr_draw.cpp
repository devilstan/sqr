#include "qdr_draw.h"

//=================================================================================
// eye_mark_setup
//=================================================================================
static void eye_mark_init(Qdr *qdr)
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
	
	//int j;
	//for(i=0; i<qdr->ssize; i++){
	//	for(j=0; j<qdr->ssize; j++){
	//		printf("%02X ", qdr->eyes.data[i][j]);
	//	}
	//	printf("\n");
	//}
	
	//マーク内側の最終セル
	qdr->eyes.lastcell[0].x = 4;
	qdr->eyes.lastcell[0].y = 4;
	qdr->eyes.lastcell[1].x = qdr->ssize - 3;
	qdr->eyes.lastcell[1].y = 4;
	qdr->eyes.lastcell[2].x = 4;
	qdr->eyes.lastcell[2].y = qdr->ssize - 3;
}

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
	
	//シンボル形状
	qdr->marktype = QDR_MARKTYPE_RECTANGLE;
	qdr->radius = QDR_DEFAULT_RADIUS;
	
	//背景
	qdr->background.type = QDR_BACKGROUND_SIMPLE;
	qdr->background.r = 1.0;
	qdr->background.g = 1.0;
	qdr->background.b = 1.0;
	qdr->background.a = 1.0;
	
	//シンボル塗り情報
	qdr->markpaint.type = QDR_MARKPAINT_SIMPLE;
	qdr->markpaint.r = 0.0;
	qdr->markpaint.g = 0.0;
	qdr->markpaint.b = 0.0;
	qdr->markpaint.a = 1.0;
	
	//eye_markセットアップ
	eye_mark_init(qdr);
	
//{
//	int x, y;
//	for(y=0; y<qdr->ssize; y++){
//		for(x=0; x<qdr->ssize; x++){
//			printf("%02X ", qdr->eyes.data[x][y]);
//		}
//		printf("\n");
//	}
//}
	
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
// qdr_set_mark
//=================================================================================
void qdr_set_mark(Qdr *qdr, QDR_MARKTYPE type, double radius)
{
	if(!qdr)
		return;
	
	qdr->marktype = type;
	qdr->radius = radius;
}

//=================================================================================
// mark_rectangle
//=================================================================================
static void mark_rectangle(Qdr *qdr, cairo_t *cr, int _x, int _y)
{
	int x = (qdr->margin + _x) * qdr->msize;
	int y = (qdr->margin + _y) * qdr->msize;
	cairo_rectangle(cr, x, y, qdr->msize, qdr->msize);
}

//=================================================================================
// mark_arc
//=================================================================================
static void mark_arc(Qdr *qdr, cairo_t *cr, int _x, int _y)
{
	int x, y;
	double px0, py0, px1, py1;
	double radius;	//円弧の半径
	
	//左上起点
	x = (qdr->margin + _x) * qdr->msize;
	y = (qdr->margin + _y) * qdr->msize;
	
	//円の中心座標
	radius = qdr->msize * qdr->radius;
	px0 = x + radius;
	py0 = y + radius;
	px1 = x + qdr->msize - radius;
	py1 = y + qdr->msize - radius;
	
	// A (px0, py0)
	cairo_arc(cr, px0, py0, radius, 180*(M_PI/180.0), 270*(M_PI/180.0));
	// B (px1, py0)
	cairo_arc(cr, px1, py0, radius, 270*(M_PI/180.0), 360*(M_PI/180.0));
	// C (px1, py1)
	cairo_arc(cr, px1, py1, radius,   0*(M_PI/180.0),  90*(M_PI/180.0));
	// D (px0, py1)
	cairo_arc(cr, px0, py1, radius,  90*(M_PI/180.0), 180*(M_PI/180.0));
}

//=================================================================================
// blockarc
//=================================================================================
static void blockarc_a(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double half, double lead, double radius)
{
	switch(edge){
		case 0:	//rect
			cairo_rel_line_to(cr, 0, -half);
			cairo_rel_line_to(cr, half, 0);
			break;
		case 1:	//arc
			cairo_rel_line_to(cr, 0, -lead);
			cairo_arc(cr, x+radius, y+radius, radius, 180*(M_PI/180.0), 270*(M_PI/180.0));
			cairo_rel_line_to(cr, lead, 0);
			break;
		case 2:	//narc
			cairo_rel_line_to(cr, 0, -lead);
			cairo_arc_negative(cr, x-radius, y+radius, radius, 0*(M_PI/180.0), 270*(M_PI/180.0));
			cairo_rel_line_to(cr, radius+half, 0);
			break;
	}
}
static void blockarc_b(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double half, double lead, double radius)
{
	switch(edge){
		case 0:	//rect
			cairo_rel_line_to(cr, half, 0);
			cairo_rel_line_to(cr, 0, half);
			break;
		case 1:	//arc
			cairo_rel_line_to(cr, lead, 0);
			cairo_arc(cr, x+half+lead, y+radius, radius, 270*(M_PI/180.0), 360*(M_PI/180.0));
			cairo_rel_line_to(cr, 0, lead);
			break;
		case 2:	//narc
			cairo_rel_line_to(cr, radius+half, 0);
			cairo_arc_negative(cr, x+qdr->msize+radius, y+radius, radius, 270*(M_PI/180.0), 180*(M_PI/180.0));
			cairo_rel_line_to(cr, 0, lead);
			break;
	}
}
static void blockarc_c(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double half, double lead, double radius)
{
	switch(edge){
		case 0:	//rect
			cairo_rel_line_to(cr, 0, half);
			cairo_rel_line_to(cr, -half, 0);
			break;
		case 1:	//arc
			cairo_rel_line_to(cr, 0, lead);
			cairo_arc(cr, x+half+lead, y+half+lead, radius, 0*(M_PI/180.0), 90*(M_PI/180.0));
			cairo_rel_line_to(cr, -lead, 0);
			break;
		case 2:	//narc
			cairo_rel_line_to(cr, 0, lead);
			cairo_arc_negative(cr, x+qdr->msize+radius, y+half+lead, radius, 180*(M_PI/180.0), 90*(M_PI/180.0));
			cairo_rel_line_to(cr, -(radius+half), 0);
			break;
	}
}
static void blockarc_d(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double half, double lead, double radius)
{
	switch(edge){
		case 0:	//rect
			cairo_rel_line_to(cr, -half, 0);
			cairo_rel_line_to(cr, 0, -half);
			break;
		case 1:	//arc
			cairo_rel_line_to(cr, -lead, 0);
			cairo_arc(cr, x+radius, y+half+lead, radius, 90*(M_PI/180.0), 180*(M_PI/180.0));
			cairo_rel_line_to(cr, 0, -lead);
			break;
		case 2:	//narc
			cairo_rel_line_to(cr, -(half+radius), 0);
			cairo_arc_negative(cr, x-radius, y+half+lead, radius, 90*(M_PI/180.0), 0*(M_PI/180.0));
			cairo_rel_line_to(cr, 0, -lead);
			break;
	}
}

//=================================================================================
// mark_blockarc
//=================================================================================
static void mark_blockarc(Qdr *qdr, cairo_t *cr, int j, int i)
{
	unsigned char a, b, c;
	int edge[4], e, x, y;
	enum {
		EDGE_RECT = 0,
		EDGE_ARC,
		EDGE_NARC
	};
	void (*blockarc[])(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double half, double lead, double radius) = {
		blockarc_a,
		blockarc_b,
		blockarc_c,
		blockarc_d
	};
	
	// A(左上)
	// +---+---+
	// | a | b |
	// +---+---+
	// | c | * |
	// +---+---+
	a = (j<=0 || i<=0) ? 0 : qdr->data[j-1][i-1];
	b = (i<=0)         ? 0 : qdr->data[j][i-1];
	c = (j<=0)         ? 0 : qdr->data[j-1][i];
	edge[0] = (b==0 && c==0)         ? EDGE_ARC : 
			  (a==1 && b==1 && c==0) ? EDGE_NARC : EDGE_RECT;
	
	// B(右上)
	// +---+---+
	// | a | b |
	// +---+---+
	// | * | c |
	// +---+---+
	a = (i<=0)                    ? 0 : qdr->data[j][i-1];
	b = (j>=qdr->ssize-1 || i<=0) ? 0 : qdr->data[j+1][i-1];
	c = (j>=qdr->ssize-1)         ? 0 : qdr->data[j+1][i];
	edge[1] = (a==0 && c==0)         ? EDGE_ARC :
			  (a==1 && b==1 && c==0) ? EDGE_NARC : EDGE_RECT;
	
	// C(右下)
	// +---+---+
	// | * | a |
	// +---+---+
	// | b | c |
	// +---+---+
	a = (j>=qdr->ssize-1) ? 0 : qdr->data[j+1][i];
	b = (i>=qdr->ssize-1) ? 0 : qdr->data[j][i+1];
	c = (j>=qdr->ssize-1 || i>=qdr->ssize-1) ? 0 : qdr->data[j+1][i+1];
	edge[2] = (a==0 && b==0)         ? EDGE_ARC :
			  (a==0 && b==1 && c==1) ? EDGE_NARC : EDGE_RECT;

	// D(左下)
	// +---+---+
	// | a | * |
	// +---+---+
	// | b | c |
	// +---+---+
	a = (j<=0)                    ? 0 : qdr->data[j-1][i];
	b = (j<=0 || i>=qdr->ssize-1) ? 0 : qdr->data[j-1][i+1];
	c = (i>=qdr->ssize-1)         ? 0 : qdr->data[j][i+1];
	edge[3] = (a==0 && c==0)         ? EDGE_ARC :
			  (a==0 && b==1 && c==1) ? EDGE_NARC : EDGE_RECT;
	
	
	double radius = qdr->msize * qdr->radius;
	double half = qdr->msize / 2.0;
	double lead = half - radius;

	//セルの左上
	x = (qdr->margin + j) * qdr->msize;
	y = (qdr->margin + i) * qdr->msize;
	
	//パスの起点
	cairo_move_to(cr, x, y+half);
	
	//各4点のパスを構築(左上->右上->右下->左下)
	for(e=0; e<4; e++)
		blockarc[e](qdr, cr, edge[e], x, y, half, lead, radius);
}


//=================================================================================
// metaball
//=================================================================================
static void metaball_a(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double radius)
{
	switch(edge){
		case 0:	//MB_NON
			cairo_arc(cr, x+radius, y+radius, radius, 180*(M_PI/180.0), 270*(M_PI/180.0));
			break;
		case 1:	//MB_TB
			cairo_arc(cr, x+radius, y+radius, radius, 180*(M_PI/180.0), 210*(M_PI/180.0));
			cairo_arc_negative(cr, x+radius-sqrt(3)*radius, y, radius, 30*(M_PI/180.0), 0*(M_PI/180.0));
			break;
		case 2:	//MB_LR
			cairo_arc_negative(cr, x, y+radius-sqrt(3)*radius, radius, 90*(M_PI/180.0), 60*(M_PI/180.0));
			cairo_arc(cr, x+radius, y+radius, radius, 240*(M_PI/180.0), 270*(M_PI/180.0));
			break;
		case 3:	//MB_BOTH
			cairo_arc_negative(cr, x, y, (2-sqrt(3))*radius, 90*(M_PI/180.0), 0*(M_PI/180.0));
			break;
	}
}

static void metaball_b(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double radius)
{
	switch(edge){
		case 0:
			cairo_arc(cr, x+radius, y+radius, radius, 270*(M_PI/180.0), 360*(M_PI/180.0));
			break;
		case 1:
			cairo_arc_negative(cr, x+radius+sqrt(3)*radius, y, radius, 180*(M_PI/180.0), 150*(M_PI/180.0));
			cairo_arc(cr, x+radius, y+radius, radius, 330*(M_PI/180.0), 360*(M_PI/180.0));
			break;
		case 2:
			cairo_arc(cr, x+radius, y+radius, radius, 270*(M_PI/180.0), 300*(M_PI/180.0));
			cairo_arc_negative(cr, x+radius*2, y+radius-sqrt(3)*radius, radius, 120*(M_PI/180.0), 90*(M_PI/180.0));
			break;
		case 3:
			cairo_arc_negative(cr, x+radius*2, y, (2-sqrt(3))*radius, 180*(M_PI/180.0), 90*(M_PI/180.0));
			break;
	}
}

static void metaball_c(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double radius)
{
	switch(edge){
		case 0:
			cairo_arc(cr, x+radius, y+radius, radius, 0*(M_PI/180.0), 90*(M_PI/180.0));
			break;
		case 1:
			cairo_arc(cr, x+radius, y+radius, radius, 0*(M_PI/180.0), 30*(M_PI/180.0));
			cairo_arc_negative(cr, x+radius+sqrt(3)*radius, y+radius*2, radius, 210*(M_PI/180.0), 180*(M_PI/180.0));
			break;
		case 2:
			cairo_arc_negative(cr, x+radius*2, y+radius+sqrt(3)*radius, radius, 270*(M_PI/180.0), 240*(M_PI/180.0));
			cairo_arc(cr, x+radius, y+radius, radius, 60*(M_PI/180.0), 90*(M_PI/180.0));
			break;
		case 3:
			cairo_arc_negative(cr, x+radius*2, y+radius*2, (2-sqrt(3))*radius, 270*(M_PI/180.0), 180*(M_PI/180.0));
			break;
	}
}

static void metaball_d(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double radius)
{
	switch(edge){
		case 0:
			cairo_arc(cr, x+radius, y+radius, radius, 90*(M_PI/180.0), 180*(M_PI/180.0));
			break;
		case 1:
			cairo_arc_negative(cr, x+radius-sqrt(3)*radius, y+radius*2, radius, 0*(M_PI/180.0), 330*(M_PI/180.0));
			cairo_arc(cr, x+radius, y+radius, radius, 150*(M_PI/180.0), 180*(M_PI/180.0));
			break;
		case 2:
			cairo_arc(cr, x+radius, y+radius, radius, 90*(M_PI/180.0), 120*(M_PI/180.0));
			cairo_arc_negative(cr, x, y+radius+sqrt(3)*radius, radius, 300*(M_PI/180.0), 270*(M_PI/180.0));
			break;
		case 3:
			cairo_arc_negative(cr, x, y+radius*2, (2-sqrt(3))*radius, 0*(M_PI/180.0), 270*(M_PI/180.0));
			break;
	}
}

//=================================================================================
// mark_metaball
//=================================================================================
static void mark_metaball(Qdr *qdr, cairo_t *cr, int j, int i)
{
	unsigned char a, b;
	int edge[4], e, x, y;
	enum {
		MB_NON = 0,
		MB_TB,			//top or bottom
		MB_LR,			//left or right
		MB_BOTH			//両方
	};
	void (*metaball[])(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double radius) = {
		metaball_a,
		metaball_b,
		metaball_c,
		metaball_d
	};
	
	// A(左上)
	// +---+---+
	// | - | a |
	// +---+---+
	// | b | * |
	// +---+---+
	a = (i<=0) ? 0 : qdr->data[j][i-1];
	b = (j<=0) ? 0 : qdr->data[j-1][i];
	edge[0] = (a==0 && b==0) ? MB_NON :
			  (a==1 && b==0) ? MB_TB  :
			  (a==0 && b==1) ? MB_LR  : MB_BOTH;
	
	// B(右上)
	// +---+---+
	// | a | - |
	// +---+---+
	// | * | b |
	// +---+---+
	a = (i<=0)            ? 0 : qdr->data[j][i-1];
	b = (j>=qdr->ssize-1) ? 0 : qdr->data[j+1][i];
	edge[1] = (a==0 && b==0) ? MB_NON :
			  (a==1 && b==0) ? MB_TB  :
			  (a==0 && b==1) ? MB_LR  : MB_BOTH;

	// C(右下)
	// +---+---+
	// | * | a |
	// +---+---+
	// | b | - |
	// +---+---+
	a = (j>=qdr->ssize-1) ? 0 : qdr->data[j+1][i];
	b = (i>=qdr->ssize-1) ? 0 : qdr->data[j][i+1];
	edge[2] = (a==0 && b==0) ? MB_NON :
			  (a==1 && b==0) ? MB_LR  :
			  (a==0 && b==1) ? MB_TB  : MB_BOTH;

	// D(左下)
	// +---+---+
	// | a | * |
	// +---+---+
	// | - | b |
	// +---+---+
	a = (j<=0)            ? 0 : qdr->data[j-1][i];
	b = (i>=qdr->ssize-1) ? 0 : qdr->data[j][i+1];
	edge[3] = (a==0 && b==0) ? MB_NON :
			  (a==1 && b==0) ? MB_LR  :
			  (a==0 && b==1) ? MB_TB  : MB_BOTH;
	
	
	double radius = qdr->msize / 2.0;

	//セルの左上
	x = (qdr->margin + j) * qdr->msize;
	y = (qdr->margin + i) * qdr->msize;
	
	//パスの起点
	cairo_move_to(cr, x, y+radius);
	
	//各4点のパスを構築(左上->右上->右下->左下)
	for(e=0; e<4; e++)
		metaball[e](qdr, cr, edge[e], x, y, radius);
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
		
		//隙間ができないようにスケーリングする
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
	//グラデーション
	if(qdr->markpaint.type == QDR_MARKPAINT_GRAD){
		pattern = grad_pattern(qdr, &qdr->markpaint.grad, qdr->margin*qdr->msize, qdr->margin*qdr->msize, qdr->ssize*qdr->msize/2);
		cairo_set_source(cr, pattern);
		return;
	}
	
	//画像
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
// eye_check
//=================================================================================
static int eye_check(Qdr *qdr, int x, int y)
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
static void eye_paint(Qdr *qdr, cairo_t *cr, int x, int y)
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
				cairo_surface_t *surface = cairo_image_surface_create_from_png(e->image);
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
				break;
			}
		}
	}
	
	cairo_restore(cr);
}

//=================================================================================
// paste_paint
//=================================================================================
static void paste_paint(Qdr *qdr, cairo_t *cr)
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
		//誤り訂正レベルに応じた許容サイズ
		int lm = sqrt(qdr->ssize*qdr->msize * qdr->ssize*qdr->msize * qdr->paste.level/100);
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
	
	cairo_restore(cr);
}

//=================================================================================
// text_paint
//=================================================================================
static void text_paint(Qdr *qdr, cairo_t *cr)
{
	cairo_text_extents_t extents;
	
	if(!qdr->text.utf8)
		return;
	
	cairo_save(cr);
	
	cairo_select_font_face(cr, qdr->text.font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, qdr->text.height);
	cairo_set_source_rgba(cr, qdr->text.r, qdr->text.g, qdr->text.b, qdr->text.a);
	
	cairo_text_extents(cr, qdr->text.utf8, &extents);
	
	//オペレータは要検討!
	//cairo_set_operator(cr, CAIRO_OPERATOR_HARD_LIGHT);
	//cairo_set_operator(cr, CAIRO_OPERATOR_DARKEN);
	
	int x = (qdr->rsize - extents.width) / 2;
	int y = (qdr->rsize + qdr->text.height) / 2;
	
	cairo_move_to(cr, x, y);
	cairo_show_text(cr, qdr->text.utf8);
	
	cairo_restore(cr);
}

//=================================================================================
// qdr_draw
//=================================================================================
void qdr_draw(Qdr *qdr)
{
	cairo_t         *cr;
	cairo_pattern_t *pattern=NULL;
	cairo_surface_t *image=NULL;
	int i, j, is_shadow;
	void (*mark)(Qdr *qdr, cairo_t *cr, int x, int y);
	
	cr = cairo_create(qdr->surface);

	//背景のセット
	background(qdr, cr);
	
	//グローバルマークペイント設定
	mark_paint_set_source(qdr, cr, pattern, image);
	
	//マーク形状に応じてメソッド選択
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
	
	//影
	is_shadow = (qdr->shadow.offsetx!=0 || qdr->shadow.offsety!=0);
	
	//マーク描画
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
				cairo_fill(cr);
			}
		}
	}
	
	//画像貼り付け
	paste_paint(qdr, cr);
	
	//テキスト(画像より前にすべきか？)
	text_paint(qdr, cr);
	
	//for pdf
	cairo_show_page(cr);
	
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
		//surfaceが存在していて、type=Imageであればfilter適用された可能性があるのでdrawしない
		if(cairo_surface_get_type(qdr->surface)==CAIRO_SURFACE_TYPE_IMAGE && format==QDR_FORMAT_PNG)
			goto PNG;
		
		//surface.typeとformatが違っている場合は、filterかけたのに違うformatで処理しようとしている?
		fprintf(stderr, "[warn] %s(%d): filter destroy!\n", __func__, __LINE__);
		cairo_surface_destroy(qdr->surface);
	}
	
	switch(format){
		case QDR_FORMAT_PNG:
			qdr->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, qdr->rsize, qdr->rsize);
			break;
		//case QDR_FORMAT_SVG:
		//	qdr->surface = cairo_svg_surface_create(file, qdr->rsize, qdr->rsize);
		//	break;
		//case QDR_FORMAT_PDF:
		//	qdr->surface = cairo_pdf_surface_create(file, qdr->rsize, qdr->rsize);
		//	break;
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
	qdr->eyes.eye[index+3].image = file;
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
	lm = sqrt(qdr->ssize*qdr->msize * qdr->ssize*qdr->msize * level/100);
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
	
	//一辺のサイズ
	lm = sqrt(qdr->ssize * qdr->ssize * level/100);
	lm -= 4;	//ぼっこり空いて認識できない可能性があるので
	
	//ホールがセンターにキレイに配置されるように調整
	if((qdr->ssize - lm) % 2)
		lm--;
	
	s = (qdr->ssize - lm)/ 2;
	
	//ターゲットのシンボル情報をクリアする
	for(i=s; i<s+lm; i++)
		for(j=s; j<s+lm; j++)
			qdr->data[j][i] = 0;
	
	return 0;
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

