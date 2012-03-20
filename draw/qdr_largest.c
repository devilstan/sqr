#include "qdr_largest.h"

//=================================================================================
// search_optimize_area
//=================================================================================
static int search_optimize_area(struct QDRRectStock *stock)
{
	int i, s, index=0;
	
	//ここに来たら、基本的に全ての面積は同じはず！
	
	//平方根で評価
	//s = sqrt(stock->rect[0].area);
	//while(s > 0){
	//	for(i=0; i<stock->count; i++){
	//		//極力sの正方形が入るであろう領域を探す
	//		if(stock->rect[i].w>=s && stock->rect[i].h>=s)
	//			return i;
	//		
	//		//printf("-----[%d]------\n", i);
	//		//printf("面積: %d\n", stock->rect[i].area);
	//		//printf("(x, y) = (%d, %d)\n", stock->rect[i].x, stock->rect[i].y);
	//		//printf("(w, h) = (%d, %d)\n", stock->rect[i].w, stock->rect[i].h);
	//	}
	//	s--;
	//}
	////giveup
	
	//w-hの絶対値が小さいモノほど正方形に近いと判断
	s = stock->rect[0].area;
	
	for(i=0; i<stock->count; i++){
		int diff = stock->rect[i].w - stock->rect[i].h;
		diff = diff<0 ? -diff : diff;
		if(diff < s){
			index = i;
			s = diff;
		}
	}
	return index;
}

//=================================================================================
// regist
//=================================================================================
static void regist(struct QDRRectStock *stock, struct QDRRect *tmp)
{
	//初回 もしくは 最後に登録されたものと同じ面積であれば追加登録
	if(stock->count==0 || tmp->area==stock->rect[stock->count-1].area)
		goto LAST;
	
	//最後に登録されたものより小さいモノは除外
	if(tmp->area < stock->rect[stock->count-1].area)
		return;
	
	
	//最後に登録されたものより大きければ刷新
	if(tmp->area > stock->rect[stock->count-1].area){
		stock->count = 0;
		goto LAST;
	}
	
LAST:
	if(stock->count >= 16){
		fprintf(stderr, "[warn] %s(%d) stock over.\n", __func__, __LINE__);
		return;
	}
	
	memcpy(&stock->rect[stock->count], tmp, sizeof(struct QDRRect));
	stock->count++;
}

//=================================================================================
// LargestRectangle
//=================================================================================
static void LargestRectangle(struct QDRRect *r, unsigned char buffer[], int size)
{
	struct LRStack S;
	int i, maxv;
	struct Rectangle rect;
	
	buffer[size] = 0;	//リミッタ用に最終+1に0をセットしておく
	maxv = 0;
	
	stack_init(&S);

	for(i=0; i<=size; i++){
		rect.height = buffer[i];
		rect.pos = i;
		
		if(stack_empty(&S)){
			stack_push(&S, &rect);
		}else{
			if((stack_top(&S))->height < rect.height){
				//スタックに入っているものより大きければ積んでいく
				stack_push(&S, &rect);
			}else if((stack_top(&S))->height > rect.height){
				//階段が下がったら、
				//スタックに入っている内容で大きな長方形が構築できるはずなので評価を開始する
				int target = i;
				
				while(!stack_empty(&S) && (stack_top(&S))->height >= rect.height){
					struct Rectangle *pre = stack_top(&S);
					stack_pop(&S);
					
					int area = pre->height * (i - pre->pos);

					target = pre->pos;
					
					//最大長方形領域の更新
					if(area >= maxv){
						maxv = area;
						
						r->area = maxv;
						r->y = pre->pos;	//ヒストグラムと実際のデータは座標系が違うので
						r->h = i-pre->pos;	//〃
						r->w = pre->height;	//〃
					}
				}
				rect.pos = target;
				stack_push(&S, &rect);
			}
		}
	}
}

//=================================================================================
// search_largest
//=================================================================================
static int search_largest(struct QDRRect *r, unsigned char data[][QDR_MAXMSIZE], int size)
{
	unsigned char T[QDR_MAXMSIZE][QDR_MAXMSIZE+1];	//ヒストグラムを格納する領域 --> MAX+1はリミッタ(0)用
	struct QDRRectStock stock;
	int i, j;
	
	if(!r)
		return 1;
	
	//ヒストグラムの初期化
	memset(T, 0, QDR_MAXMSIZE*(QDR_MAXMSIZE+1));
	
	//ヒストグラム構築
	//縦(下)方向に1が連続していたら、1を加算していく
	for(j=0; j<size; j++){
		int sequence = 0;
		for(i=0; i<size; i++){
			if(data[i][j] == 0){
				sequence = T[i][j] = 0;
			}else{
				T[i][j] = ++sequence;
			}
		}
	}
	
	//ヒストグラムの1行ずつの最大面積を評価していく
	int maxv = 0;
	memset(&stock, 0, sizeof(struct QDRRectStock));
	
	for(i=0; i<size; i++){
		struct QDRRect tmp;
		
		memset(&tmp, 0, sizeof(struct QDRRect));
		LargestRectangle(&tmp, T[i], size);
		
		if(tmp.area <= 0)
			continue;
		
		//最大領域をストック
		if(tmp.area >= maxv){
			maxv = tmp.area;
			
			//y起点を補足して、最大領域であれば登録
			tmp.x = i - tmp.w + 1;
			regist(&stock, &tmp);
		}
	}
	
	//登録されたものはなかった(これはないはず)
	if(stock.count <= 0)
		return 1;
	
	//最適領域を探し出す
	int z = search_optimize_area(&stock);
	
	//結果を格納
	memcpy(r, &stock.rect[z], sizeof(struct QDRRect));
	
	return 0;
}

