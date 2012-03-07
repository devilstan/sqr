#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pikl.h>

#ifdef DEBUG
#include "dbg/dbg.h"
#endif

//=============================================================================
// trim
//=============================================================================
static void trim(char *in)
{
	PKLImage pk;
	
printf("**trim\n");
	pk = pkl_open_file(in);
	pkl_trim(pk, 10, 10, 100, 100);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/trim.jpg");
	pkl_close(pk);
}

//=============================================================================
// rotate
//=============================================================================
static void rotate(char *in)
{
	PKLImage pk;
	PKLColor color;
	
	color = pkl_color_create(0x99, 0xCC, 0xFF, 0);
	
printf("**rotate-->90\n");
	pk = pkl_open_file(in);
	pkl_rotate(pk, 90.0, PKL_SAMPLE_NN, color);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rotate90.jpg");
	pkl_close(pk);
	
printf("**rotate-->180\n");
	pk = pkl_open_file(in);
	pkl_rotate(pk, 180.0, PKL_SAMPLE_NN, color);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rotate180.jpg");
	pkl_close(pk);
	
printf("**rotate-->270\n");
	pk = pkl_open_file(in);
	pkl_rotate(pk, 270.0, PKL_SAMPLE_NN, color);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rotate270.jpg");
	pkl_close(pk);

printf("**rotate-->PKL_SAMPLE_NN(30.0)\n");
	pk = pkl_open_file(in);
	pkl_rotate(pk, 30.0, PKL_SAMPLE_NN, color);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rorate_nn.jpg");
	pkl_close(pk);
	
printf("**rotate-->PKL_SAMPLE_BL(30.0)\n");
	pk = pkl_open_file(in);
	pkl_rotate(pk, 30.0, PKL_SAMPLE_BL, color);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rorate_bl.jpg");
	pkl_close(pk);

printf("**rotate-->PKL_SAMPLE_BL(23.0)\n");
	pk = pkl_open_file(in);
	pkl_rotate(pk, 23.0, PKL_SAMPLE_BL, color);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rorate_bl_23.jpg");
	pkl_close(pk);


printf("**rotate-->PKL_SAMPLE_BC(30.0)\n");
	pk = pkl_open_file(in);
	pkl_rotate(pk, 30.0, PKL_SAMPLE_BC, color);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rorate_bc.jpg");
	pkl_close(pk);
	
printf("**rotate-->PKL_SAMPLE_BC(23.0)\n");
	pk = pkl_open_file(in);
	pkl_rotate(pk, 23.0, PKL_SAMPLE_BC, color);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rorate_bc_23.jpg");
	pkl_close(pk);
	
	
printf("**rotate-->PKL_SAMPLE_PA(30.0)\n");
	pk = pkl_open_file(in);
	pkl_rotate(pk, 30.0, PKL_SAMPLE_PA, color);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rorate_pa.jpg");
	pkl_close(pk);
	
	
	pkl_color_close(color);
}

