#ifndef _QDR_
#define _QDR_

#ifdef WIN32
	#define QDRExport __declspec(dllexport)
	#define XMD_H
	#define QDR_DEPRECATED
#else
	#define QDRExport
	#ifdef __GNUC__
		#define QDR_DEPRECATED __attribute__((__deprecated__))
	#endif
#endif

#if defined __cplusplus
extern "C" {
#endif

//=============================================================================
// version
//=============================================================================
#define QDR_VERSION_MAJOR 0
#define QDR_VERSION_MINOR 0
#define QDR_VERSION_MICRO 3
#define qdr_version() ( \
	  (QDR_VERSION_MAJOR * 10000) \
	+ (QDR_VERSION_MINOR *   100) \
	+ (QDR_VERSION_MICRO *     1))

int qdr_use_cairo_version();
int qdr_use_pikl_version();

//=============================================================================
// defines
//=============================================================================
#define QDR_MAXMSIZE       177
#define QDR_DEFAULT_MSIZE   10
#define QDR_DEFAULT_MARGIN   2
#define QDR_DEFAULT_RADIUS 0.3

//シンボル形状
typedef enum {
	QDR_MARKTYPE_RECTANGLE,		//四角形(default)
	QDR_MARKTYPE_ARC,			//角丸
	QDR_MARKTYPE_BLOCKARC,		//集合角丸
	QDR_MARKTYPE_METABALL		//メタボール風
} QDR_MARKTYPE;

//背景塗りパターン
typedef enum {
	QDR_BACKGROUND_SIMPLE,		//単色(default)
	QDR_BACKGROUND_IMAGE,		//画像
	QDR_BACKGROUND_GRAD			//グラデーション
} QDR_BACKGROUND_TYPE;

//マーク塗りパターン
typedef enum {
	QDR_MARKPAINT_SIMPLE,		//単色(default)
	QDR_MARKPAINT_IMAGE,		//画像
	QDR_MARKPAINT_PALETTE,		//パレット(グループ化した時のみ有効)
	QDR_MARKPAINT_GRAD			//グラデーション
} QDR_MARKPAINT_TYPE;

//グラデーション方向
typedef enum {
	QDR_GRADIENT_L2R,		//left to right
	QDR_GRADIENT_T2B,		//top to bottom
	QDR_GRADIENT_TL2BR,		//topleft to bottomright
	QDR_GRADIENT_BL2TR,		//bottomleft to topright
	QDR_GRADIENT_RADIAL		//放射状
} QDR_GRADIENT_TYPE;

// for paste
typedef enum {
	QDR_LAYOUT_UNSET,	//未設定
	QDR_LAYOUT_NORTH,	//上部
	QDR_LAYOUT_SOUTH,	//下部
	QDR_LAYOUT_EAST,	//右
	QDR_LAYOUT_WEST,	//左
	QDR_LAYOUT_CENTER	//中央
} QDR_LAYOUT;

// for paste
typedef enum {
	QDR_LEVEL_N =  0,	//未設定
	QDR_LEVEL_L =  7,
	QDR_LEVEL_M = 15,
	QDR_LEVEL_Q = 25,
	QDR_LEVEL_H = 30
} QDR_LEVEL;

// paste-type
typedef enum {
	QDR_PASTETYPE_LAYOUT,
	QDR_PASTETYPE_POSITION
} QDR_PASTETYPE;

// save-format
typedef enum {
	QDR_FORMAT_PNG,
	QDR_FORMAT_SVG,
	QDR_FORMAT_PDF
} QDR_FORMAT;

//=============================================================================
// datatypes
//=============================================================================
typedef struct _cairo_surface * QDRSurface;

//グループ
struct QDRGroupAttr {
	int count;
	double r, g, b, a;
};

struct QDRGroup {
	unsigned char data[QDR_MAXMSIZE][QDR_MAXMSIZE];
	int count;						//グループ数(実際にメンバーがいるかはattr.count>0で判定)
	struct QDRGroupAttr attr[256];	//取り急ぎ256個
	
	//1セル用画像設定
	const char *image;
	int is_mark;
};

//グラデーション
struct QDRGrad {
	QDR_GRADIENT_TYPE type;
	double r0, g0, b0, a0;
	double r1, g1, b1, a1;
};

//eye
struct QDREye {
	int enable;		//eyemarkの設定をしている時true
	double r, g, b, a;
	const char *image;
};

//最大長方形情報
struct QDRRect {
	int area;		//最大長方形の面積
	int x, y;		//最大長方形の左上起点
	int w, h;		//最大長方形のサイズ
};

