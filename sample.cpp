#include <stdio.h>
#include "QR_Encode.h"
#include "qdr.h"
#ifdef DEBUG
#include "dbg/dbg.h"
#endif

static void _save(Qdr *qdr, const char *n)
{
	char a[256];
	printf("%s\n", n);
	sprintf(a, "out/%s", n);
	qdr_save_png(qdr, a);
}

//=================================================================================
// 単色
//=================================================================================
static void simple(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_bg_color(&qdr, 0xee, 0xee, 0xee, 0xff);
	
	//デフォルト
	_save(&qdr, "simple1.png");

	//色変更
	qdr_set_mark_color(&qdr, 0xff, 0, 0, 0xff);
	_save(&qdr, "simple2.png");
	
	//セルサイズ変更
	qdr_set_msize(&qdr, 16);
	_save(&qdr, "simple3.png");
	
	//マージン変更
	qdr_set_msize(&qdr, QDR_DEFAULT_MSIZE);
	qdr_set_margin(&qdr, 4);
	_save(&qdr, "simple4.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// マーク形状(円)
//=================================================================================
static void mark_arc(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_mark_color(&qdr, 0, 0, 0xff, 0xff);
	qdr_set_msize(&qdr, 16);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_ARC, 0.1);
	_save(&qdr, "mark_arc1.png");
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_ARC, 0.2);
	_save(&qdr, "mark_arc2.png");
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_ARC, QDR_DEFAULT_RADIUS);
	_save(&qdr, "mark_arc3.png");
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_ARC, 0.4);
	_save(&qdr, "mark_arc4.png");
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_ARC, 0.5);
	_save(&qdr, "mark_arc5.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// マーク形状(ブロック)
//=================================================================================
static void mark_blockarc(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_mark_color(&qdr, 0, 0, 0xff, 0xff);
	qdr_set_msize(&qdr, 16);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.1);
	_save(&qdr, "mark_blockarc1.png");
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.2);
	_save(&qdr, "mark_blockarc2.png");
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, QDR_DEFAULT_RADIUS);
	_save(&qdr, "mark_blockarc3.png");
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.4);
	_save(&qdr, "mark_blockarc4.png");
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	_save(&qdr, "mark_blockarc5.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// mark_metaball
//=================================================================================
static void mark_metaball(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	//qdr_group(&qdr);
	//qdr_group_palette_rand(&qdr, 0, 200, 0xff);
	
	qdr_set_mark_color(&qdr, 0, 0, 0xff, 0xff);
	qdr_set_msize(&qdr, 16);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_METABALL, QDR_DEFAULT_RADIUS);
	_save(&qdr, "mark_metaball.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// 背景
//=================================================================================
static void background(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_msize(&qdr, 16);
	
	//単色
	qdr_bg_color(&qdr, 0x00, 0xee, 0xee, 0xff);
	_save(&qdr, "background1.png");
	
	//グラデーション
	qdr_bg_grad(&qdr, QDR_GRADIENT_TL2BR, 0xff, 0x55, 0x00, 0xff, 0x00, 0xff, 0xff, 0xff);
	_save(&qdr, "background2.png");
	
	//画像(アルファ適用)
	//qdr_bg_image(&qdr, "images/back1.png", 150);
	qdr_bg_image(&qdr, "images/back2.png", 80);
	_save(&qdr, "background3.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// グルーピング
//=================================================================================
static void group(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	qdr_set_msize(&qdr, 16);
	
	//グループ作成
	qdr_group(&qdr);
	
	//各グループにランダムに色を設定
	qdr_group_palette_rand(&qdr, 0, 200, 0xff);
	//int x=0;
	//for(int i=1; i<qdr_group_count(&qdr); i++){
	//	if(qdr_group_member_count(&qdr, i) == 0)
	//		continue;
	//	x++;
	//	if(x%2 == 0){
	//		qdr_group_palette(&qdr, i, 0x59, 0x49, 0x3f, 0xff);
	//		continue;
	//	}
	//	if(x%3 == 0){
	//		qdr_group_palette(&qdr, i, 0xe7, 0xb2, 0x1f, 0xff);
	//		continue;
	//	}
	//	qdr_group_palette(&qdr, i, 0, 0, 0, 0xff);
	//}
	
	_save(&qdr, "group1.png");
	
	//1セルのグループに画像を設定(下地の色は消す)
	qdr_group_image(&qdr, "images/heart.png", 0);
	_save(&qdr, "group2.png");
	

	//1セルのグループに画像を設定(下地の色は消す)
	qdr_group_image(&qdr, "images/gif-normal-transparent.gif", 0);
	_save(&qdr, "group3.png");
	
	
	qdr_close(&qdr);
}

//=================================================================================
// マーク塗り
//=================================================================================
static void markpaint(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_group(&qdr);
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	qdr_set_msize(&qdr, 16);
	
	//影を付ける
	qdr_set_mark_shadow(&qdr, 3, 3, 200, 200, 230, 0xff);
	
	//画像
	qdr_set_mark_image(&qdr, "images/back2.png", 0xff);
	_save(&qdr, "markpaint1.png");
	
	//グラデーション
	qdr_set_mark_grad(&qdr, QDR_GRADIENT_TL2BR, 0xff, 0, 0, 0xff, 0, 0, 0xff, 0xff);
	_save(&qdr, "markpaint2.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// eye
//=================================================================================
static void eye(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_group(&qdr);
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	qdr_set_msize(&qdr, 16);
	
	qdr_eye_outer_color(&qdr, 0, 0xff, 0x00, 0x00, 0xff);
	//qdr_eye_inner_color(&qdr, 0, 0x88, 0x00, 0x00, 0xff);
	qdr_eye_image(&qdr, 0, "images/0.png");

	qdr_eye_outer_color(&qdr, 1, 0x00, 0xff, 0x00, 0xff);
	//qdr_eye_inner_color(&qdr, 1, 0x00, 0x88, 0x00, 0xff);
	qdr_eye_image(&qdr, 1, "images/1.png");
	
	qdr_eye_outer_color(&qdr, 2, 0x00, 0x00, 0xff, 0xff);
	qdr_eye_inner_color(&qdr, 2, 0x00, 0x00, 0x88, 0xff);
	qdr_eye_image(&qdr, 2, "images/2.png");
	
	_save(&qdr, "eye.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// paste_layout
//=================================================================================
static void paste_layout(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	qdr_set_msize(&qdr, 16);
	
	qdr_group(&qdr);
	qdr_group_palette_rand(&qdr, 0, 200, 0xff);
	
	QDR_LEVEL level = qr->m_nLevel==0 ? QDR_LEVEL_L :
					  qr->m_nLevel==1 ? QDR_LEVEL_M :
					  qr->m_nLevel==2 ? QDR_LEVEL_Q :
					  qr->m_nLevel==3 ? QDR_LEVEL_H : QDR_LEVEL_L;
	qdr_paste_layout(&qdr, "images/tama.png", QDR_LAYOUT_SOUTH, level);

	_save(&qdr, "paste_layout.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// paste
//=================================================================================
static void paste(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	qdr_set_msize(&qdr, 16);
	
	qdr_group(&qdr);
	qdr_group_palette_rand(&qdr, 0, 200, 0xff);
	
	//png
	qdr_paste(&qdr, "images/tama.png", 100, 100);
	_save(&qdr, "paste-png.png");
	
	
	//jpeg-gray
	qdr_paste(&qdr, "images/jpg-gray.jpg", 100, 100);
	_save(&qdr, "paste-jpeg-gray.png");
	
	//jpeg-rgb
	qdr_paste(&qdr, "images/jpg-rgb.jpg", 100, 100);
	_save(&qdr, "paste-jpg-rgb.png");
	
	//jpeg-cmyk
	qdr_paste(&qdr, "images/jpg-cmyk.jpg", 100, 100);
	_save(&qdr, "paste-jpg-cmyk.png");
	
	
	//gif-anime
	qdr_paste(&qdr, "images/gif-anime.gif", 100, 100);
	_save(&qdr, "paste-gif-anime.png");

	//gif-interlace-transparent
	qdr_paste(&qdr, "images/gif-interlace-transparent.gif", 100, 100);
	_save(&qdr, "paste-gif-interlace-transparent.png");
	
	//gif-normal-transparent
	qdr_paste(&qdr, "images/gif-normal-transparent.gif", 100, 100);
	_save(&qdr, "paste-gif-normal-transparent.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// largest_rect
//=================================================================================
static void largest_rect()
{
	CQR_Encode qr;
	
	char *data = (char *)"@@@@@@@@@@@";
	
	if(!qr.EncodeData(1, 0, true, -1, data, strlen(data)))
		return;
	
	/////////////////////////////////////////////
	Qdr qdr;

	qdr_init(&qdr, qr.m_nSymbleSize, qr.m_byModuleData);
	
	qdr_set_msize(&qdr, 16);
	qdr_set_mark_color(&qdr, 0x20, 0, 0xf0, 0xff);
	qdr_set_margin(&qdr, 1);
	
	printf("(x, y) = (%d, %d)\n",
		qdr_largest_x(&qdr),
		qdr_largest_y(&qdr));
	printf("w = %d, h = %d\n",
		qdr_largest_width(&qdr),
		qdr_largest_height(&qdr));
	
	_save(&qdr, "largest_rect.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// hole
//=================================================================================
static void hole(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	qdr_set_msize(&qdr, 16);
	
	//qdr_group(&qdr);
	//qdr_group_palette_rand(&qdr, 0, 200, 0xff);
	
	QDR_LEVEL level = qr->m_nLevel==0 ? QDR_LEVEL_L :
					  qr->m_nLevel==1 ? QDR_LEVEL_M :
					  qr->m_nLevel==2 ? QDR_LEVEL_Q :
					  qr->m_nLevel==3 ? QDR_LEVEL_H : QDR_LEVEL_L;
	qdr_hole(&qdr, level);

	_save(&qdr, "hole.png");
	
	qdr_close(&qdr);
}

//=================================================================================
// text
//=================================================================================
static void text(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	qdr_set_msize(&qdr, 16);
	
	qdr_group(&qdr);
	qdr_group_palette_rand(&qdr, 0, 130, 0xff);
	
	qdr_text(&qdr, "cursive", "sns.creyle.jp", 16*3, 0x00, 0xe0, 0xff, 0xff);

	_save(&qdr, "text.png");
	
	qdr_close(&qdr);
}


//=================================================================================
// filter_setup
//=================================================================================
static void filter_setup(CQR_Encode *qr, Qdr *qdr, const char *name)
{
	qdr_init(qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
//	qdr_set_mark_color(qdr, 0, 100, 0xff, 0xff);
	
//	qdr_set_mark(qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
//	//qdr_set_msize(qdr, 16);
	qdr_group(qdr);
	qdr_group_palette_rand(qdr, 0, 180, 0xff);
	
	char a[256];
	sprintf(a, "out/%s_src.png", name);
	qdr_save_png(qdr, a);
}

//=================================================================================
// filter_invert
//=================================================================================
static void filter_invert(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_invert(&qdr);
	
	_save(&qdr, "filter_invert.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_crayon
//=================================================================================
static void filter_crayon(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_crayon(&qdr, 30.0, 3, 2, 65);
	
	_save(&qdr, "filter_crayon.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_circle
//=================================================================================
static void filter_circle(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_circle(&qdr, 3);
	
	_save(&qdr, "filter_circle.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_grid
//=================================================================================
static void filter_grid(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_grid(&qdr, 4, 30);
	
	_save(&qdr, "filter_grid.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_voronoi
//=================================================================================
static void filter_voronoi(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_voronoi(&qdr, 5);
	
	_save(&qdr, "filter_voronoi.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_dots
//=================================================================================
static void filter_dots(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_dots(&qdr, 5);
	
	_save(&qdr, "filter_dots.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_dots
//=================================================================================
static void filter_hexagon(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_hexagon(&qdr, 3);
	
	_save(&qdr, "filter_hexagon.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_dots
//=================================================================================
static void filter_diamond(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_diamond(&qdr, 3);
	
	_save(&qdr, "filter_diamond.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_dots
//=================================================================================
static void filter_tile(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_tile(&qdr, qdr.msize);
	
	_save(&qdr, "filter_tile.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_noise
//=================================================================================
static void filter_noise(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_noise(&qdr, 2);
	
	_save(&qdr, "filter_noise.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_hydrangea
//=================================================================================
static void filter_hydrangea(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_hydrangea(&qdr, 4);
	
	_save(&qdr, "filter_hydrangea.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_wave
//=================================================================================
static void filter_wave(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_wave(&qdr, 10.0, 2);
	
	_save(&qdr, "filter_wave.png");
	qdr_close(&qdr);
}

//=================================================================================
// filter_ball
//=================================================================================
static void filter_ball(CQR_Encode *qr)
{
	Qdr qdr;
	
	filter_setup(qr, &qdr, __func__);
	
	qdr_filter_ball(&qdr);
	
	_save(&qdr, "filter_ball.png");
	qdr_close(&qdr);
}

//=================================================================================
// output_svg
//=================================================================================
static void output_svg(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	qdr_set_msize(&qdr, 16);
	
	qdr_group(&qdr);
	qdr_group_palette_rand(&qdr, 0, 200, 0xff);
	qdr_save_svg(&qdr, "out/group1.svg");
	
	qdr_close(&qdr);
}

//=================================================================================
// output_pdf
//=================================================================================
static void output_pdf(CQR_Encode *qr)
{
	Qdr qdr;
	
	qdr_init(&qdr, qr->m_nSymbleSize, qr->m_byModuleData);
	
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	qdr_set_msize(&qdr, 16);
	
	qdr_group(&qdr);
	qdr_group_palette_rand(&qdr, 0, 200, 0xff);
	qdr_save_pdf(&qdr, "out/group1.pdf");
	
	qdr_close(&qdr);
}

//=============================================================================
// main
//=============================================================================
int main(int argc, char **argv)
{
	char *data = (char *)"abcdefghijklmnopqrstuvwxyz0123456789";
	
	if(argc >= 2)
		data = argv[1];
	
	CQR_Encode qr;
	int  nLevel      = 1;		//M
	int  nVersion    = 0;		//自動
	bool bAutoExtent = true;
	int  nMaskingNo  = -1;		//自動
	
	if(!qr.EncodeData(nLevel, nVersion, bAutoExtent, nMaskingNo, data, strlen(data)))
		return 1;

	printf("SymbleSize = %d\n", qr.m_nSymbleSize);
	for(int i=0; i<qr.m_nSymbleSize; i++){
		for(int j=0; j<qr.m_nSymbleSize; j++){
			printf("%d ", qr.m_byModuleData[j][i]);
		}
		printf("\n");
	}
	
//--- version
	printf("version\n");
	printf("  qdr:   %06d\n", qdr_version());
	printf("  cairo: %06d\n", qdr_use_cairo_version());
	printf("  pikl:  %06d\n", qdr_use_pikl_version());
	
//--- samples
	simple(&qr);
	mark_arc(&qr);
	mark_blockarc(&qr);
	mark_metaball(&qr);
	background(&qr);
	group(&qr);
	markpaint(&qr);
	eye(&qr);
	largest_rect();
	paste_layout(&qr);
	paste(&qr);
	hole(&qr);
	text(&qr);
	
	filter_invert(&qr);
	filter_crayon(&qr);
	filter_circle(&qr);
	filter_grid(&qr);
	filter_voronoi(&qr);
	filter_dots(&qr);
	filter_hexagon(&qr);
	filter_diamond(&qr);
	filter_tile(&qr);
	filter_noise(&qr);
	filter_hydrangea(&qr);
	filter_wave(&qr);
	filter_ball(&qr);
	
	output_svg(&qr);
	output_pdf(&qr);

/////////////////////////////////////////////////////////////////
#ifdef DEBUG
{
	CQR_Encode qr;
	char *data = (char *)"qdr";
	qr.EncodeData(1, 0, true, -1, data, strlen(data));

	Qdr qdr;
	qdr_init(&qdr, qr.m_nSymbleSize, qr.m_byModuleData);
	
	//不正画像
	qdr_bg_image(&qdr, "images/dummy", 80);
	
	qdr_save_png(&qdr, "out/bad.png");
	qdr_close(&qdr);
}
#endif
/////////////////////////////////////////////////////////////////
#ifdef DEBUG
{
	CQR_Encode qr;
	char *data = (char *)"qdr";
	qr.EncodeData(1, 0, true, -1, data, strlen(data));

	Qdr qdr;
	qdr_init(&qdr, qr.m_nSymbleSize, qr.m_byModuleData);
	qdr_group(&qdr);
	qdr_group_palette_rand(&qdr, 0, 200, 0xff);

	//多重フィルター
	qdr_filter_invert(&qdr);
	qdr_filter_hexagon(&qdr, 3);
	//qdr_save_png(&qdr, "out/test.png");
	
	//フィルター後にsvg保存(surface違いのwarnが出る)
	qdr_save_svg(&qdr, "out/test.svg");
	
	qdr_close(&qdr);
}
#endif
/////////////////////////////////////////////////////////////////
#ifdef DEBUG
{
	//test
	CQR_Encode qr;
	qr.EncodeData(1, 0, true, -1, data, strlen(data));

	Qdr qdr;
	qdr_init(&qdr, qr.m_nSymbleSize, qr.m_byModuleData);
	
	qdr_set_msize(&qdr, 16);
	qdr_set_mark(&qdr, QDR_MARKTYPE_BLOCKARC, 0.5);
	//qdr_group(&qdr);
	//qdr_group_palette_rand(&qdr, 0, 200, 0xff);

	qdr_test(&qdr);
	
	qdr_close(&qdr);
}
#endif
/////////////////////////////////////////////////////////////////


#ifdef DEBUG
	dbg_print_alloc_count(stdout);
#endif

	return 0;
}