//=============================================================================
// resize
//=============================================================================
static void resize(char *in)
{
	PKLImage pk;
	
printf("**resize-->PKL_SAMPLE_NN\n");
	pk = pkl_open_file(in);
	pkl_resize(pk, pkl_width(pk)*0.6, pkl_height(pk)*0.6, PKL_SAMPLE_NN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/resize_nn.jpg");
	pkl_close(pk);
	
printf("**resize-->PKL_SAMPLE_BL\n");
	pk = pkl_open_file(in);
	pkl_resize(pk, pkl_width(pk)*0.6, pkl_height(pk)*0.6, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/resize_bl.jpg");
	pkl_close(pk);
	
printf("**resize-->PKL_SAMPLE_BC\n");
	pk = pkl_open_file(in);
	pkl_resize(pk, pkl_width(pk)*0.6, pkl_height(pk)*0.6, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/resize_bc.jpg");
	pkl_close(pk);
	
printf("**resize-->PKL_SAMPLE_PA\n");
	pk = pkl_open_file(in);
	pkl_resize(pk, pkl_width(pk)*0.6, pkl_height(pk)*0.6, PKL_SAMPLE_PA);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/resize_pa.jpg");
	pkl_close(pk);
	
printf("**resize-->PKL_SAMPLE_LZ\n");
	pk = pkl_open_file(in);
	pkl_resize(pk, pkl_width(pk)*0.6, pkl_height(pk)*0.6, PKL_SAMPLE_LZ);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/resize_lz.jpg");
	pkl_close(pk);
}

//=============================================================================
// unsharp
//=============================================================================
static void unsharp(char *in)
{
	PKLImage pk;
	
printf("**unsharp\n");
	pk = pkl_open_file(in);
	pkl_unsharp(pk, 5, 1.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/unsharp.jpg");
	pkl_close(pk);
}

//=============================================================================
// contrast
//=============================================================================
static void contrast(char *in)
{
	PKLImage pk;
	
printf("**contrast\n");
	pk = pkl_open_file(in);
	pkl_contrast(pk, 100);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/contrast.jpg");
	pkl_close(pk);
}

//=============================================================================
// level
//=============================================================================
static void level(char *in)
{
	PKLImage pk;
	
printf("**level\n");
	pk = pkl_open_file(in);
	pkl_level(pk, 10.0, 5.0, 1.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/level.jpg");
	pkl_close(pk);
}

//=============================================================================
// brightness
//=============================================================================
static void brightness(char *in)
{
	PKLImage pk;
	
printf("**brightness\n");
	pk = pkl_open_file(in);
	pkl_brightness(pk, 30);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/brightness.jpg");
	pkl_close(pk);
}

//=============================================================================
// hls
//=============================================================================
static void hls(char *in)
{
	PKLImage pk;
	
printf("**hls\n");
	pk = pkl_open_file(in);
	pkl_hls(pk, 0.3, 0.3, 110.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/hls.jpg");
	pkl_close(pk);
}

//=============================================================================
// gamma
//=============================================================================
static void _gamma(char *in)
{
	PKLImage pk;
	
printf("**gamma\n");
	pk = pkl_open_file(in);
	pkl_gamma(pk, 2.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/gamma.jpg");
	pkl_close(pk);
}

//=============================================================================
// affine
//=============================================================================
static void affine(char *in)
{
	PKLImage pk;
	PKLColor color=pkl_color_create(0x99, 0xCC, 0xFF, 0);
	
printf("**affine-->PKL_SAMPLE_NN\n");
	pk = pkl_open_file(in);
	pkl_affine(pk, PKL_SAMPLE_NN, 500, 500, color, 1.0, 1.0, -30.0, -100, -10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/affine_nn.jpg");
	pkl_close(pk);

printf("**affine-->PKL_SAMPLE_BL\n");
	pk = pkl_open_file(in);
	pkl_affine(pk, PKL_SAMPLE_BL, 500, 500, color, 1.0, 1.0, -30.0, -100, -10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/affine_bl.jpg");
	pkl_close(pk);
	
printf("**affine-->PKL_SAMPLE_BC\n");
	pk = pkl_open_file(in);
	pkl_affine(pk, PKL_SAMPLE_BC, 500, 500, color, 1.0, 1.0, -30.0, -100, -10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/affine_bc.jpg");
	pkl_close(pk);
	
	pkl_color_close(color);
}

//=============================================================================
// xor
//=============================================================================
static void invert(char *in)
{
	PKLImage pk;
	
printf("**xor\n");
	pk = pkl_open_file(in);
	pkl_invert(pk);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/invert.jpg");
	pkl_close(pk);
}

//=============================================================================
// emboss
//=============================================================================
static void emboss(char *in)
{
	PKLImage pk;
	
printf("**emboss(EMBOSS)\n");
	pk = pkl_open_file(in);
	pkl_emboss(pk, PKL_EMBOSS_EMBOSS);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/emboss_EMBOSS.jpg");
	pkl_close(pk);
	
printf("**emboss(LAPLACIAN)\n");
	pk = pkl_open_file(in);
	pkl_emboss(pk, PKL_EMBOSS_LAPLACIAN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/emboss_LAPLACIAN.jpg");
	pkl_close(pk);
	
printf("**emboss(HEAVY)\n");
	pk = pkl_open_file(in);
	pkl_emboss(pk, PKL_EMBOSS_HEAVY);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/emboss_HEAVY.jpg");
	pkl_close(pk);
	
printf("**emboss(LIGHT)\n");
	pk = pkl_open_file(in);
	pkl_emboss(pk, PKL_EMBOSS_LIGHT);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/emboss_LIGHT.jpg");
	pkl_close(pk);
}

//=============================================================================
// focus
//=============================================================================
static void focus(char *in)
{
	PKLImage pk;
	
printf("**focus(DETAIL)\n");
	pk = pkl_open_file(in);
	pkl_focus(pk, PKL_FOCUS_DETAIL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/focus_DETAIL.jpg");
	pkl_close(pk);
	
printf("**focus(EDGES)\n");
	pk = pkl_open_file(in);
	pkl_focus(pk, PKL_FOCUS_EDGES);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/focus_EDGES.jpg");
	pkl_close(pk);
	
printf("**focus(FOCUS)\n");
	pk = pkl_open_file(in);
	pkl_focus(pk, PKL_FOCUS_FOCUS);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/focus_FOCUS.jpg");
	pkl_close(pk);
	
printf("**focus(BUMP)\n");
	pk = pkl_open_file(in);
	pkl_focus(pk, PKL_FOCUS_BUMP);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/focus_BUMP.jpg");
	pkl_close(pk);
}

//=============================================================================
// sepia
//=============================================================================
static void sepia(char *in)
{
	PKLImage pk;
	
printf("**sepia1\n");
	pk = pkl_open_file(in);
	pkl_sepia(pk, 1.0, 0.8, 0.6);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sepia1.jpg");
	pkl_close(pk);
	
printf("**sepia2\n");
	pk = pkl_open_file(in);
	pkl_sepia(pk, 1.0, 0.9, 0.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sepia2.jpg");
	pkl_close(pk);
	
printf("**sepia3\n");
	pk = pkl_open_file(in);
	pkl_sepia(pk, 1.0, 0.0, 0.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sepia3.jpg");
	pkl_close(pk);
}

//=============================================================================
// oilpaint
//=============================================================================
static void oilpaint(char *in)
{
	PKLImage pk;
	
printf("**oilpaint1\n");
	pk = pkl_open_file(in);
	pkl_oilpaint(pk, 2);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/oilpaint1.jpg");
	pkl_close(pk);
	
printf("**oilpaint2\n");
	pk = pkl_open_file(in);
	pkl_oilpaint(pk, 3);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/oilpaint2.jpg");
	pkl_close(pk);
	
printf("**oilpaint3\n");
	pk = pkl_open_file(in);
	pkl_oilpaint(pk, 4);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/oilpaint3.jpg");
	pkl_close(pk);
}

//=============================================================================
// composite
//=============================================================================
static void composite(char *img1, char *img2)
{
	PKLImage pk1, pk2;
	PKLColor color = pkl_color_create(0xff, 0xff, 0xff, 0);
	
printf("**composite\n");
	pk1 = pkl_open_file(img1);
	pk2 = pkl_open_file(img2);
	
	pkl_composite(pk1, pk2, -50, -50, NULL);
	pkl_composite(pk1, pk2, 100,  80, NULL);
	pkl_composite(pk1, pk2, 250, 250, NULL);

	pkl_save_file(pk1, PKL_FORMAT_JPEG, "out/composite.jpg");
	pkl_close(pk1);
	pkl_close(pk2);

printf("**composite-alpha\n");
	pk1 = pkl_open_file(img1);
	pk2 = pkl_open_file(img2);
	pkl_composite(pk1, pk2, -50, -50, color);
	pkl_composite(pk1, pk2, 100,  80, color);
	pkl_composite(pk1, pk2, 250, 250, color);
	pkl_save_file(pk1, PKL_FORMAT_JPEG, "out/composite-alpha.jpg");
	pkl_close(pk1);
	pkl_close(pk2);
	
	pkl_color_close(color);
}

//=============================================================================
// blur
//=============================================================================
static void blur(char *img)
{
	PKLImage pk;
	
printf("**blur\n");
	pk = pkl_open_file(img);
	pkl_blur(pk, 2);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/blur.jpg");
	pkl_close(pk);
}

//=============================================================================
// gaussblur
//=============================================================================
static void gaussblur(char *img)
{
	PKLImage pk;
	
printf("**gaussblur\n");
	pk = pkl_open_file(img);
	pkl_gaussblur(pk, 5.5);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/gaussblur.jpg");
	pkl_close(pk);
}

//=============================================================================
// illust
//=============================================================================
static void illust(char *img)
{
	PKLImage pk;
	
printf("**illust\n");
	pk = pkl_open_file(img);
	pkl_illust(pk, 5, 18, 60.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/illust.jpg");
	pkl_close(pk);
}

//=============================================================================
// color_emboss
// mil: ハイライト(1..400)
// env: 環境光(1..100)
//=============================================================================
static void rock(char *img)
{
	PKLImage pk;
	
printf("**rock1\n");
	pk = pkl_open_file(img);
	pkl_rock(pk, 300, 70);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rock1.jpg");
	pkl_close(pk);
	
printf("**rock2\n");
	pk = pkl_open_file(img);
	pkl_rock(pk, 100, 50);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rock2.jpg");
	pkl_close(pk);
	
printf("**rock+noisecut\n");
	pk = pkl_open_file(img);
	pkl_rock(pk, 100, 70);
	pkl_noisecut(pk);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rock_noisecut.jpg");
	pkl_close(pk);
}

//=============================================================================
// mosaic
//=============================================================================
static void mosaic(char *img)
{
	PKLImage pk;
	
printf("**mosaic\n");
	pk = pkl_open_file(img);
	pkl_mosaic(pk, 10, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/mosaic.jpg");
	pkl_close(pk);
	
printf("**circle-1\n");
	pk = pkl_open_file(img);
	pkl_circle(pk, 5, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/circle1.jpg");
	pkl_close(pk);

printf("**circle-2\n");
	pk = pkl_open_file(img);
	pkl_circle(pk, 5, 2);
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/circle2.bmp");
	pkl_close(pk);


//printf("**pixelate\n");
//	pk = pkl_open_file(img);
//	pkl_pixelate(pk, 1);
//	pkl_save_file(pk, "out/pixelate.jpg", PKL_FORMAT_JPEG);
//	pkl_close(pk);
}

//=============================================================================
// noise
//=============================================================================
static void noise(char *img)
{
	PKLImage pk;
	
printf("**noise\n");
	pk = pkl_open_file(img);
	pkl_noise(pk, 1);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/noise.jpg");
	pkl_close(pk);
}

//=============================================================================
// posterize
//=============================================================================
static void posterize(char *img)
{
	PKLImage pk;
	
printf("**posterize\n");
	pk = pkl_open_file(img);
	pkl_posterize(pk, 8);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/posterize.jpg");
	pkl_close(pk);
}

//=============================================================================
// cutcolor
//=============================================================================
static void cutcolor(char *img)
{
	PKLImage pk;
	
printf("**pkl_cutcolor\n");
	pk = pkl_open_file(img);
	pkl_cutcolor(pk, 0xf0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/pkl_cutcolor.jpg");
	pkl_close(pk);
}

//=============================================================================
// noisecut
//=============================================================================
static void noisecut(char *img)
{
	PKLImage pk;
	
printf("**noisecut\n");
	pk = pkl_open_file(img);
	pkl_noisecut(pk);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/noisecut.jpg");
	pkl_close(pk);
}

//=============================================================================
// edgepaint
//=============================================================================
static void edgepaint(char *img)
{
	PKLImage pk;
	
printf("**edgepaint1\n");
	pk = pkl_open_file(img);
	pkl_edgepaint(pk, 1);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/edgepaint1.jpg");
	pkl_close(pk);
	
printf("**edgepaint2\n");
	pk = pkl_open_file(img);
	pkl_edgepaint(pk, 2);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/edgepaint2.jpg");
	pkl_close(pk);
}

//=============================================================================
// edgeposter
//=============================================================================
static void edgeposter(char *img)
{
	PKLImage pk;
	
printf("**edgeposter1\n");
	pk = pkl_open_file(img);
	pkl_edgeposter(pk, 5, 1.5, 1.5, 1.0, 1);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/edgeposter1.jpg");
	pkl_close(pk);
	
printf("**edgeposter2\n");
	pk = pkl_open_file(img);
	pkl_edgeposter(pk, 5, 1.5, 1.5, 1.0, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/edgeposter2.jpg");
	pkl_close(pk);
	
printf("**edgeposter3\n");
	pk = pkl_open_file(img);
	pkl_edgeposter(pk, 5, 1.5, 1.5, 1.0, 2);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/edgeposter3.jpg");
	pkl_close(pk);
}

//=============================================================================
// tileslit
//=============================================================================
static void tileslit(char *img)
{
	PKLImage pk;

printf("**tileslit\n");
	pk = pkl_open_file(img);
	pkl_tileslit(pk, 20, 2);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/tileslit.jpg");
	pkl_close(pk);
}

//=============================================================================
// vtr
//=============================================================================
static void vtr(char *img)
{
	PKLImage pk;
	
printf("**vtr\n");
	pk = pkl_open_file(img);
	pkl_vtr(pk, 2, 5, 100);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/vtr.jpg");
	pkl_close(pk);
}

//=============================================================================
// grid
//=============================================================================
static void grid(char *img)
{
	PKLImage pk;
	
printf("**grid\n");
	pk = pkl_open_file(img);
	pkl_grid(pk, 10, 10, 30);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/grid.jpg");
	pkl_close(pk);
}

//=============================================================================
// rblur
//=============================================================================
static void rblur(char *img)
{
	PKLImage pk;
	
printf("**rblur1\n");
	pk = pkl_open_file(img);
	pkl_rblur(pk, -1, -1, 10.0, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rblur1.jpg");
	pkl_close(pk);
	
printf("**rblur2\n");
	pk = pkl_open_file(img);
	pkl_rblur(pk, -1, -1, 30.0, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rblur2.jpg");
	pkl_close(pk);
	
printf("**rblur3\n");
	pk = pkl_open_file(img);
	pkl_rblur(pk, 0, 0, 30.0, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rblur3.jpg");
	pkl_close(pk);
}

//=============================================================================
// ablur
//=============================================================================
static void ablur(char *img)
{
	PKLImage pk;
	
printf("**ablur1\n");
	pk = pkl_open_file(img);
	pkl_ablur(pk, -1, -1, 10.0, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/ablur1.jpg");
	pkl_close(pk);
	
printf("**ablur2\n");
	pk = pkl_open_file(img);
	pkl_ablur(pk, -1, -1, 30.0, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/ablur2.jpg");
	pkl_close(pk);
	
printf("**ablur3\n");
	pk = pkl_open_file(img);
	pkl_ablur(pk, 0, 0, 30.0, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/ablur3.jpg");
	pkl_close(pk);
}

//=============================================================================
// wblur
//=============================================================================
static void wblur(char *img)
{
	PKLImage pk;
	
printf("**wblur1\n");
	pk = pkl_open_file(img);
	pkl_wblur(pk, -1, -1, 10.0, 10, 30.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/wblur1.jpg");
	pkl_close(pk);
	
printf("**wblur2\n");
	pk = pkl_open_file(img);
	pkl_wblur(pk, -1, -1, 30.0, 10, 60.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/wblur2.jpg");
	pkl_close(pk);
	
printf("**wblur3\n");
	pk = pkl_open_file(img);
	pkl_wblur(pk, 0, 0, 30.0, 10, 60.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/wblur3.jpg");
	pkl_close(pk);
}

//=============================================================================
// mblur
//=============================================================================
static void mblur(char *img)
{
	PKLImage pk;
	
printf("**mblur1\n");
	pk = pkl_open_file(img);
	pkl_mblur(pk, 30.0, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/mblur1.jpg");
	pkl_close(pk);
	
printf("**mblur2\n");
	pk = pkl_open_file(img);
	pkl_mblur(pk, 30.0, 20);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/mblur2.jpg");
	pkl_close(pk);
	
printf("**mblur3\n");
	pk = pkl_open_file(img);
	pkl_mblur(pk, 60.0, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/mblur3.jpg");
	pkl_close(pk);
}

//=============================================================================
// kuwahara
//=============================================================================
static void kuwahara(char *img)
{
	PKLImage pk;
	
printf("**kuwahara\n");
	pk = pkl_open_file(img);
	pkl_kuwahara(pk, 3);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/kuwahara.jpg");
	pkl_close(pk);
}

//=============================================================================
// hexagon
//=============================================================================
static void pattern(char *img)
{
	PKLImage pk;

printf("**pattern_hexagon_line\n");
	pk = pkl_open_file(img);
	pkl_pattern(pk, 5, 15, PKL_PATTERN_HEXAGON, PKL_PAINT_LINE);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/pattern_hexagon_line.jpg");
	pkl_close(pk);
	
printf("**pattern_hexagon_ave\n");
	pk = pkl_open_file(img);
	pkl_pattern(pk, 5, 15, PKL_PATTERN_HEXAGON, PKL_PAINT_AVE);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/pattern_hexagon_ave.jpg");
	pkl_close(pk);
	
printf("**pattern_diamond_line\n");
	pk = pkl_open_file(img);
	pkl_pattern(pk, 10, 10, PKL_PATTERN_DIAMOND, PKL_PAINT_LINE);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/pattern_diamond_line.jpg");
	pkl_close(pk);
	
printf("**pattern_diamond_ave\n");
	pk = pkl_open_file(img);
	pkl_pattern(pk, 10, 10, PKL_PATTERN_DIAMOND, PKL_PAINT_AVE);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/pattern_diamond_ave.jpg");
	pkl_close(pk);
	
printf("**pattern_circle_line\n");
	pk = pkl_open_file(img);
	pkl_pattern(pk, 8, 24, PKL_PATTERN_CIRCLE, PKL_PAINT_LINE);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/pattern_circle_line.jpg");
	pkl_close(pk);

printf("**pattern_circle_ave\n");
	pk = pkl_open_file(img);
	pkl_pattern(pk, 8, 24, PKL_PATTERN_CIRCLE, PKL_PAINT_AVE);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/pattern_circle_ave.jpg");
	pkl_close(pk);
	
printf("**pattern_brick_line\n");
	pk = pkl_open_file(img);
	pkl_pattern(pk, 10, 30, PKL_PATTERN_BRICK, PKL_PAINT_LINE);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/pattern_brick_line.jpg");
	pkl_close(pk);
	
printf("**pattern_brick_ave\n");
	pk = pkl_open_file(img);
	pkl_pattern(pk, 10, 30, PKL_PATTERN_BRICK, PKL_PAINT_AVE);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/pattern_brick_ave.jpg");
	pkl_close(pk);
}

//=============================================================================
// alphablend
//=============================================================================
static void alphablend(char *img1, char *img2, char *img3)
{
	PKLImage pk1, pk2;
	
printf("**alphablend\n");
	pk1 = pkl_open_file(img1);
	pk2 = pkl_open_file(img3);
	
	pkl_alphablend(pk1, pk2, -100, 50,  50);
	pkl_alphablend(pk1, pk2,  100, 50, 100);
	pkl_alphablend(pk1, pk2,  300, 50, 150);
	
	//pkl_alphablend(pk1, pk2,  -5, -5, 150);
	//pkl_alphablend(pk1, pk2, 100, 80,   0);
	//pkl_alphablend(pk1, pk2, 150, 80,  50);
	//pkl_alphablend(pk1, pk2, 200, 80,  80);
	//pkl_alphablend(pk1, pk2, 250, 80, 100);
	//pkl_alphablend(pk1, pk2, 300, 80, 150);
	//pkl_alphablend(pk1, pk2, 350, 80, 200);
	//pkl_alphablend(pk1, pk2, 400, 80, 220);
	//pkl_alphablend(pk1, pk2, 450, 80, 255);
	//pkl_alphablend(pk1, pk2, 480, 80,  50);
	
	pkl_save_file(pk1, PKL_FORMAT_JPEG, "out/alphablend.jpg");
	pkl_close(pk1);
	pkl_close(pk2);
	
	
printf("**alphablend(round)\n");
	pk1 = pkl_open_file(img1);
	pk2 = pkl_open_file(img3);
	
	//pkl_alphablend(pk1, pk2, -50, -50, -1);
	pkl_alphablend(pk1, pk2,  50,  50, -1);
	//pkl_alphablend(pk1, pk2, 250, 250, -1);
	
	pkl_save_file(pk1, PKL_FORMAT_JPEG, "out/alphablend-round.jpg");
	pkl_close(pk1);
	pkl_close(pk2);
}

////=============================================================================
//// alphablend2
////=============================================================================
//static void alphablend2(char *img1, char *img2)
//{
//	PKLImage pk1, pk2;
//	
//printf("**alphablend2\n");
//	pk1 = pkl_open_file(img1);
//	pk2 = pkl_open_file(img2);
//	
//	pkl_alphablend2(pk1, pk2, -50, -50);
//	pkl_alphablend2(pk1, pk2,  50,  50);
//	pkl_alphablend2(pk1, pk2, 250, 250);
//	
//	pkl_save_file(pk1, "out/alphablend2.jpg", PKL_FORMAT_JPEG);
//	pkl_close(pk1);
//	pkl_close(pk2);
//}

//=============================================================================
// shadowframe
//=============================================================================
static void shadowframe(char *img)
{
	PKLImage pk;
	PKLColor color1, color2;
	
	color1=pkl_color_create(0x60, 0x60, 0x60, 0);
	color2=pkl_color_create(166, 202, 240, 0);
	
printf("**shadowframe1\n");
	pk = pkl_open_file(img);
	pkl_shadowframe(pk, 10, color2, color1);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/shadowframe1.jpg");
	pkl_close(pk);
	
	pkl_color_close(color1);
	pkl_color_close(color2);
	
	
printf("**shadowframe2\n");
	pk = pkl_open_file(img);
	pkl_shadowframe(pk, 10, NULL, NULL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/shadowframe2.jpg");
	pkl_close(pk);
}

//=============================================================================
// edge
//=============================================================================
static void edge(char *img)
{
	PKLImage pk;
	
printf("**edge1\n");
	pk = pkl_open_file(img);
	pkl_edge(pk, 5);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/edge1.jpg");
	pkl_close(pk);
	
printf("**edge2\n");
	pk = pkl_open_file(img);
	pkl_edge(pk, 10);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/edge2.jpg");
	pkl_close(pk);
	
printf("**edge3\n");
	pk = pkl_open_file(img);
	pkl_edge(pk, 30);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/edge3.jpg");
	pkl_close(pk);
}

//=============================================================================
// swirl
//=============================================================================
static void swirl(char *img)
{
	PKLImage pk;
	PKLColor color = pkl_color_create(0xfc, 0x99, 0xaa, 0x00);
	
printf("**swirl1\n");
	pk = pkl_open_file(img);
	pkl_swirl(pk, 0.002, -1, -1, color, PKL_SAMPLE_NN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swirl1_nn.jpg");
	pkl_close(pk);

printf("**swirl1\n");
	pk = pkl_open_file(img);
	pkl_swirl(pk, 0.002, -1, -1, color, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swirl1_bl.jpg");
	pkl_close(pk);

printf("**swirl1\n");
	pk = pkl_open_file(img);
	pkl_swirl(pk, 0.002, -1, -1, color, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swirl1_bc.jpg");
	pkl_close(pk);
	
printf("**swirl2\n");
	pk = pkl_open_file(img);
	pkl_swirl(pk, 0.005, -1, -1, color, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swirl2.jpg");
	pkl_close(pk);
	
printf("**swirl3\n");
	pk = pkl_open_file(img);
	pkl_swirl(pk, 0.009, -1, -1, color, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swirl3.jpg");
	pkl_close(pk);
	
	pkl_color_close(color);
}

//=============================================================================
// dots
//=============================================================================
static void dots(char *img)
{
	PKLImage pk;
	PKLColor color = pkl_color_create(0x99, 0x55, 0xaa, 0);
	PKLColor back = pkl_color_create(0x50, 0xee, 0x50, 0);
	
printf("**dots1\n");
	pk = pkl_open_file(img);
	pkl_dots(pk, 10, NULL, NULL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/dots1.jpg");
	pkl_close(pk);
	
printf("**dots2\n");
	pk = pkl_open_file(img);
	pkl_dots(pk, 8, color, back);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/dots2.jpg");
	pkl_close(pk);
	
printf("**dots3\n");
	pk = pkl_open_file(img);
	pkl_dots(pk, 8, NULL, NULL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/dots3.jpg");
	pkl_close(pk);
	
	pkl_color_close(color);
	pkl_color_close(back);
}

//=============================================================================
// wave
//=============================================================================
static void wave(char *img)
{
	PKLImage pk;
	
printf("**wave1\n");
	pk = pkl_open_file(img);
	pkl_wave(pk, PKL_WAVE_HORZ, 15.0, 4.3, NULL, PKL_SAMPLE_NN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/wave1_nn.jpg");
	pkl_close(pk);
	
printf("**wave1\n");
	pk = pkl_open_file(img);
	pkl_wave(pk, PKL_WAVE_HORZ, 15.0, 4.3, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/wave1_bl.jpg");
	pkl_close(pk);
	
printf("**wave1\n");
	pk = pkl_open_file(img);
	pkl_wave(pk, PKL_WAVE_HORZ, 15.0, 4.3, NULL, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/wave1_bc.jpg");
	pkl_close(pk);
	

printf("**wave2\n");
	pk = pkl_open_file(img);
	pkl_wave(pk, PKL_WAVE_VERT, 15.0, 4.3, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/wave2.jpg");
	pkl_close(pk);

printf("**wave3\n");
	pk = pkl_open_file(img);
	pkl_wave(pk, PKL_WAVE_BOTH, 15.0, 4.3, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/wave3.jpg");
	pkl_close(pk);

printf("**wave4\n");
	pk = pkl_open_file(img);
	pkl_wave(pk, PKL_WAVE_HORZ, 25.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/wave4.jpg");
	pkl_close(pk);
}

//=============================================================================
// dither
//=============================================================================
static void dither(char *img)
{
	PKLImage pk;
	
printf("**dither1\n");
	pk = pkl_open_file(img);
	pkl_dither(pk, PKL_DITHER_FLOYDSTEINBERG);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/dither1.jpg");
	pkl_close(pk);
	
printf("**dither2\n");
	pk = pkl_open_file(img);
	pkl_dither(pk, PKL_DITHER_STUCCI);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/dither2.jpg");
	pkl_close(pk);
	
printf("**dither3\n");
	pk = pkl_open_file(img);
	pkl_dither(pk, PKL_DITHER_SIERRA);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/dither3.jpg");
	pkl_close(pk);
	
printf("**dither4\n");
	pk = pkl_open_file(img);
	pkl_dither(pk, PKL_DITHER_JAJUNI);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/dither4.jpg");
	pkl_close(pk);
}

////=============================================================================
//// mediancut
////=============================================================================
//static void mediancut(char *img)
//{
//	PKLImage pk;
//	
//printf("**mediancut1\n");
//	pk = pkl_open_file(img);
//	pkl_mediancut(pk, 256, 0);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/mediancut1.png");
//	pkl_close(pk);
//	
//printf("**mediancut2\n");
//	pk = pkl_open_file(img);
//	pkl_mediancut(pk, 256, 1);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/mediancut2.png");
//	pkl_close(pk);
//
//printf("**mediancut3\n");
//	pk = pkl_open_file(img);
//	pkl_mediancut(pk, 64, 0);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/mediancut3.png");
//	pkl_close(pk);
//
//printf("**mediancut4\n");
//	pk = pkl_open_file(img);
//	pkl_mediancut(pk, 64, 1);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/mediancut4.png");
//	pkl_close(pk);
//
//}

////=============================================================================
//// neuquant
////=============================================================================
//static void neuquant(char *img)
//{
//	PKLImage pk;
//	
//printf("**neuquant1\n");
//	pk = pkl_open_file(img);
//	pkl_neuquant(pk, 256, 1);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/neuquant1.png");
//	pkl_close(pk);
//	
//printf("**neuquant2\n");
//	pk = pkl_open_file(img);
//	pkl_neuquant(pk, 256, 30);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/neuquant2.png");
//	pkl_close(pk);
//
//printf("**neuquant3\n");
//	pk = pkl_open_file(img);
//	pkl_neuquant(pk, 64, 1);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/neuquant3.png");
//	pkl_close(pk);
//	
//printf("**neuquant4\n");
//	pk = pkl_open_file(img);
//	pkl_neuquant(pk, 64, 30);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/neuquant4.png");
//	pkl_close(pk);
//}
//
////=============================================================================
//// wu
////=============================================================================
//static void wu(char *img)
//{
//	PKLImage pk;
//	
//printf("**wu1\n");
//	pk = pkl_open_file(img);
//	pkl_wu(pk, 256);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/wu1.png");
//	pkl_close(pk);
//	
//printf("**wu2\n");
//	pk = pkl_open_file(img);
//	pkl_wu(pk, 64);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/wu2.png");
//	pkl_close(pk);
//}

////=============================================================================
//// dl
////=============================================================================
//static void dl(char *img)
//{
//	PKLImage pk;
//	
//printf("**dl1\n");
//	pk = pkl_open_file(img);
//	pkl_dl(pk, 256, 0);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/dl1.png");
//	pkl_close(pk);
//	
//printf("**dl2\n");
//	pk = pkl_open_file(img);
//	pkl_dl(pk, 256, 1);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/dl2.png");
//	pkl_close(pk);
//	
//printf("**dl3\n");
//	pk = pkl_open_file(img);
//	pkl_dl(pk, 64, 0);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/dl3.png");
//	pkl_close(pk);
//	
//printf("**dl4\n");
//	pk = pkl_open_file(img);
//	pkl_dl(pk, 64, 1);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/dl4.png");
//	pkl_close(pk);
//}
//

////=============================================================================
//// cc
////=============================================================================
//static void cc(char *img)
//{
//	PKLImage pk;
//	
//printf("**cc1\n");
//	pk = pkl_open_file(img);
//	pkl_cc(pk, 256);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/cc1.png");
//	pkl_close(pk);
//	
//printf("**cc2\n");
//	pk = pkl_open_file(img);
//	pkl_cc(pk, 64);
//	pkl_save_file(pk, PKL_FORMAT_PNG, "out/cc2.png");
//	pkl_close(pk);
//}

//=============================================================================
// splitframe
//=============================================================================
static void splitframe(char *img)
{
	PKLImage pk;
	PKLColor color = pkl_color_create(0x99,0xcc,0x66,0);
	
printf("**splitframe1\n");
	pk = pkl_open_file(img);
	pkl_splitframe(pk, color, 5, 5, 20, 5);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/splitframe1.jpg");
	pkl_close(pk);
	
printf("**splitframe2\n");
	pk = pkl_open_file(img);
	pkl_splitframe(pk, color, 10, 10, 0, 5);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/splitframe2.jpg");
	pkl_close(pk);
	
printf("**splitframe3\n");
	pk = pkl_open_file(img);
	pkl_splitframe(pk, color, 10, 10, 0, 1);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/splitframe3.jpg");
	pkl_close(pk);
	
	pkl_color_close(color);
}
//=============================================================================
// voronoi_zone
//=============================================================================
static void voronoi_zone(char *img)
{
	PKLImage pk;
	PKLColor color=pkl_color_create(0,0,0,0);
	
printf("**voronoi_zone-1\n");
	pk = pkl_open_file(img);
	pkl_voronoi_zone(pk, 10, NULL, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_zone-1.jpg");
	pkl_close(pk);

printf("**voronoi_type1-2\n");
	pk = pkl_open_file(img);
	pkl_voronoi_zone(pk, 10, color, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_zone-2.jpg");
	pkl_close(pk);
	
printf("**voronoi_type1-3\n");
	pk = pkl_open_file(img);
	pkl_voronoi_zone(pk, 10, color, 1);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_zone-3.jpg");
	pkl_close(pk);
	
printf("**voronoi_type1-4\n");
	pk = pkl_open_file(img);
	pkl_voronoi_zone(pk, 5, NULL, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_zone-4.jpg");
	pkl_close(pk);
	
printf("**voronoi_type1-5\n");
	pk = pkl_open_file(img);
	pkl_voronoi_zone(pk, 20, NULL, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_zone-5.jpg");
	pkl_close(pk);
	
	pkl_color_close(color);
}
//=============================================================================
// voronoi_count
//=============================================================================
static void voronoi_count(char *img)
{
	PKLImage pk;
	PKLColor color=pkl_color_create(0,0,0,0);
	
printf("**voronoi_count-1\n");
	pk = pkl_open_file(img);
	pkl_voronoi_count(pk, 1000, NULL, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_count-1.jpg");
	pkl_close(pk);

printf("**voronoi_count-2\n");
	pk = pkl_open_file(img);
	pkl_voronoi_count(pk, 1000, color, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_count-2.jpg");
	pkl_close(pk);
	
printf("**voronoi_count-3\n");
	pk = pkl_open_file(img);
	pkl_voronoi_count(pk, 1000, color, 1);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_count-3.jpg");
	pkl_close(pk);
	
printf("**voronoi_count-4\n");
	pk = pkl_open_file(img);
	pkl_voronoi_count(pk, 2000, NULL, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_count-4.jpg");
	pkl_close(pk);
	
printf("**voronoi_count-5\n");
	pk = pkl_open_file(img);
	pkl_voronoi_count(pk, 3000, NULL, 0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/voronoi_count-5.jpg");
	pkl_close(pk);
	
	pkl_color_close(color);
}
//=============================================================================
// colordither
//=============================================================================
static void colordither(char *img)
{
	PKLImage pk;
	
printf("**colordither1\n");
	pk = pkl_open_file(img);
	pkl_colordither(pk, -50);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/colordither1.jpg");
	pkl_close(pk);
	
printf("**colordither2\n");
	pk = pkl_open_file(img);
	pkl_colordither(pk, 50);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/colordither2.jpg");
	pkl_close(pk);
	
printf("**colordither3\n");
	pk = pkl_open_file(img);
	pkl_colordither(pk, 100);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/colordither3.jpg");
	pkl_close(pk);
	
printf("**colordither4\n");
	pk = pkl_open_file(img);
	pkl_colordither(pk, 200);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/colordither4.jpg");
	pkl_close(pk);
}

//=============================================================================
// sobelpaint
//=============================================================================
static void sobelpaint(char *img)
{
	PKLImage pk;
	
printf("**sobelpaint1\n");
	pk = pkl_open_file(img);
	pkl_sobelpaint(pk, 1, 0.0, 1.0, 0.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sobelpaint1.jpg");
	pkl_close(pk);
	
printf("**sobelpaint2\n");
	pk = pkl_open_file(img);
	pkl_sobelpaint(pk, 2, 0.5, 1.0, 0.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sobelpaint2.jpg");
	pkl_close(pk);
	
printf("**sobelpaint3\n");
	pk = pkl_open_file(img);
	pkl_sobelpaint(pk, 1, 0.0, 1.0, 0.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sobelpaint3.jpg");
	pkl_close(pk);
}

//=============================================================================
// info
//=============================================================================
static void info(char *img)
{
	PKLImage pk;
	
	pk = pkl_open_file(img);
	printf("%s", pkl_format(pk)==PKL_FORMAT_JPEG ? "JPEG"
			   : pkl_format(pk)==PKL_FORMAT_PNG ? "PNG"
			   : pkl_format(pk)==PKL_FORMAT_BITMAP ? "BITMAP" : "ERROR");
	printf("(%dx%d):%d\n", pkl_width(pk), pkl_height(pk), pkl_count(pk));
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/info.jpg");
	pkl_close(pk);
}

//=============================================================================
// pixel_access
//=============================================================================
static void pixel_access(char *img)
{
	PKLImage pk;
	
	pk = pkl_open_file(img);
	printf(" GRAY=%X\n", pkl_get_color(pk, 0, 0, PKL_GRAY));
	printf(" RGB=%X %X %X\n",
				pkl_get_color(pk, 0, 0, PKL_RED),
				pkl_get_color(pk, 0, 0, PKL_GREEN),
				pkl_get_color(pk, 0, 0, PKL_BLUE));
	printf(" CMYK=%X %X %X %X\n",
				pkl_get_color(pk, 0, 0, PKL_CYAN),
				pkl_get_color(pk, 0, 0, PKL_MAGENTA),
				pkl_get_color(pk, 0, 0, PKL_YELLOW),
				pkl_get_color(pk, 0, 0, PKL_BLACK));
	pkl_close(pk);
}

//=============================================================================
// randomshadow
//=============================================================================
static void randomshadow(char *img)
{
	PKLImage pk, canvas;
	PKLColor c1,c2,c3,c4,c5;
	
	c1=pkl_color_create(100,100,100,0);
	c2=pkl_color_create(  0,  0,255,0);
	c3=pkl_color_create(  0,255,  0,0);
	c4=pkl_color_create(255,  0,  0,0);
	c5=pkl_color_create(  0,  0,  0,0);

printf("**canvas & randomshadow\n");
	canvas = pkl_canvas(500, 500, PKL_RGB, NULL);
	//pkl_save_file(canvas, PKL_FORMAT_JPEG, "out/canvas.jpg");
	
	pk = pkl_open_file(img);
	pkl_randomshadow(canvas, pk, 200, 200, 10, c1);
	pkl_randomshadow(canvas, pk, 150, 150, 10, c2);
	pkl_randomshadow(canvas, pk, 100, 100, 10, c3);
	pkl_randomshadow(canvas, pk,  50,  50, 10, c4);
	pkl_randomshadow(canvas, pk,   0,   0, 10, c5);
	pkl_close(pk);
	
	pkl_save_file(canvas, PKL_FORMAT_JPEG, "out/randomshadow.jpg");
	pkl_close(canvas);
	
	pkl_color_close(c1);
	pkl_color_close(c2);
	pkl_color_close(c3);
	pkl_color_close(c4);
	pkl_color_close(c5);
}

//=============================================================================
// vignette
//=============================================================================
static void vignette(char *img)
{
	PKLImage pk;
	PKLColor c;
	
printf("**vignette\n");
	c=pkl_color_create(0, 0, 10, 0);
	pk = pkl_open_file(img);
	pkl_vignette(pk, c, 0, 250, 10, 40);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/vignette.jpg");
	pkl_color_close(c);
	pkl_close(pk);
}

//=================================================================================
// vaseline
//=================================================================================
static void vaseline(char *img)
{
	PKLImage pk;
	
printf("**vaseline\n");
	pk = pkl_open_file(img);
	//pkl_vaseline(pk, 600);
	pkl_vaseline(pk, 200, 30, 50);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/vaseline.jpg");
	pkl_close(pk);
}

//=============================================================================
// film
//=============================================================================
static void film(char *img)
{
	PKLImage pk;
	PKLColor c;
	
printf("**film\n");
	c=pkl_color_create(100, 50, 200, 0);
	pk = pkl_open_file(img);
	pkl_film(pk, c, 60);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/film.jpg");
	pkl_color_close(c);
	pkl_close(pk);
}

//=============================================================================
// vv
//=============================================================================
static void vv(char *img)
{
	PKLImage pk;
	PKLColor c;
	
printf("**vv\n");
	c=pkl_color_create(50, 100, 130, 0);
	pk = pkl_open_file(img);
	pkl_vv(pk, c, 0, 200, 100, 20);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/vv.jpg");
	pkl_color_close(c);
	pkl_close(pk);
}

////=============================================================================
//// spotlight
////=============================================================================
//static void spotlight(char *img)
//{
//	PKLImage pk;
//	PKLColor c;
//	
//printf("**spotlight\n");
//	c=pkl_color_create(0, 0, 0, 0);
//	pk = pkl_open_file(img);
//	pkl_spotlight(pk, 250, c, 80);
//	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/spotlight.jpg");
//	pkl_color_close(c);
//	pkl_close(pk);
//}

//=============================================================================
// tile
//=============================================================================
static void tile(char *img)
{
	PKLImage pk;
	
printf("**tile_rect\n");
	pk = pkl_open_file(img);
	pkl_tile(pk, PKL_TILE_RECT, 10, 10, 32);
	pkl_save_file(pk, PKL_FORMAT_PNG, "out/tile_rect.png");
	pkl_close(pk);
	
printf("**tile_hexagon\n");
	pk = pkl_open_file(img);
	pkl_tile(pk, PKL_TILE_HEXAGON, 5, 15, 32);
	pkl_save_file(pk, PKL_FORMAT_PNG, "out/tile_hexagon.png");
	pkl_close(pk);

printf("**tile_brick\n");
	pk = pkl_open_file(img);
	pkl_tile(pk, PKL_TILE_BRICK, 15, 15, 32);
	pkl_save_file(pk, PKL_FORMAT_PNG, "out/tile_brick.png");
	pkl_close(pk);

printf("**tile_diamond\n");
	pk = pkl_open_file(img);
	pkl_tile(pk, PKL_TILE_DIAMOND, 5, 10, 32);
	pkl_save_file(pk, PKL_FORMAT_PNG, "out/tile_diamond.png");
	pkl_close(pk);
	
printf("**tile_circle\n");
	pk = pkl_open_file(img);
	pkl_tile(pk, PKL_TILE_CIRCLE, 5, 20, 32);
	pkl_save_file(pk, PKL_FORMAT_PNG, "out/tile_circle.png");
	pkl_close(pk);
}

//=============================================================================
// contour
//=============================================================================
static void contour(char *img)
{
	PKLImage pk;
	
printf("**contour\n");
	pk = pkl_open_file(img);
	pkl_contour(pk);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/contour.jpg");
	pkl_close(pk);
}

//=============================================================================
// rinkaku
//=============================================================================
static void rinkaku(char *img)
{
	PKLImage pk;
	
printf("**rinkaku\n");
	pk = pkl_open_file(img);
	pkl_rinkaku(pk, 30.0);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rinkaku.jpg");
	pkl_close(pk);
}

//=============================================================================
// crayon
//=============================================================================
static void crayon(char *img)
{
	PKLImage pk;
	
printf("**crayon1\n");
	pk = pkl_open_file(img);
	pkl_crayon(pk, 30.0, 3, 2, 65);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/crayon1.jpg");
	pkl_close(pk);
	
printf("**crayon2\n");
	pk = pkl_open_file(img);
	pkl_crayon(pk, 10.0, 3, 2, 80);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/crayon2.jpg");
	pkl_close(pk);
	
}

//=============================================================================
// emboss2
//=============================================================================
static void emboss2(char *img)
{
	PKLImage pk;
	int mask9[3*3] =
			{ 0,  0, -1,
			  0,  0,  0,
			  1,  0,  0 };

	int mask25[5*5] =
			{ -1, -2, -3, -2, -1,
			  -2, -3, -4, -3, -2,
			  -3, -4, 60, -4, -3,
			  -2, -3, -4, -3, -2,
			  -1, -2, -3, -2, -1 };

printf("**emboss2_3x3\n");
	pk = pkl_open_file(img);
	pkl_emboss2(pk, mask9, 3, 3, 1.0, 255);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/emboss2_3x3.jpg");
	pkl_close(pk);
	

printf("**emboss2_5x5\n");
	pk = pkl_open_file(img);
	pkl_emboss2(pk, mask25, 5, 5, 1.0, 255);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/emboss2_5x5.jpg");
	pkl_close(pk);
}

//=============================================================================
// focus2
//=============================================================================
static void focus2(char *img)
{
	PKLImage pk;
	int mask9[3*3] =
			{ 0,  0, -1,
			  0,  0,  0,
			  2,  0,  0 };

	int mask12[5*5] =
			{ -1, -1, -1, -1,
			   0,  2,  2,  0,
			   0,  0,  0,  0,};

printf("**focus2_3x3\n");
	pk = pkl_open_file(img);
	pkl_focus2(pk, mask9, 3, 3);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/focus2_3x3.jpg");
	pkl_close(pk);
	
printf("**focus2_5x5\n");
	pk = pkl_open_file(img);
	pkl_focus2(pk, mask12, 4, 3);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/focus2_4x3.jpg");
	pkl_close(pk);
}

//=============================================================================
// hydrangea
//=============================================================================
static void hydrangea(char *img)
{
	PKLImage pk;
	
printf("**hydrangea\n");
	pk = pkl_open_file(img);
	pkl_hydrangea(pk, 10, 9.0, 9.0, 1.0, 50, 70);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/hydrangea.jpg");
	pkl_close(pk);
}

//=============================================================================
// alphaedge
//=============================================================================
static void alphaedge(char *img)
{
	PKLImage pk;
	
printf("**alphaedge1\n");
	pk = pkl_open_file(img);
	pkl_alphaedge(pk, 5, 9.0, 9.0, 1.0, 30, 70);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/alphaedge1.jpg");
	pkl_close(pk);
	
printf("**alphaedge2\n");
	pk = pkl_open_file(img);
	pkl_alphaedge(pk, 5, 9.0, 9.0, 1.0, 30, -1);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/alphaedge2.jpg");
	pkl_close(pk);

printf("**alphaedge3\n");
	pk = pkl_open_file(img);
	pkl_alphaedge(pk, 8, 9.0, 9.0, 1.0, 50, 80);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/alphaedge3.jpg");
	pkl_close(pk);
}

//=============================================================================
// decrease
//=============================================================================
static void decrease(char *img)
{
	PKLImage pk;
	
printf("**decrease-RGB\n");
	pk = pkl_open_file(img);
	pkl_rgb(pk);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/cmyk_rgb.jpg");
	pkl_close(pk);
	
printf("**decrease-GRAY1\n");
	pk = pkl_open_file(img);
	pkl_gray(pk, PKL_GRAY_NRM);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rgb_gray1.jpg");
	pkl_close(pk);

printf("**decrease-GRAY2\n");
	pk = pkl_open_file(img);
	pkl_gray(pk, PKL_GRAY_MED);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rgb_gray2.jpg");
	pkl_close(pk);
	
printf("**decrease-GRAY3\n");
	pk = pkl_open_file(img);
	pkl_gray(pk, PKL_GRAY_AVE);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/rgb_gray3.jpg");
	pkl_close(pk);
	
printf("**decrease-2c\n");
	pk = pkl_open_file(img);
	pkl_2c(pk, 127);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/2c.jpg");
	pkl_close(pk);
}

//=================================================================================
// swapcolor
//=================================================================================
static void swapcolor(char *img)
{
	PKLImage pk;

printf("**swapcolor-RBG\n");
	pk = pkl_open_file(img);
	pkl_swapcolor(pk, PKL_RGB2RBG);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swapcolor-RBG.jpg");
	pkl_close(pk);
	
printf("**swapcolor-BGR\n");
	pk = pkl_open_file(img);
	pkl_swapcolor(pk, PKL_RGB2BGR);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swapcolor-BGR.jpg");
	pkl_close(pk);

printf("**swapcolor-BRG\n");
	pk = pkl_open_file(img);
	pkl_swapcolor(pk, PKL_RGB2BRG);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swapcolor-BRG.jpg");
	pkl_close(pk);

printf("**swapcolor-GRB\n");
	pk = pkl_open_file(img);
	pkl_swapcolor(pk, PKL_RGB2GRB);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swapcolor-GRB.jpg");
	pkl_close(pk);

printf("**swapcolor-GBR\n");
	pk = pkl_open_file(img);
	pkl_swapcolor(pk, PKL_RGB2GBR);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/swapcolor-GBR.jpg");
	pkl_close(pk);
}


//=================================================================================
// cylinder
//=================================================================================
static void cylinder(char *img)
{
	PKLImage pk;

printf("**cylinder-h\n");
	pk = pkl_open_file(img);
	pkl_cylinder(pk, PKL_CYLINDER_HORZ, PKL_SAMPLE_NN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/cylinder-h_nn.jpg");
	pkl_close(pk);
	
printf("**cylinder-h\n");
	pk = pkl_open_file(img);
	pkl_cylinder(pk, PKL_CYLINDER_HORZ, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/cylinder-h_bl.jpg");
	pkl_close(pk);
	
printf("**cylinder-h\n");
	pk = pkl_open_file(img);
	pkl_cylinder(pk, PKL_CYLINDER_HORZ, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/cylinder-h_bc.jpg");
	pkl_close(pk);
	
	
printf("**cylinder-v\n");
	pk = pkl_open_file(img);
	pkl_cylinder(pk, PKL_CYLINDER_VERT, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/cylinder-v.jpg");
	pkl_close(pk);
printf("**cylinder-b\n");
	pk = pkl_open_file(img);
	pkl_cylinder(pk, PKL_CYLINDER_BOTH, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/cylinder-b.jpg");
	pkl_close(pk);
}

//=================================================================================
// toy
//=================================================================================
static void toy(char *img)
{
	PKLImage pk;
	PKLColor c;
	int radius;
	
printf("**toy\n");
	pk = pkl_open_file(img);
	
	c = pkl_color_create(0, 0, 0, 0);
	radius = pkl_width(pk) < pkl_height(pk) ? pkl_width(pk) : pkl_height(pk);
	pkl_toy(pk, c, 20, radius, -1, -1, 100, 1);
	
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/toy.jpg");
	pkl_close(pk);
}

//=================================================================================
// solarization
//=================================================================================
static void solarization(char *img)
{
	PKLImage pk;
	
printf("**solarization-0\n");
	pk = pkl_open_file(img);
	pkl_solarization(pk, 0, 50);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/solarization-0.jpg");
	pkl_close(pk);
	
printf("**solarization-1\n");
	pk = pkl_open_file(img);
	pkl_solarization(pk, 1, 100);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/solarization-1.jpg");
	pkl_close(pk);
}

//=================================================================================
// ellipse
//=================================================================================
static void ellipse(char *img)
{
	PKLImage pk;
	PKLColor c;
	int radius, x, y, i;
	
printf("**ellipse-1\n");
	pk = pkl_canvas(500, 500, PKL_RGB, NULL);
	srand(time(NULL));
	for(i=0; i<30; i++){
		radius = rand()%150;
		x = rand()%500;
		y = rand()%500;
		c = pkl_color_create(rand()%255, rand()%255, rand()%255, 0);
		draw_ellipse(pk, x, y, radius, radius, c, 0, 0);
		pkl_color_close(c);
	}
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/ellipse1.bmp");
	pkl_close(pk);

printf("**ellipse-2\n");
	//pk = pkl_open_file(img);
	pk = pkl_canvas(500, 500, PKL_RGB, NULL);
	
	c = pkl_color_create(0, 0, 0xff, 0);
	
	draw_ellipse(pk,  70, 70, 50, 50, c, 0, 0);
	draw_ellipse(pk, 180, 70, 50, 50, c, 0, 5);
	draw_ellipse(pk, 290, 70, 50, 50, c, 0, 8);
	draw_ellipse(pk, 400, 70, 50, 50, c, 0,10);
	
	draw_ellipse(pk,  70, 200, 20, 50, c, 0, 0);
	draw_ellipse(pk, 180, 200, 20, 50, c, 0, 5);
	draw_ellipse(pk, 290, 200, 20, 50, c, 0, 8);
	draw_ellipse(pk, 400, 200, 20, 50, c, 0,10);
	
	draw_ellipse(pk,  70, 330, 50, 20, c, 0, 0);
	draw_ellipse(pk, 180, 330, 50, 20, c, 0, 5);
	draw_ellipse(pk, 290, 330, 50, 20, c, 0, 8);
	draw_ellipse(pk, 400, 330, 50, 20, c, 0,10);
	
	draw_ellipse(pk,  10, 420,  5,  5, c, 0, 0);
	draw_ellipse(pk,  30, 420, 10, 10, c, 0, 0);
	draw_ellipse(pk,  80, 420, 30, 30, c, 0, 0);
	draw_ellipse(pk, 180, 420, 50, 50, c, 0, 0);
	
	draw_ellipse(pk, 290, 420, 20, 20, c, 0, 3);
	draw_ellipse(pk, 350, 420, 20, 20, c, 0, 5);
	draw_ellipse(pk, 410, 420, 20, 20, c, 0, 8);
	draw_ellipse(pk, 470, 420, 20, 20, c, 0,10);
	
	pkl_color_close(c);
	
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/ellipse2.bmp");
	pkl_close(pk);
	
	
printf("**ellipse-edge\n");
	pk = pkl_open_file(img);
	//pk = pkl_canvas(500, 500, PKL_RGB, NULL);
	
	c = pkl_color_create(0xff, 0, 0, 0);
	draw_ellipse(pk, 150, 150,  80,  80, c, 5, 0);
	draw_ellipse(pk, 400, 400, 150, 150, c, 5, 0);
	draw_ellipse(pk, -30, -30, 150, 150, c, 5, 0);
	pkl_color_close(c);
	
	c = pkl_color_create(0, 0, 0xff, 0);
	draw_ellipse(pk, 150, 150,  80, 40, c, 5, 0);
	draw_ellipse(pk, 400, 400, 150, 50, c, 5, 0);
	draw_ellipse(pk, -30, -30, 150, 50, c, 5, 0);
	pkl_color_close(c);
	
	c = pkl_color_create(0, 0xff, 0xff, 0);
	draw_ellipse(pk, 150, 150, 40,  80, c, 5, 0);
	draw_ellipse(pk, 400, 400, 50, 150, c, 5, 0);
	draw_ellipse(pk, -30, -30, 50, 150, c, 5, 0);
	pkl_color_close(c);
	
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/ellipse_edge.bmp");
	pkl_close(pk);
}

//=================================================================================
// line
//=================================================================================
static void line()
{
	PKLImage pk, pk2;
	PKLColor c;
	int i;
	
printf("**line\n");
	pk = pkl_canvas(500, 500, PKL_RGB, NULL);

	c = pkl_color_create(0, 0, 0, 0);
		draw_line(pk, 10, 10, 150, 10, 1, c, 0, 0);
		draw_line(pk, 10, 20, 150, 20, 2, c, 0, 0);
		draw_line(pk, 10, 30, 150, 30, 3, c, 0, 0);
		draw_line(pk, 10, 40, 150, 40, 4, c, 0, 0);
		draw_line(pk, 10, 50, 150, 50, 5, c, 0, 0);
	pkl_color_close(c);
	
	c = pkl_color_create(0, 0, 0xff, 0);
		draw_line(pk, 400, 10, 400, 200, 1, c, 0, 0);
		draw_line(pk, 410, 10, 410, 200, 2, c, 0, 0);
		draw_line(pk, 420, 10, 420, 200, 3, c, 0, 0);
		draw_line(pk, 430, 10, 430, 200, 4, c, 0, 0);
		draw_line(pk, 440, 10, 440, 200, 5, c, 0, 0);
	pkl_color_close(c);
	
	
	
	c = pkl_color_create(0, 0, 0, 0);
		draw_line(pk, 10, 100, 100, 200, 1, c, 0, 0);
		draw_line(pk, 10, 110, 100, 210, 2, c, 0, 0);
		draw_line(pk, 10, 120, 100, 220, 3, c, 0, 0);
		draw_line(pk, 10, 130, 100, 230, 4, c, 0, 0);
		draw_line(pk, 10, 140, 100, 240, 5, c, 0, 0);
	pkl_color_close(c);

	c = pkl_color_create(0, 0, 0xff, 0);
		draw_line(pk, 100, 200, 190, 100, 1, c, 0, 0);
		draw_line(pk, 100, 210, 190, 110, 2, c, 0, 0);
		draw_line(pk, 100, 220, 190, 120, 3, c, 0, 0);
		draw_line(pk, 100, 230, 190, 130, 4, c, 0, 0);
		draw_line(pk, 100, 240, 190, 140, 5, c, 0, 0);
	pkl_color_close(c);
	

	
	c = pkl_color_create(0, 0, 0, 0);
		draw_line(pk, 10, 250, 150, 260, 1, c, 0, 0);
		draw_line(pk, 10, 260, 150, 270, 2, c, 0, 0);
		draw_line(pk, 10, 270, 150, 280, 3, c, 0, 0);
		draw_line(pk, 10, 280, 150, 290, 4, c, 0, 0);
		draw_line(pk, 10, 290, 150, 300, 5, c, 0, 0);
	pkl_color_close(c);

	c = pkl_color_create(0, 0, 0xff, 0);
		draw_line(pk, 150, 260, 300, 250, 1, c, 0, 0);
		draw_line(pk, 150, 270, 300, 260, 2, c, 0, 0);
		draw_line(pk, 150, 280, 300, 270, 3, c, 0, 0);
		draw_line(pk, 150, 290, 300, 280, 4, c, 0, 0);
		draw_line(pk, 150, 300, 300, 290, 5, c, 0, 0);
	pkl_color_close(c);
	
	

	c = pkl_color_create(0, 0, 0, 0);
		draw_line(pk, 10, 300, 150, 440, 1, c, 0, 0);
		draw_line(pk, 10, 310, 150, 450, 2, c, 0, 0);
		draw_line(pk, 10, 320, 150, 460, 3, c, 0, 0);
		draw_line(pk, 10, 330, 150, 470, 4, c, 0, 0);
		draw_line(pk, 10, 340, 150, 480, 5, c, 0, 0);
	pkl_color_close(c);
	
	c = pkl_color_create(0, 0, 0xff, 0);
		draw_line(pk, 150, 440, 290, 300, 1, c, 0, 0);
		draw_line(pk, 150, 450, 290, 310, 2, c, 0, 0);
		draw_line(pk, 150, 460, 290, 320, 3, c, 0, 0);
		draw_line(pk, 150, 470, 290, 330, 4, c, 0, 0);
		draw_line(pk, 150, 480, 290, 340, 5, c, 0, 0);
	pkl_color_close(c);
	
	
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/line1.bmp");
	pkl_close(pk);



printf("**line-aa\n");
	pk = pkl_canvas(500, 500, PKL_RGB, NULL);
	
	c = pkl_color_create(0, 0, 0, 0);
		draw_line(pk, 10, 10, 150, 10, 1, c, 1, 1);
		draw_line(pk, 10, 20, 150, 20, 2, c, 1, 1);
		draw_line(pk, 10, 30, 150, 30, 3, c, 1, 1);
		draw_line(pk, 10, 40, 150, 40, 4, c, 1, 1);
		draw_line(pk, 10, 50, 150, 50, 5, c, 1, 1);
	pkl_color_close(c);
	
	c = pkl_color_create(0, 0, 0xff, 0);
		draw_line(pk, 400, 10, 400, 200, 1, c, 1, 1);
		draw_line(pk, 410, 10, 410, 200, 2, c, 1, 1);
		draw_line(pk, 420, 10, 420, 200, 3, c, 1, 1);
		draw_line(pk, 430, 10, 430, 200, 4, c, 1, 1);
		draw_line(pk, 440, 10, 440, 200, 5, c, 1, 1);
	pkl_color_close(c);

	c = pkl_color_create(0, 0, 0, 0);
		draw_line(pk, 10, 100, 100, 200, 1, c, 1, 1);
		draw_line(pk, 10, 110, 100, 210, 2, c, 1, 1);
		draw_line(pk, 10, 120, 100, 220, 3, c, 1, 1);
		draw_line(pk, 10, 130, 100, 230, 4, c, 1, 1);
		draw_line(pk, 10, 140, 100, 240, 5, c, 1, 1);
	pkl_color_close(c);

	c = pkl_color_create(0, 0, 0xff, 0);
		draw_line(pk, 100, 200, 190, 100, 1, c, 1, 1);
		draw_line(pk, 100, 210, 190, 110, 2, c, 1, 1);
		draw_line(pk, 100, 220, 190, 120, 3, c, 1, 1);
		draw_line(pk, 100, 230, 190, 130, 4, c, 1, 1);
		draw_line(pk, 100, 240, 190, 140, 5, c, 1, 1);
	pkl_color_close(c);
	

	
	c = pkl_color_create(0, 0, 0, 0);
		draw_line(pk, 10, 250, 150, 260, 1, c, 1, 1);
		draw_line(pk, 10, 260, 150, 270, 2, c, 1, 1);
		draw_line(pk, 10, 270, 150, 280, 3, c, 1, 1);
		draw_line(pk, 10, 280, 150, 290, 4, c, 1, 1);
		draw_line(pk, 10, 290, 150, 300, 5, c, 1, 1);
	pkl_color_close(c);

	c = pkl_color_create(0, 0, 0xff, 0);
		draw_line(pk, 150, 260, 300, 250, 1, c, 1, 1);
		draw_line(pk, 150, 270, 300, 260, 2, c, 1, 1);
		draw_line(pk, 150, 280, 300, 270, 3, c, 1, 1);
		draw_line(pk, 150, 290, 300, 280, 4, c, 1, 1);
		draw_line(pk, 150, 300, 300, 290, 5, c, 1, 1);
	pkl_color_close(c);
	
	

	c = pkl_color_create(0, 0, 0, 0);
		draw_line(pk, 10, 300, 150, 440, 1, c, 1, 1);
		draw_line(pk, 10, 310, 150, 450, 2, c, 1, 1);
		draw_line(pk, 10, 320, 150, 460, 3, c, 1, 1);
		draw_line(pk, 10, 330, 150, 470, 4, c, 1, 1);
		draw_line(pk, 10, 340, 150, 480, 5, c, 1, 1);
	pkl_color_close(c);
	
	c = pkl_color_create(0, 0, 0xff, 0);
		draw_line(pk, 150, 440, 290, 300, 1, c, 1, 1);
		draw_line(pk, 150, 450, 290, 310, 2, c, 1, 1);
		draw_line(pk, 150, 460, 290, 320, 3, c, 1, 1);
		draw_line(pk, 150, 470, 290, 330, 4, c, 1, 1);
		draw_line(pk, 150, 480, 290, 340, 5, c, 1, 1);
	pkl_color_close(c);

	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/line2.bmp");
	pkl_close(pk);
	

printf("**line-2\n");
	pk  = pkl_canvas(500, 500, PKL_RGB, NULL);
	pk2 = pkl_canvas(500, 500, PKL_RGB, NULL);
	srand(time(NULL));
	//srand(0);
	for(i=0; i<500; i++){
		int x1 = rand()%500 - 10;
		int y1 = rand()%500 - 10;
		int x2 = rand()%500 + 10;
		int y2 = rand()%500 + 10;
		c = pkl_color_create(rand()%255, rand()%255, rand()%255, 0);
		draw_line(pk,  x1, y1, x2, y2, i%6, c, 0, 0);
		draw_line(pk2, x1, y1, x2, y2, i%6, c, 1, 1);
		pkl_color_close(c);
	}
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/line3.bmp");
	pkl_save_file(pk2, PKL_FORMAT_BITMAP, "out/line4.bmp");
	pkl_close(pk);
	pkl_close(pk2);

printf("**line-5\n");
	pk = pkl_canvas(500, 500, PKL_RGB, NULL);
{
	int width = 20;
	
	c = pkl_color_create(0, 0, 0xff, 0);
	draw_line(pk, 100, 100, 200, 400, width, c, 0, 0);
	draw_line(pk, 150, 100, 250, 400, width, c, 1, 1);
	pkl_color_close(c);
	
	c = pkl_color_create(0xff, 0, 0, 0);
	draw_line(pk, 150, 400, 250, 100, width, c, 1, 1);
	draw_line(pk,  50,  50, 200, 400, width, c, 1, 1);
	pkl_color_close(c);
	
	c = pkl_color_create(0, 0, 0, 0);
	draw_line(pk, 50, 400, 400, 100, width, c, 0, 0);
	draw_line(pk, 100, 400, 450, 100, width, c, 1, 1);
	//pkl_color_close(c);
	
	//c = pkl_color_create(0, 0xff, 0, 0);
	draw_line(pk, 50, 100, 400, 400, width, c, 0, 0);
	draw_line(pk, 100, 100, 450, 400, width, c, 1, 1);
	draw_line(pk, 30, 100, 450, 200, width, c, 1, 1);
	draw_line(pk, 30, 200, 450, 230, width, c, 1, 1);
	pkl_color_close(c);
}

	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/line5.bmp");
	pkl_close(pk);
	


printf("**line-6\n");
	pk  = pkl_canvas(500, 500, PKL_RGB, NULL);
	srand(time(NULL));
	//srand(0);
	for(i=0; i<100; i++){
		int x1 = rand()%500;
		int y1 = rand()%500;
		c = pkl_color_create(rand()%255, rand()%255, rand()%255, 0);
		draw_line(pk,  0, 0, x1, y1, i%6, c, 1, 1);
		pkl_color_close(c);
		
		int x2 = rand()%500;
		int y2 = rand()%500;
		c = pkl_color_create(rand()%255, rand()%255, rand()%255, 0);
		draw_line(pk, 500, 0, x2, y2, i%6, c, 1, 1);
		pkl_color_close(c);
	}
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/line6.bmp");
	pkl_close(pk);
}

//=================================================================================
// line
//=================================================================================
static void rect()
{
	PKLImage pk;
	PKLColor c;
	
printf("**rect1\n");
	pk = pkl_canvas(500, 500, PKL_RGB, NULL);

	c = pkl_color_create(0, 0, 0, 0);
		draw_rect(pk, 30, 30, 100, 100, 6, c, 0);
	pkl_color_close(c);
	
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/rect1.bmp");
	pkl_close(pk);
	
printf("**rect2\n");
	pk = pkl_canvas(500, 500, PKL_RGB, NULL);

	c = pkl_color_create(0, 0, 0, 0);
		draw_rect(pk, 800, 800, 300, 300, 6, c, 1);
	pkl_color_close(c);
	
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/rect2.bmp");
	pkl_close(pk);
}

//=================================================================================
// sphere
//=================================================================================
static void sphere(char *img)
{
	PKLImage pk;
	
printf("**sphere-nn\n");
	pk = pkl_open_file(img);
	pkl_sphere(pk, NULL, PKL_SAMPLE_NN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sphere-nn.jpg");
	pkl_close(pk);
	
printf("**sphere-bl\n");
	pk = pkl_open_file(img);
	pkl_sphere(pk, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sphere-bl.jpg");
	pkl_close(pk);
	
printf("**sphere-bc\n");
	pk = pkl_open_file(img);
	pkl_sphere(pk, NULL, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sphere-bc.jpg");
	pkl_close(pk);
}

//=================================================================================
// fisheye
//=================================================================================
static void fisheye(char *img)
{
	PKLImage pk;
	
printf("**fisheye-nn\n");
	pk = pkl_open_file(img);
	pkl_fisheye(pk, 30.0, PKL_SAMPLE_NN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/fisheye-nn.jpg");
	pkl_close(pk);
	
printf("**fisheye-bl\n");
	pk = pkl_open_file(img);
	pkl_fisheye(pk, 30.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/fisheye-bl.jpg");
	pkl_close(pk);
	
printf("**fisheye1\n");
	pk = pkl_open_file(img);
	pkl_fisheye(pk, 10.0, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/fisheye1.jpg");
	pkl_close(pk);
	
printf("**fisheye2\n");
	pk = pkl_open_file(img);
	pkl_fisheye(pk, 30.0, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/fisheye2.jpg");
	pkl_close(pk);
	
printf("**fisheye3\n");
	pk = pkl_open_file(img);
	pkl_fisheye(pk, 50.0, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/fisheye3.jpg");
	pkl_close(pk);
}

//=================================================================================
// sinusoidal
//=================================================================================
static void sinusoidal(char *img)
{
	PKLImage pk;
	
printf("**sinusoidal-0\n");
	pk = pkl_open_file(img);
	pkl_sinusoidal(pk, 5.0, PKL_SAMPLE_NN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sinusoidal-0.jpg");
	pkl_close(pk);
	
printf("**sinusoidal-1\n");
	pk = pkl_open_file(img);
	pkl_sinusoidal(pk, 5.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sinusoidal-1.jpg");
	pkl_close(pk);
	
printf("**sinusoidal-2\n");
	pk = pkl_open_file(img);
	pkl_sinusoidal(pk, 5.0, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sinusoidal-2.jpg");
	pkl_close(pk);
}

//=================================================================================
// sinusoidal2
//=================================================================================
static void sinusoidal2(char *img)
{
	PKLImage pk;
	
printf("**sinusoidal2-1\n");
	pk = pkl_open_file(img);
	pkl_sinusoidal2(pk, 5.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/sinusoidal2.jpg");
	pkl_close(pk);
}

//=================================================================================
// fabric
//=================================================================================
static void fabric(char *img)
{
	PKLImage pk;
	
printf("**fabric-1\n");
	pk = pkl_open_file(img);
	pkl_fabric(pk, 3.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/fabric-1.jpg");
	pkl_close(pk);
	
printf("**fabric-2\n");
	pk = pkl_open_file(img);
	pkl_fabric(pk, 5.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/fabric-2.jpg");
	pkl_close(pk);

printf("**fabric-3\n");
	pk = pkl_open_file(img);
	pkl_fabric(pk, 8.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/fabric-3.jpg");
	pkl_close(pk);
}

//=================================================================================
// water
//=================================================================================
static void water(char *img)
{
	PKLImage pk;
	
printf("**water-nn\n");
	pk = pkl_open_file(img);
	pkl_water(pk, -1, -1, -1, 30.0, 1.3, 0.0, PKL_SAMPLE_NN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/water-nn.jpg");
	pkl_close(pk);
	
printf("**water-bl\n");
	pk = pkl_open_file(img);
	pkl_water(pk, -1, -1, -1, 30.0, 1.3, 0.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/water-bl.jpg");
	pkl_close(pk);
	
printf("**water-bc\n");
	pk = pkl_open_file(img);
	pkl_water(pk, -1, -1, -1, 30.0, 1.3, 0.0, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/water-bc.jpg");
	pkl_close(pk);	
}

//=================================================================================
// tv
//=================================================================================
static void tv(char *img)
{
	PKLImage pk;
	
printf("**tv\n");
	pk = pkl_open_file(img);
	pkl_tv(pk, 1, 4);
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/tv.bmp");
	pkl_close(pk);
}

//=================================================================================
// disk
//=================================================================================
static void disk(char *img)
{
	PKLImage pk;
	
printf("**disk-1\n");
	pk = pkl_open_file(img);
	pkl_disk(pk, -1, -1, 1.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/disk-1.jpg");
	pkl_close(pk);
	
printf("**disk-2\n");
	pk = pkl_open_file(img);
	pkl_disk(pk, -1, -1, 5.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/disk-2.jpg");
	pkl_close(pk);
	
printf("**disk-3\n");
	pk = pkl_open_file(img);
	pkl_disk(pk, -1, -1, 10.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/disk-3.jpg");
	pkl_close(pk);
}

//=================================================================================
// disk
//=================================================================================
static void anaglyph(char *img)
{
	PKLImage pk, pk2;
	
printf("**anaglyph-1\n");
	pk = pkl_open_file(img);
	pkl_anaglyph(pk, NULL, 5, 0, 0, 0);
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/anaglyph-1.bmp");
	pkl_close(pk);
	
printf("**anaglyph-2\n");
	pk = pkl_open_file(img);
	pkl_anaglyph(pk, NULL, 5, 0, 1, 0);
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/anaglyph-2.bmp");
	pkl_close(pk);
	
printf("**anaglyph-3\n");
	pk = pkl_open_file(img);
	pk2 = pkl_open_file("in/novato.jpg");
	pkl_anaglyph(pk, pk2, 5, 0, 0, 0);
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/anaglyph-3.bmp");
	pkl_close(pk);
	pkl_close(pk2);
	
printf("**anaglyph-4\n");
	pk = pkl_open_file(img);
	pkl_anaglyph(pk, NULL, 10, 0, 0, 1);
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/anaglyph-4.bmp");
	pkl_close(pk);
	
}

//=============================================================================
// ball
//=============================================================================
static void ball(char *img)
{
	PKLImage pk;
	
printf("**ball-1\n");
	pk = pkl_open_file(img);
	pkl_ball(pk, NULL, PKL_SAMPLE_NN);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/ball_nn.jpg");
	pkl_close(pk);
	
printf("**ball-2\n");
	pk = pkl_open_file(img);
	pkl_ball(pk, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/ball_bl.jpg");
	pkl_close(pk);
	
printf("**ball-3\n");
	pk = pkl_open_file(img);
	pkl_ball(pk, NULL, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/ball_bc.jpg");
	pkl_close(pk);
}

//=================================================================================
// marble
//=================================================================================
static void marble(char *img)
{
	PKLImage pk;
	
printf("**marble-1\n");
	pk = pkl_open_file(img);
	pkl_marble(pk, 10.0, 10.0, 1.0, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/marble_bl.jpg");
	pkl_close(pk);

printf("**marble-2\n");
	pk = pkl_open_file(img);
	pkl_marble(pk, 10.0, 10.0, 1.0, PKL_SAMPLE_BC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/marble_bc.jpg");
	pkl_close(pk);
}

//=================================================================================
// perlin1d
//=================================================================================
static void perlin1d(char *img)
{
	PKLImage pk;
	
printf("**perlin_noise1d-1\n");
	pk = pkl_open_file(img);
	pkl_perlin1d(pk, 5.0, 5.0, 10.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/perlin_noise1d-1.jpg");
	pkl_close(pk);
	
printf("**perlin_noise1d-2\n");
	pk = pkl_open_file(img);
	pkl_perlin1d(pk, 10.0, 10.0, 10.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/perlin_noise1d-2.jpg");
	pkl_close(pk);

printf("**perlin_noise1d-3\n");
	pk = pkl_open_file(img);
	pkl_perlin1d(pk, 20.0, 20.0, 10.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/perlin_noise1d-3.jpg");
	pkl_close(pk);
}

//=================================================================================
// perlin2d
//=================================================================================
static void perlin2d(char *img)
{
	PKLImage pk;
	
printf("**perlin_noise2d-1\n");
	pk = pkl_open_file(img);
	pkl_perlin2d(pk, 5.0, 5.0, 10.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/perlin_noise2d-1.jpg");
	pkl_close(pk);

printf("**perlin_noise2d-2\n");
	pk = pkl_open_file(img);
	pkl_perlin2d(pk, 10.0, 10.0, 10.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/perlin_noise2d-2.jpg");
	pkl_close(pk);

printf("**perlin_noise2d-3\n");
	pk = pkl_open_file(img);
	pkl_perlin2d(pk, 20.0, 20.0, 10.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/perlin_noise2d-3.jpg");
	pkl_close(pk);
}

//=================================================================================
// perlin3d
//=================================================================================
static void perlin3d(char *img)
{
	PKLImage pk;
	
printf("**perlin_noise3d-1\n");
	pk = pkl_open_file(img);
	pkl_perlin3d(pk, 5.0, 5.0, 10.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/perlin_noise3d-1.jpg");
	pkl_close(pk);

printf("**perlin_noise3d-2\n");
	pk = pkl_open_file(img);
	pkl_perlin3d(pk, 10.0, 10.0, 10.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/perlin_noise3d-2.jpg");
	pkl_close(pk);

printf("**perlin_noise3d-3\n");
	pk = pkl_open_file(img);
	pkl_perlin3d(pk, 20.0, 20.0, 10.0, 10.0, NULL, PKL_SAMPLE_BL);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/perlin_noise3d-3.jpg");
	pkl_close(pk);
}

//=================================================================================
// thistle
//=================================================================================
static void thistle(char *img)
{
	PKLImage pk;
	
printf("**thistle-1\n");
	pk = pkl_open_file(img);
	pkl_thistle(pk, 2, 3);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/thistle-1.jpg");
	pkl_close(pk);

printf("**thistle-2\n");
	pk = pkl_open_file(img);
	pkl_thistle(pk, 3, 5);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/thistle-2.jpg");
	pkl_close(pk);
}

//=================================================================================
// thorn
//=================================================================================
static void thorn(char *img)
{
	PKLImage pk;
	
printf("**thorn-1\n");
	pk = pkl_open_file(img);
	pkl_thorn(pk, 3, 1);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/thorn-1.jpg");
	pkl_close(pk);

printf("**thorn-2\n");
	pk = pkl_open_file(img);
	pkl_thorn(pk, 1, 2);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/thorn-2.jpg");
	pkl_close(pk);
}

//=================================================================================
// spotmosaic
//=================================================================================
static void spotmosaic(char *img)
{
	PKLImage pk;
	
printf("**spotmosaic-1\n");
	pk = pkl_open_file(img);
	pkl_spotmosaic(pk, 0, 170, 300, 100, 10, PKL_SPOT_MOSAIC);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/spotmosaic-1.jpg");
	pkl_close(pk);
	
printf("**spotmosaic-2\n");
	pk = pkl_open_file(img);
	pkl_spotmosaic(pk, 0, 170, 300, 100, 10, PKL_SPOT_BLUR);
	pkl_save_file(pk, PKL_FORMAT_JPEG, "out/spotmosaic-2.jpg");
	pkl_close(pk);
}

//=================================================================================
// io
//=================================================================================
static void io()
{
	PKLImage pk;
	FILE *fp;
	unsigned long length;
	unsigned char *in, *out;
	
	//JPEG
	fp = fopen("in/wp3_1024x768.jpg", "rb");
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	in = malloc(length);
	fread(in, 1, length, fp);
	fclose(fp);
	
	pk = pkl_open_mem(in, length);
	pkl_mosaic(pk, 20, 20);
	out = pkl_save_mem(pk, PKL_FORMAT_JPEG, &length);
	
	fp = fopen("out/mem.jpg", "wb");
	fwrite(out, 1, length, fp);
	fclose(fp);
	
	free(in);
	free(out);
	pkl_close(pk);
	
	
	//PNG
	fp = fopen("in/051028.png", "rb");
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	in = malloc(length);
	fread(in, 1, length, fp);
	fclose(fp);
	
	pk = pkl_open_mem(in, length);
	pkl_mosaic(pk, 20, 20);
	out = pkl_save_mem(pk, PKL_FORMAT_PNG, &length);
	
	fp = fopen("out/mem.png", "wb");
	fwrite(out, 1, length, fp);
	fclose(fp);
	
	free(in);
	free(out);
	pkl_close(pk);
	
	
	//BMP
	fp = fopen("in/xxx.bmp", "rb");
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	in = malloc(length);
	fread(in, 1, length, fp);
	fclose(fp);
	
	pk = pkl_open_mem(in, length);
	pkl_mosaic(pk, 20, 20);
	out = pkl_save_mem(pk, PKL_FORMAT_BITMAP, &length);
	
	fp = fopen("out/mem.bmp", "wb");
	fwrite(out, 1, length, fp);
	fclose(fp);
	
	free(in);
	free(out);
	pkl_close(pk);
}

//=============================================================================
// main
//=============================================================================
int main(int argc, char **argv)
{
	if(argc < 2){
		printf("**error augument to image.\n");
		return(1);
	}

//インフォメーション
	info(argv[1]);

//pixel_access
	pixel_access(argv[1]);

//変形(切り取り・回転・リサイズ)
	trim(argv[1]);
	rotate(argv[1]);
	resize(argv[1]);
	affine(argv[1]);

//エフェクト(ベーシック)
	unsharp(argv[1]);
	contrast(argv[1]);
	level(argv[1]);
	brightness(argv[1]);
	hls(argv[1]);
	_gamma(argv[1]);
	noisecut(argv[1]);

//--ぼかしシリーズ
	blur(argv[1]);
	gaussblur(argv[1]);
	mblur(argv[1]);
	rblur(argv[1]);
	ablur(argv[1]);
	wblur(argv[1]);

//--パターン
	pattern(argv[1]);
	voronoi_zone(argv[1]);
	voronoi_count(argv[1]);

//--画像分割効果
	mosaic(argv[1]);
	splitframe(argv[1]);
	tileslit(argv[1]);
	grid(argv[1]);

//--タイル
	tile(argv[1]);
	
//--減色
	//mediancut(argv[1]);
	//neuquant(argv[1]);
	//wu(argv[1]);
	posterize(argv[1]);
	cutcolor(argv[1]);
	decrease(argv[1]);

//---画像合成
	composite(argv[1], "in/tama.png");
	alphablend(argv[1], "in/IMG_0789.JPG", "in/tama.png");
//	alphablend(argv[1], "in/gray2.jpg", "in/gray3.jpg");
//	alphablend(argv[1], "in/cmyk2.jpg", "in/cmyk3.jpg");
	randomshadow(argv[1]);
	shadowframe(argv[1]);

//---ベーシックエフェクト
	invert(argv[1]);
	sepia(argv[1]);
	oilpaint(argv[1]);
	noise(argv[1]);
	vtr(argv[1]);
	edge(argv[1]);
	edgepaint(argv[1]);
	kuwahara(argv[1]);
	dither(argv[1]);
	colordither(argv[1]);
	focus(argv[1]);
	focus2(argv[1]);
	emboss(argv[1]);
	emboss2(argv[1]);
	contour(argv[1]);
	swapcolor(argv[1]);
	solarization(argv[1]);

//--camera filter
	vignette(argv[1]);
	vaseline(argv[1]);
	film(argv[1]);
	vv(argv[1]);
	//spotlight(argv[1]);

//--special
	edgeposter(argv[1]);
	crayon(argv[1]);
	rinkaku(argv[1]);
	hydrangea(argv[1]);
	alphaedge(argv[1]);
	toy(argv[1]);
	sinusoidal2(argv[1]);
	fabric(argv[1]);
	spotmosaic(argv[1]);
	
//--scrap
	illust(argv[1]);
	swirl(argv[1]);
	wave(argv[1]);
	dots(argv[1]);
	sobelpaint(argv[1]);
	rock(argv[1]);
	cylinder(argv[1]);
	sphere(argv[1]);
	fisheye(argv[1]);
	sinusoidal(argv[1]);
	water(argv[1]);
	tv(argv[1]);
	disk(argv[1]);
	anaglyph(argv[1]);
	ball(argv[1]);
	thistle(argv[1]);
	thorn(argv[1]);
	
//--noise
	marble(argv[1]);
	perlin1d(argv[1]);
	perlin2d(argv[1]);
	perlin3d(argv[1]);


//--draw
	ellipse(argv[1]);
	line();
	rect();


//--io
	io();

/////////////////////////////////////////////////////
{
	PKLImage pk;
	pk = pkl_open_file(argv[1]);
	pkl_test(pk);
	pkl_save_file(pk, PKL_FORMAT_BITMAP, "out/test.bmp");
	pkl_close(pk);
}
///////////////////////////////////////////////


#ifdef DEBUG
	dbg_print_alloc_count(stdout);
#endif

	return(0);
}