typedef struct _Qdr {
	int margin;					//マージンサイズ
	int msize;					//1つのセルの一辺のピクセル数
	int ssize;					//QRデータの1行のサイズ(<=QRDRAW_MAX_MSIZE)
	int rsize;					//マージンを含めた実際のイメージの一辺
	unsigned char data[QDR_MAXMSIZE][QDR_MAXMSIZE];
	
	//シンボル形状
	QDR_MARKTYPE marktype;
	double radius;				//角丸用の情報

	//グループ情報
	struct QDRGroup group;
	
	//背景
	struct {
		QDR_BACKGROUND_TYPE type;
		double r, g, b, a;		//単色
		const char *image;		//画像
		struct QDRGrad grad;	//グラデーション
	} background;
	
	//シンボル塗り
	struct {
		QDR_MARKPAINT_TYPE type;
		double r, g, b, a;		//単色塗り
		const char *image;		//画像
		struct QDRGrad grad;	//グラデーション
	} markpaint;
	
	//影
	struct {
		double r, g, b, a;
		int offsetx, offsety;
	} shadow;
	
	//eye
	struct {
		unsigned char data[QDR_MAXMSIZE][QDR_MAXMSIZE];
		struct QDREye eye[6];
		struct {
			int x, y;
		} lastcell[3];
	} eyes;
	
	//paste
	struct {
		QDR_PASTETYPE type;
		const char *image;		//画像
		QDR_LAYOUT layout;		//配置場所
		QDR_LEVEL  level;		//誤り訂正レベル
		int x, y;				//for user-setting
	} paste;
	
	//最大長方形
	struct {
		int searched;			//探索済みの時true
		struct QDRRect rect;
	} lr;
	
	//text
	struct {
		const char *font;
		const char *utf8;
		unsigned int height;
		double r, g, b, a;
	} text;
	
	QDRSurface surface;
} Qdr;

//=============================================================================
// input/output format
//=============================================================================
/*
 <input>
   PNG
     use cairo
   GIF
     animation(但し1枚目のみを使用)
     interlace
     transparent
   JPG
     gray
     rgb
     cmyk

 <output(use cairo)>
   PNG
   PDF
   SVG
*/

//=================================================================================
// methods
//=================================================================================
//描画情報の初期化
//	symbolsize
//		dataの一辺のデータ長(実際にはQREncodeが返す値をそのまま設定する)
//	data
//		エンコード済みデータ(実際にはQREncodeが返す値をそのまま設定する)
int qdr_init(Qdr *qdr, unsigned int symbolsize, unsigned char data[QDR_MAXMSIZE][QDR_MAXMSIZE]);

void qdr_close(Qdr *qdr);

// モジュールサイズ
void qdr_set_msize(Qdr *qdr, unsigned int msize);
unsigned int qdr_get_msize(Qdr *qdr);

// マージン領域
void qdr_set_margin(Qdr *qdr, unsigned int margin);
unsigned int qdr_get_margin(Qdr *qdr);

//QR画像の仕上がりピクセル数を返す
#define qdr_size(msize, symbolsize, margin) (symbolsize+margin*2)*msize

//----------------------------------
// save
//----------------------------------
int qdr_save(Qdr *qdr, QDR_FORMAT format, const char *file);

#define qdr_save_png(qdr, file) qdr_save(qdr, QDR_FORMAT_PNG, file)
#define qdr_save_svg(qdr, file) qdr_save(qdr, QDR_FORMAT_SVG, file)
#define qdr_save_pdf(qdr, file) qdr_save(qdr, QDR_FORMAT_PDF, file)

