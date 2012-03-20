#ifndef __QDR_LARGEST_RECTANGLE__
#define __QDR_LARGEST_RECTANGLE__

#include "qdr_private.h"
#include "qdr_stack.h"

//同じ面積が検出された時のストック用
struct QDRRectStock {
	int count;						//stockの使用数
	struct QDRRect rect[16];		//取り急ぎ16個用意(一応、リミッタは設定済)
};

#endif

