#include "qdr_mark.h"

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
void mark_rectangle(Qdr *qdr, cairo_t *cr, int _x, int _y)
{
	int x = (qdr->margin + _x) * qdr->msize;
	int y = (qdr->margin + _y) * qdr->msize;
	cairo_rectangle(cr, x, y, qdr->msize, qdr->msize);
}

//=================================================================================
// mark_arc
//=================================================================================
void mark_arc(Qdr *qdr, cairo_t *cr, int _x, int _y)
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
void mark_blockarc(Qdr *qdr, cairo_t *cr, int j, int i)
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
		case 4:	//MB_FULL
			cairo_line_to(cr, x, y-radius);
			cairo_rel_line_to(cr, radius, 0);
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
		case 4:
			cairo_line_to(cr, x+qdr->msize, y-radius);
			cairo_rel_line_to(cr, 0, radius);
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
		case 4:
			cairo_line_to(cr, x+qdr->msize, y+qdr->msize);
			cairo_rel_line_to(cr, -radius, 0);
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
		case 4:
			cairo_line_to(cr, x, y+qdr->msize);
			cairo_rel_line_to(cr, 0, -radius);
			break;
	}
}

//=================================================================================
// mark_metaball
//=================================================================================
void mark_metaball(Qdr *qdr, cairo_t *cr, int j, int i)
{
	unsigned char a, b, c;
	int edge[4], e, x, y;
	enum {
		MB_NON = 0,
		MB_TB,			//top or bottom
		MB_LR,			//left or right
		MB_BOTH,		//両方
		MB_FULL			//周囲全て(RECT)
	};
	void (*metaball[])(Qdr *qdr, cairo_t *cr, int edge, int x, int y, double radius) = {
		metaball_a,
		metaball_b,
		metaball_c,
		metaball_d
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
	edge[0] = (b==0 && c==0) ? MB_NON  :
			  (b==1 && c==0) ? MB_TB   :
			  (b==0 && c==1) ? MB_LR   :
			  (a==1 && b==1 && c==1) ? MB_FULL : MB_BOTH;
			  
	// B(右上)
	// +---+---+
	// | a | b |
	// +---+---+
	// | * | c |
	// +---+---+
	a = (i<=0)                    ? 0 : qdr->data[j][i-1];
	b = (j>=qdr->ssize-1 || i<=0) ? 0 : qdr->data[j+1][i-1];
	c = (j>=qdr->ssize-1)         ? 0 : qdr->data[j+1][i];
	edge[1] = (a==0 && c==0) ? MB_NON  :
			  (a==1 && c==0) ? MB_TB   :
			  (a==0 && c==1) ? MB_LR   :
			  (a==1 && b==1 && c==1) ? MB_FULL : MB_BOTH;

	// C(右下)
	// +---+---+
	// | * | a |
	// +---+---+
	// | b | c |
	// +---+---+
	a = (j>=qdr->ssize-1) ? 0 : qdr->data[j+1][i];
	b = (i>=qdr->ssize-1) ? 0 : qdr->data[j][i+1];
	c = (j>=qdr->ssize-1 || i>=qdr->ssize-1) ? 0 : qdr->data[j+1][i+1];
	edge[2] = (a==0 && b==0) ? MB_NON :
			  (a==1 && b==0) ? MB_LR  :
			  (a==0 && b==1) ? MB_TB  :
			  (a==1 && b==1 && c==1) ? MB_FULL : MB_BOTH;

	// D(左下)
	// +---+---+
	// | a | * |
	// +---+---+
	// | b | c |
	// +---+---+
	a = (j<=0)                    ? 0 : qdr->data[j-1][i];
	b = (j<=0 || i>=qdr->ssize-1) ? 0 : qdr->data[j-1][i+1];
	c = (i>=qdr->ssize-1)         ? 0 : qdr->data[j][i+1];
	edge[3] = (a==0 && c==0) ? MB_NON :
			  (a==1 && c==0) ? MB_LR  :
			  (a==0 && c==1) ? MB_TB  :
			  (a==1 && b==1 && c==1) ? MB_FULL : MB_BOTH;
	
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