//----------------------------------
// 背景
//----------------------------------
//単色
void qdr_bg_color(Qdr *qdr, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

//画像
// alphaは貼り付け画像に適用するalpha値
// 背景は隙間ができないようにフィットされる
int qdr_bg_image(Qdr *qdr, const char *file, unsigned char alpha);

//グラデーション
void qdr_bg_grad(Qdr *qdr, QDR_GRADIENT_TYPE type,
	unsigned char r0, unsigned char g0, unsigned char b0, unsigned char a0,
	unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1);

//----------------------------------
// グルーピング
//----------------------------------
// セルのグルーピングをする
int qdr_group(Qdr *qdr);

// グループ数を返す
int qdr_group_count(Qdr *qdr);

// グループに所属するメンバー数を返す
int qdr_group_member_count(Qdr *qdr, int index);

// グルーピングパレットの設定
int qdr_group_palette(Qdr *qdr, int index, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

// グルーピングパレットをランダムに設定する
//  offset: 色の下限
//  limit:  色の上限
void qdr_group_palette_rand(Qdr *qdr, unsigned char offset, unsigned char limit, unsigned char alpha);

// 1セル領域に画像を設定する
void qdr_group_image(Qdr *qdr, const char *file, int is_mark);

// 1セル領域の画像設定をクリアする
#define qdr_group_image_clear(qdr) qdr_group_image(qdr, NULL)

//----------------------------------
// シンボルの形状
//----------------------------------
// ARC, BLOCK_ARCの時にはradiusの指定が可能(radiusのデフォルトはQDR_DEFAULT_RADIUS)
void qdr_set_mark(Qdr *qdr, QDR_MARKTYPE type, double radius);

//----------------------------------
// マークの塗り
//----------------------------------
//単色
void qdr_set_mark_color(Qdr *qdr, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

//グラデーション
void qdr_set_mark_grad(Qdr *qdr, QDR_GRADIENT_TYPE type,
	unsigned char r0, unsigned char g0, unsigned char b0, unsigned char a0,
	unsigned char r1, unsigned char g1, unsigned char b1, unsigned char a1);

//画像
void qdr_set_mark_image(Qdr *qdr, const char *file, unsigned char alpha);

//目玉の色
// index
//	+---+---+
//	| 0 | 1 |
//	+---+---+
//	| 2 |   |
//	+---+---+
void qdr_eye_outer_color(Qdr *qdr, int index, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void qdr_eye_inner_color(Qdr *qdr, int index, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void qdr_eye_image(Qdr *qdr, int index, const char *file);

//設定のリセット
void qdr_eye_outer_color_reset(Qdr *qdr, int index);
void qdr_eye_inner_color_reset(Qdr *qdr, int index);
void qdr_eye_image_reset(Qdr *qdr, int index);

//3目玉に同じ設定をする
#define qdr_eye_color_all(qdr, r, g, b, a) \
	qdr_eye_outer_color(qdr, 0, r, g, b, a); \
	qdr_eye_outer_color(qdr, 1, r, g, b, a); \
	qdr_eye_outer_color(qdr, 2, r, g, b, a); \
	qdr_eye_inner_color(qdr, 0, r, g, b, a); \
	qdr_eye_inner_color(qdr, 1, r, g, b, a); \
	qdr_eye_inner_color(qdr, 2, r, g, b, a);

#define qdr_eye_image_all(qdr, index, file) \
	qdr_eye_image(qdr, 0, file); \
	qdr_eye_image(qdr, 1, file); \
	qdr_eye_image(qdr, 2, file);

// オプション
// 	影を付けるかどうか（影の色を設定）
void qdr_set_mark_shadow(Qdr *qdr, int offsetx, int offsety, 
		unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

//----------------------------------
// 画像の貼り付け
//----------------------------------
//画像を貼り付ける
// level(誤り訂正レベル)より
//   - 大きい場合は縮小
//   - 小さい場合はそのまま
// のサイズで貼り付ける
int qdr_paste_layout(Qdr *qdr, const char *file, QDR_LAYOUT layout, QDR_LEVEL level);

//任意の場所に画像を貼り付ける
// 誤り訂正レベルのチェックはしない
// 画像の拡縮もしない
// x, yは仕上がり画像の座標
int qdr_paste(Qdr *qdr, const char *file, int x, int y);

#define qdr_paste_reset(qdr) qdr_paste(qdr, NULL, 0, 0)

//----------------------------------
// 穴あけ
//----------------------------------
//今のところセンターのみ
// ※このメソッドを実行すると、シンボル情報が破壊される
int qdr_hole(Qdr *qdr, QDR_LEVEL level);

//----------------------------------
// 最大長方形領域
//----------------------------------
//最大長方形領域の情報
int qdr_largest_width(Qdr *qdr);
int qdr_largest_height(Qdr *qdr);
int qdr_largest_x(Qdr *qdr);
int qdr_largest_y(Qdr *qdr);

//最大領域に画像を貼る
//int qdr_largestrect_paste(Qdr *qdr, const char *file);

//----------------------------------
// 文字列の描画
//----------------------------------
//	operatorの検討
//	SLANTとweightの検討
//font: CSS2 generic family names, ("serif", "sans-serif", "cursive", "fantasy", "monospace")
int qdr_text(Qdr *qdr, const char *font, const char *utf8, unsigned int height,
						unsigned char r, unsigned char g, unsigned char b, unsigned char a);

#define qdr_text_reset(qdr) (qdr, NULL, NULL, 1, 0, 0, 0, 0)

//----------------------------------
// フィルター(仕上がり全体への効果)
//	このメソッド群は、draw後に実行しなければいけない。
//	pngのみに有効（SVG、PDFを選択した場合はこのメソッドは無効）
//----------------------------------
int qdr_filter_invert(Qdr *qdr);
int qdr_filter_crayon(Qdr *qdr, double factor, int bv1, int bv2, int alpha);
int qdr_filter_noise(Qdr *qdr, int noise);
int qdr_filter_circle(Qdr *qdr, int radius);
int qdr_filter_grid(Qdr *qdr, int ms, int color);
int qdr_filter_voronoi(Qdr *qdr, int zone);
int qdr_filter_dots(Qdr *qdr, int zone);
int qdr_filter_hexagon(Qdr *qdr, int s);
int qdr_filter_diamond(Qdr *qdr, int s);
int qdr_filter_tile(Qdr *qdr, int s);
int qdr_filter_noise(Qdr *qdr, int n);
int qdr_filter_hydrangea(Qdr *qdr, int n);
int qdr_filter_wave(Qdr *qdr, double factor, double frequency);
int qdr_filter_ball(Qdr *qdr);


//test(DEBUG)
int qdr_test(Qdr *qdr);


#if defined __cplusplus
}
#endif

#endif