//=================================================================================
// qdr_largest
//=================================================================================
static int qdr_largest(Qdr *qdr)
{
	if(!qdr)
		return 1;
	
	if(!qdr->lr.searched){
		search_largest(&qdr->lr.rect, qdr->data, qdr->ssize);
		qdr->lr.searched = 1;
	}
	return 0;
}

//=================================================================================
// qdr_largest_width
//=================================================================================
int qdr_largest_width(Qdr *qdr)
{
	return qdr_largest(qdr) ? 1 : qdr->lr.rect.w;
}

//=================================================================================
// qdr_largest_height
//=================================================================================
int qdr_largest_height(Qdr *qdr)
{
	return qdr_largest(qdr) ? 1 : qdr->lr.rect.h;
}

//=================================================================================
// qdr_largest_x
//=================================================================================
int qdr_largest_x(Qdr *qdr)
{
	return qdr_largest(qdr) ? 1 : qdr->lr.rect.x;
}

//=================================================================================
// qdr_largest_y
//=================================================================================
int qdr_largest_y(Qdr *qdr)
{
	return qdr_largest(qdr) ? 1 : qdr->lr.rect.y;
}

////=================================================================================
//// qdr_largestrect_paste
////=================================================================================
//int qdr_largestrect_paste(Qdr *qdr, const char *file)
//{
//}

#ifdef __LARGEST_RECTANGLE_DEBUG__
//=================================================================================
// main
//=================================================================================
int main()
{
	unsigned char data[QDR_MAXMSIZE][QDR_MAXMSIZE] = {
		//{ 1, 1, 1, 0, 1, 0, 0, 1, 0, 1 },
		//{ 0, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
		//{ 0, 0, 1, 1, 1, 1, 0, 0, 1, 1 },
		//{ 1, 1, 1, 1, 1, 1, 1, 0, 0, 1 },
		//{ 0, 0, 1, 0, 0, 0, 0, 0, 1, 1 },
		//{ 1, 1, 0, 0, 1, 0, 0, 1, 1, 0 },
		//{ 0, 1, 1, 1, 0, 1, 1, 0, 0, 1 },
		//{ 1, 1, 0, 1, 1, 0, 0, 1, 0, 1 },
		//{ 0, 0, 0, 1, 0, 1, 1, 0, 0, 0 },
		//{ 1, 0, 1, 0, 0, 1, 0, 0, 0, 0 }
		
		//{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		//{0, 0, 1, 1, 0, 0, 0, 0, 1, 0},
		//{0, 0, 1, 1, 0, 0, 0, 0, 1, 0},
		//{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
		//{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
		//{0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
		//{0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
		//{0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
		//{0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
		//{0, 0, 0, 0, 0, 0, 0, 0, 1, 0},

		//{1, 1, 1, 0, 1, 0, 0, 1, 0, 1},
		//{0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
		//{0, 0, 1, 1, 1, 1, 0, 0, 1, 1},
		//{1, 1, 1, 1, 0, 1, 1, 0, 0, 1},
		//{0, 0, 1, 0, 0, 0, 0, 0, 1, 1},
		//{1, 1, 0, 1, 1, 1, 1, 1, 1, 0},
		//{0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
		//{1, 1, 0, 1, 1, 1, 1, 1, 0, 1},
		//{0, 0, 0, 1, 0, 1, 1, 0, 0, 0},
		//{1, 0, 1, 0, 0, 1, 0, 0, 0, 0},
		
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{ 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
		{ 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0},
		{ 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
		{ 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0},
		{ 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0},
		{ 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
		{ 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0},
		{ 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
	};
	int size = 12;
	
	struct QDRLargestRect r;
	
	//最大長方形を探索
	memset(&r, 0, sizeof(struct QDRLargestRect));
	search_largest_rect(&r, data, size);
	
	//最大長方形の探索結果
	printf("面積: %d\n", r.area);
	printf("(x, y) = (%d, %d)\n", r.x, r.y);
	printf("(w, h) = (%d, %d)\n", r.w, r.h);
	
    return 0;
}
#endif
