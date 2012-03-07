#include "qdr_group.h"

//=================================================================================
// qdr_group_count
//=================================================================================
int qdr_group_count(Qdr *qdr)
{
	return qdr ? qdr->group.count : 0;
}

//=================================================================================
// qdr_group_member_count
//=================================================================================
int qdr_group_member_count(Qdr *qdr, int index)
{
	if(!qdr)
		return 0;
	if(index <= 0 || index >= qdr->group.count)
		return 0;
	
	return qdr->group.attr[index].count;
}

//=================================================================================
// qdr_group_palette_rand
//=================================================================================
void qdr_group_palette_rand(Qdr *qdr, unsigned char offset, unsigned char limit, unsigned char alpha)
{
	int i, c;
	
	if(!qdr)
		return;
	if(offset >= limit)
		return;
	
	qdr->markpaint.type = QDR_MARKPAINT_PALETTE;
	
	srand(time(NULL));
	for(i=0; i<qdr->group.count; i++){
		if(qdr->group.attr[i].count==0)
			continue;
		
		qdr->group.attr[i].r = ((c=rand()%(limit+1)) >= offset) ? (double)c/255 : (double)offset/255;
		qdr->group.attr[i].g = ((c=rand()%(limit+1)) >= offset) ? (double)c/255 : (double)offset/255;
		qdr->group.attr[i].b = ((c=rand()%(limit+1)) >= offset) ? (double)c/255 : (double)offset/255;
		qdr->group.attr[i].a = (double)alpha/255;
	}
}

//=================================================================================
// qdr_group_palette
//=================================================================================
int qdr_group_palette(Qdr *qdr, int index, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	if(!qdr)
		return 1;
	if(index <= 0 || index >= qdr->group.count || index>256)
		return 1;

	qdr->markpaint.type = QDR_MARKPAINT_PALETTE;
	qdr->group.attr[index].r = (double)r/255;
	qdr->group.attr[index].g = (double)g/255;
	qdr->group.attr[index].b = (double)b/255;
	qdr->group.attr[index].a = (double)a/255;
	return 0;
}

//=================================================================================
// qdr_group_image
//=================================================================================
void qdr_group_image(Qdr *qdr, const char *file, int is_mark)
{
	if(!qdr)
		return;
		
	qdr->group.image = file;
	qdr->group.is_mark = !file ? 0 : is_mark;
}

//=================================================================================
// qdr_group
//=================================================================================
int qdr_group(Qdr *qdr)
{
	struct QDRGroup *g;
	int i, j;
	
	if(!qdr)
		return 1;
	
	g = &qdr->group;
	memset(g, 0, sizeof(struct QDRGroup));
	
	//グループ0はreserveなのでグループ数は初期値1
	g->count = 1;
	
	for(i=0; i<qdr->ssize; i++){
		for(j=0; j<qdr->ssize; j++){
			if(!qdr->data[j][i])
				continue;
			
			if(j<=0){
				if(i<=0)
					g->data[j][i] = g->count++;
				else
					g->data[j][i] = qdr->data[j][i-1] ? g->data[j][i-1] : g->count++;
			}else{
				if(i<=0){
					g->data[j][i] = qdr->data[j-1][i] ? g->data[j-1][i] : g->count++;
				}else{
					int a = qdr->data[j][i-1] ? 1 : 0;	//上セル
					int b = qdr->data[j-1][i] ? 1 : 0;	//左セル
					
					if(a){
						//上方向が有効なのでグルーピング
						g->data[j][i] = g->data[j][i-1];
						
						//但し、横方向も有効な時はグループ統合
						if(b){
							int x, y;
							int n = g->data[j-1][i];
							for(y=0; y<=i; y++){
								for(x=0; x<qdr->ssize; x++){
									if(g->data[x][y] != n)
										continue;
									g->attr[g->data[x][y]].count--;
									g->data[x][y] = g->data[j][i];
									g->attr[g->data[x][y]].count++;
								}
							}
						}
					}else{
						//上セルがいないので、左セルの存在状態でグループを決定する
						g->data[j][i] = b ? g->data[j-1][i] : g->count++;
					}
				}
			}
			g->attr[g->data[j][i]].count++;
		}
	}
	
	//for(i=0; i<qdr->ssize; i++)
	//	for(j=0; j<qdr->ssize; j++)
	//		printf("%03d ", qdr->group.data[j][i]);
	//for(i=0; i<qdr->group.count; i++)
	//	printf("%3d %d\n", i, qdr->group.attr[i].count);
	
	return 0;
}


//=================================================================================
// mark_paint_group
//=================================================================================
void mark_paint_group(Qdr *qdr, cairo_t *cr, cairo_pattern_t *pattern, int _x, int _y)
{
	int idx = qdr->group.data[_x][_y];
	
	if(qdr->group.attr[idx].count==1 && qdr->group.image){
		int x = (_x + qdr->margin) * qdr->msize;
		int y = (_y + qdr->margin) * qdr->msize;

		if(!pattern){
			cairo_surface_t *surface = cairo_image_surface_create_from_png(qdr->group.image);
			int w = cairo_image_surface_get_width(surface);
			int h = cairo_image_surface_get_height(surface);
			
			pattern = cairo_pattern_create_for_surface(surface);
			
			//scaling
			if(w>qdr->msize || h>qdr->msize){
				int big = w>h ? w : h;
				double scaling = (double)qdr->msize/big;
				
				cairo_matrix_t matrix;
				cairo_matrix_init_scale(&matrix, 1/scaling, 1/scaling);
				cairo_pattern_set_matrix(pattern, &matrix);
			}
			cairo_surface_destroy(surface);
		}
		
		//umm...
		if(qdr->group.is_mark){
			cairo_set_source_rgba(cr, qdr->group.attr[idx].r, qdr->group.attr[idx].g, qdr->group.attr[idx].b, qdr->group.attr[idx].a);
			cairo_fill(cr);
		}
		
		cairo_translate(cr, x, y);
			cairo_set_source(cr, pattern);
			//cairo_pattern_set_filter (cairo_get_source(cr), CAIRO_FILTER_GAUSSIAN);
			cairo_rectangle(cr, 0, 0, qdr->msize, qdr->msize);
			//cairo_fill(cr);
		cairo_translate(cr, -x, -y);
		
		return;
	}

	cairo_set_source_rgba(cr, qdr->group.attr[idx].r, qdr->group.attr[idx].g, qdr->group.attr[idx].b, qdr->group.attr[idx].a);
	cairo_fill(cr);
}
