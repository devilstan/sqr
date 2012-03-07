#include "pikl_draw.h"

//=============================================================================
//	ellipse
//=============================================================================
static int ellipse(PKLImage pkl, int x, int y, int rw, int rh, PKLColor color, int aa)
{
	int sx, sy, ex, ey, i, j, k, t, s;
	int is, os, dimm, DIV;
	
	if(rw<=0 || rh<=0) return(1);
	if(x+rw<0 || x-rw>=pkl->width || y+rh<0 || y-rh>=pkl->height) return(1);

	//if(aa < 3) return(1);
	//aa<3の時はアンチエイリアスはほとんどかからない
	DIV = aa < 3 ? 3 : aa;
	dimm = DIV*DIV;
	
	//矩形座標を算出
	sx = x-rw - DIV;
	sy = y-rh - DIV;
	ex = x+rw + DIV;
	ey = y+rh + DIV;
	
	for(i=sy; i<=ey; i++){
		if(i<0 || i>=pkl->height)
			continue;
		
		for(j=sx; j<=ex; j++){
			if(j<0 || j>=pkl->width)
				continue;
			
			os=0;
			is=0;
			
			//こっちはいい感じになるけど、1pix少ない円になる
			//for(t=-area; t<=area; t++){)
			//	for(s=-area; s<=area; s++){
			//こっちは正確なサイズにできる
			for(t=0; t<DIV; t++){
				for(s=0; s<DIV; s++){
					double dx = (double)s/(double)DIV;
					double dy = (double)t/(double)DIV;
					double tx = (aa < 3) ? j+dx-x : j-x+s+dx;
					double ty = (aa < 3) ? i+dy-y : i-y+t+dy;
					//楕円の方程式
					// x^2/rw^2 + y^2/rh^2 = 1より、座標が領域のどちらにいるかを判定
					if( (tx*tx)*(rh*rh) + (ty*ty)*(rw*rw) <= (rw*rw)*(rh*rh))
						is++;
					else
						os++;
				}
			}
			
			if(os==dimm)
				continue;
			if(is==dimm){
				memcpy(&pkl->image[(i*pkl->width+j)*pkl->channel], color->color, pkl->channel);
				continue;
			}
			
			for(k=0; k<pkl->channel; k++){
				int d = pkl->image[(i*pkl->width+j)*pkl->channel+k] * os/dimm + color->color[k] * is/dimm;
				pkl->image[(i*pkl->width+j)*pkl->channel+k] = PKL_COLOR_CLIP(d);
			}
		}
	}
	return(0);
}

//=================================================================================
// ellipse_edge
//=================================================================================
static int ellipse_edge(PKLImage pkl, int x, int y, int rw, int rh, PKLColor color, int width)
{
	unsigned char *wrk;
	int sx, sy, ex, ey, i, j, k, t, s, w, h;
	int is, os, dimm, DIV=2;
	
	if(rw<=0 || rh<=0) return(1);
	if(x+rw<0 || x-rw>=pkl->width || y+rh<0 || y-rh>=pkl->height) return(1);
	if(width>=rw || width>=rh) return(1);
	
	wrk = malloc(pkl->height * pkl->width * pkl->channel);
	if(!wrk) return(1);
	memcpy(wrk, pkl->image, pkl->height*pkl->width*pkl->channel);
	
	//まず塗りアリ円を描く
	ellipse(pkl, x, y, rw, rh, color, DIV);
	
	//次に塗りナシ円を描く
	dimm = DIV*DIV;
	w = rw - width;
	h = rh - width;
	sx = x-w - DIV;
	sy = y-h - DIV;
	ex = x+w + DIV;
	ey = y+h + DIV;
	
	for(i=sy; i<=ey; i++){
		if(i<0 || i>=pkl->height)
			continue;
		
		for(j=sx; j<=ex; j++){
			if(j<0 || j>=pkl->width)
				continue;
			
			os=0;
			is=0;
			for(t=0; t<DIV; t++){
				for(s=0; s<DIV; s++){
					double dx = (double)s/(double)DIV;
					double dy = (double)t/(double)DIV;
					double tx = j+dx-x;
					double ty = i+dy-y;
					if( (tx*tx)*(h*h) + (ty*ty)*(w*w) <= (w*w)*(h*h))
						is++;
					else
						os++;
				}
			}
			
			if(os==dimm)
				continue;
			if(is==dimm){
				memcpy(&pkl->image[(i*pkl->width+j)*pkl->channel], &wrk[(i*pkl->width+j)*pkl->channel], pkl->channel);
				continue;
			}
			
			for(k=0; k<pkl->channel; k++){
				int d = pkl->image[(i*pkl->width+j)*pkl->channel+k] * os/dimm + wrk[(i*pkl->width+j)*pkl->channel+k] * is/dimm;
				pkl->image[(i*pkl->width+j)*pkl->channel+k] = PKL_COLOR_CLIP(d);
			}
		}
	}
	free(wrk);
	return(0);
}

//=============================================================================
//	draw_ellipse
//=============================================================================
PKLExport int draw_ellipse(PKLImage pkl, int x, int y, int rw, int rh, PKLColor color, int width, int aa)
{
	int result;
	
	if(width){
		result = ellipse_edge(pkl, x, y, rw, rh, color, width);
	}else{
		result = ellipse(pkl, x, y, rw, rh, color, aa);
	}
	return result;
}

//=================================================================================
// clip_line
//=================================================================================
static int clip_line(int *x0, int *y0, int *x1, int *y1, int mindim, int maxdim)
{
	double m;
	
	if(*x0 < mindim){
		if(*x1 < mindim)
			return(1);
		m = (*y1 - *y0) / (double)(*x1 - *x0);
		*y0 -= m * (*x0 - mindim);
		*x0 = mindim;
		if(*x1 > maxdim){
			*y1 += m * (maxdim - *x1);
			*x1 = maxdim;
		}
		return(0);
	}
	
	if(*x0 > maxdim){
		if(*x1 > maxdim)
			return(1);
		m = (*y1 - *y0) / (double)(*x1 - *x0);
		*y0 += m * (maxdim - *x0);
		*x0 = maxdim;
		if(*x1 < mindim){
			*y1 -= m * (*x1 - mindim);
			*x1 = mindim;
		}
		return(0);
	}

	if(*x1 > maxdim){
		m = (*y1 - *y0) / (double)(*x1 - *x0);
		*y1 += m * (maxdim - *x1);
		*x1 = maxdim;
		return(0);
	}
	
	if(*x1 < mindim){
		m = (*y1 - *y0) / (double)(*x1 - *x0);
		*y1 -= m * (*x1 - mindim);
		*x1 = mindim;
		return(0);
	}
	return(0);
}

//=================================================================================
// line_hv
//=================================================================================
static int line_hv(PKLImage pkl, int sx, int sy, int ex, int ey, int width, PKLColor color)
{
	int i, j, tsx, tex, tsy, tey, w0, w1;
	
	w0 = -width/2;
	w1 = (width==1) ? 1 : (width%2) ? width/2+1 : width/2;
	
	if(sy == ey){
		tsx = sx;
		tex = ex+1 > pkl->width ? pkl->width : ex+1;
		tsy = (sy+w0 < 0) ? 0 : (sy+w0 > pkl->height) ? pkl->height : sy+w0;
		tey = (sy+w1 < 0) ? 0 : (sy+w1 > pkl->height) ? pkl->height : sy+w1;
	}else{
		tsx = (sx+w0 < 0) ? 0 : (sx+w0 > pkl->width) ? pkl->width : sx+w0;
		tex = (sx+w1 < 0) ? 0 : (sx+w1 > pkl->width) ? pkl->width : sx+w1;
		tsy = sy;
		tey = ey+1 > pkl->height ? pkl->height : ey+1;
	}
	
	for(i=tsy; i<tey; i++){
		for(j=tsx; j<tex; j++)
			memcpy(&pkl->image[(i*pkl->width+j)*pkl->channel], color->color, pkl->channel);
	}
	
	return(0);
}

//=================================================================================
// line
//=================================================================================
static int line(PKLImage pkl, int _sx, int _sy, int _ex, int _ey, int width, PKLColor color)
{
	int sx, sy, ex, ey, dx, dy, a;
	int i, j, e, w0, w1, s, tmp;
	
	if(width < 1)
		width=1;
	
	sx = _sx;
	ex = _ex;
	sy = _sy;
	ey = _ey;
	if(clip_line(&sx, &sy, &ex, &ey, 0, pkl->width-1))
		return(0);
	if(clip_line(&sy, &sx, &ey, &ex, 0, pkl->height-1))
		return(0);
	
	//Horizon and Vertical
	if(sx==ex || sy==ey)
		return line_hv(pkl, sx, sy, ex, ey, width, color);
		
	w0 = -width/2;
	w1 = (width==1) ? 1 : (width%2) ? width/2+1 : width/2;
	dx = ex - sx;
	dy = ey - sy;
	e = 0;

	if(abs(dx) > abs(dy)){
		if(dx < 0){
			tmp=sx; sx=ex; ex=tmp;
			tmp=sy; sy=ey; ey=tmp;
		}
		a = (sy>ey) ? -1 : 1;
		
		for(i=sy,j=sx; j<=ex; j++){
			e += abs(dy);
			if(e > abs(dx)){
				e -= abs(dx);
				i+=a;
			}
			for(s=w0; s<w1; s++){
				if(i+s>=0 && i+s<pkl->height)
					memcpy(&pkl->image[((i+s)*pkl->width+j)*pkl->channel], color->color, pkl->channel);
			}
		}
	}else{
		if(dy < 0){
			tmp=sx; sx=ex; ex=tmp;
			tmp=sy; sy=ey; ey=tmp;
		}
		a = (sx>ex) ? -1 : 1;
		for(i=sy,j=sx; i<ey; i++) {
			e += abs(dx);
			if(e > abs(dy)){
				e -= abs(dy);
				j+=a;
			}
			for(s=w0; s<w1; s++){
				if(j+s>=0 && j+s<pkl->width)
					memcpy(&pkl->image[(i*pkl->width+j+s)*pkl->channel], color->color, pkl->channel);
			}
		}
	}
	return(0);
}

////=================================================================================
//// line_aa(エッジ処理なし版)
////=================================================================================
//static int line_aa(PKLImage pkl, int _sx, int _sy, int _ex, int _ey, int width, PKLColor color, int _edge)
//{
//	int sx, sy, ex, ey;
//	int i, j, k, t, s, tmp;
//	int wt, w0, w1;
//	double f, ff, ft;
//	
//	if(width < 1)
//		width=1;
//	
//	sx = _sx;
//	ex = _ex;
//	sy = _sy;
//	ey = _ey;
//	if(clip_line(&sx, &sy, &ex, &ey, 0, pkl->width-1))
//		return(0);
//	if(clip_line(&sy, &sx, &ey, &ex, 0, pkl->height-1))
//		return(0);
//	//Horizon and Vertical
//	if(sx==ex || sy==ey)
//		return line_hv(pkl, sx, sy, ex, ey, width, color);
//	if(abs(ex-sx) > abs(ey-sy)){
//		if(_sx > _ex){
//			tmp=sx; sx=ex; ex=tmp;
//			tmp=sy; sy=ey; ey=tmp;
//		}
//		ft = (double)sy;
//		f = (sx==ex) ? 0 : (double)(ey-sy)/(double)(ex-sx);
//		
//		//太さ調整(傾きが大きくないので、太さ調整はしなくてもダイジョウブそう)
//		wt = width;
//		w0 = -(wt/2);
//		w1 = (wt==1) ? 1 : (wt%2) ? wt/2 : wt/2-1;
//
//		for(j=sx; j<=ex; j++){	// j<=ex ummm...
//			t = ft;
//			ff  = ft - t;
//			
//			if(width == 1){
//				if(t+w0-1>=0 && t+w0-1<pkl->height){
//					for(k=0; k<pkl->channel; k++)
//						pkl->image[((t+w0-1)*pkl->width+j)*pkl->channel+k] = color->color[k] * (1-ff) + pkl->image[((t+w0-1)*pkl->width+j)*pkl->channel+k] * ff;
//				}
//				if(t+w0>=0 && t+w0<pkl->height){
//					for(k=0; k<pkl->channel; k++)
//						pkl->image[((t+w0)*pkl->width+j)*pkl->channel+k] = color->color[k] * ff + pkl->image[((t+w0)*pkl->width+j)*pkl->channel+k] * (1-ff);
//				}
//			}else{
//				for(s=w0; s<w1; s++){
//					if(t+s-1>=0 && t+s-1<pkl->height){
//						for(k=0; k<pkl->channel; k++)
//							pkl->image[((t+s-1)*pkl->width+j)*pkl->channel+k] = color->color[k] * (1-ff) + pkl->image[((t+s-1)*pkl->width+j)*pkl->channel+k] * ff;
//					}
//					if(t+s>=0 && t+s<pkl->height)
//						memcpy(&pkl->image[((t+s)*pkl->width+j)*pkl->channel], color->color, pkl->channel);
//					if(t+s+1>=0 && t+s+1<pkl->height){
//						for(k=0; k<pkl->channel; k++)
//							pkl->image[((t+s+1)*pkl->width+j)*pkl->channel+k] = color->color[k] * ff + pkl->image[((t+s+1)*pkl->width+j)*pkl->channel+k] * (1-ff);
//					}
//				}
//			}
//			ft += f;
//		}
//	}else{
//		if(_sy > _ey){
//			tmp=sx; sx=ex; ex=tmp;
//			tmp=sy; sy=ey; ey=tmp;
//		}
//		ft = (double)sx;
//		f = (sy==ey) ? 0 : (double)(ex-sx)/(double)(ey-sy);
//
//		//太さ調整
//		wt = abs(sqrt( width*width + width*f * width*f));
//		w0 = (wt%2) ? -(wt/2+1) : -(wt/2);
//		w1 = (wt==1) ? 2 : (wt%2) ? wt/2+1 : wt/2;
//
//		for(i=sy; i<ey; i++){
//			t = ft;
//			ff  = ft - t;
//			
//			if(width == 1){
//				if(t+w0-1>=0 && t+w0-1<pkl->width){
//					for(k=0; k<pkl->channel; k++)
//						pkl->image[(i*pkl->width+t+w0-1)*pkl->channel+k] = color->color[k] * (1-ff) + pkl->image[(i*pkl->width+t+w0-1)*pkl->channel+k] * ff;
//				}
//				if(t+w0>=0 && t+w0<pkl->width){
//					for(k=0; k<pkl->channel; k++)
//						pkl->image[(i*pkl->width+t+w0)*pkl->channel+k] = color->color[k] * ff + pkl->image[(i*pkl->width+t+w0)*pkl->channel+k] * (1-ff);
//				}
//			}else{
//				for(s=w0; s<w1; s++){
//					if(t+s-1>=0 && t+s-1<pkl->width){
//						for(k=0; k<pkl->channel; k++)
//							pkl->image[(i*pkl->width+t+s-1)*pkl->channel+k] = color->color[k] * (1-ff) + pkl->image[(i*pkl->width+t+s-1)*pkl->channel+k] * ff;
//					}
//					if(t+s>=0 && t+s<pkl->width)
//						memcpy(&pkl->image[(i*pkl->width+t+s)*pkl->channel], color->color, pkl->channel);
//					if(t+s+1>=0 && t+s+1<pkl->width){
//						for(k=0; k<pkl->channel; k++)
//							pkl->image[(i*pkl->width+t+s+1)*pkl->channel+k] = color->color[k] * ff + pkl->image[(i*pkl->width+t+s+1)*pkl->channel+k] * (1-ff);
//					}
//				}
//			}
//			ft += f;
//		}
//	}
//	return(0);
//}
//
//=================================================================================
// blend_color
//=================================================================================
static void blend_color(int channel, unsigned char *dst, unsigned char *c1, unsigned char *c2, double weight1, double weight2)
{
	int k;
	for(k=0; k<channel; k++)
		dst[k] = c1[k] * weight1 + c2[k] * weight2;
}

//=================================================================================
// draw_line_aa
//=================================================================================
static int line_aa(PKLImage pkl, int _sx, int _sy, int _ex, int _ey, int width, PKLColor color, int _edge)
{
	int sx, sy, ex, ey;
	int i, j, t, s, tmp, edge=0;
	int wt, w0, w1, sn, en, p0, p1, p2;
	double f, ff, ft, a, we0, we1;
	
	if(width < 1)
		width=1;

	if(width>3 && _edge)
		edge=1;

	sx = _sx;
	ex = _ex;
	sy = _sy;
	ey = _ey;
	if(clip_line(&sx, &sy, &ex, &ey, 0, pkl->width-1))
		return(0);
	if(clip_line(&sy, &sx, &ey, &ex, 0, pkl->height-1))
		return(0);
	
	//Horizon and Vertical
	if(sx==ex || sy==ey)
		return line_hv(pkl, sx, sy, ex, ey, width, color);
	
	if(abs(ex-sx) > abs(ey-sy)){
		if(_sx > _ex){
			tmp=sx; sx=ex; ex=tmp;
			tmp=sy; sy=ey; ey=tmp;
		}
		f = (double)(ey-sy)/(double)(ex-sx);
		
		//太さ調整(傾きが大きくないので、太さ調整はしなくてもダイジョウブそう)
		wt = width;
		w0 = -(wt/2);
		w1 = (wt==1) ? 1 : (wt%2) ? wt/2 : wt/2-1;

		//直行する直線の傾きとx方向の範囲補正
		a = -1/f;
		sn = sx;
		en = ex;
		ft = (double)sy;
		if(edge){
			sn = sx - width;
			en = ex + width;
			sn = sn<0 ? 0 : sn>=pkl->width ? pkl->width : sn;
			en = en<0 ? 0 : en>=pkl->width ? pkl->width : en;
			//x 方向に位置調整したので、y 起点の調整
			ft = f * sn + (sy - f * sx);
		}

		for(j=sn; j<en; j++){
			t = ft;
			ff = fabs(ft - t);
			
			if(width == 1){
				if(PKL_RANGE_CHECK(t+w0-1, 0, pkl->height-1))
					blend_color(pkl->channel, &pkl->image[((t+w0-1)*pkl->width+j)*pkl->channel], color->color, &pkl->image[((t+w0-1)*pkl->width+j)*pkl->channel], 1-ff, ff);
				if(PKL_RANGE_CHECK(t+w0, 0, pkl->height-1))
					blend_color(pkl->channel, &pkl->image[((t+w0)*pkl->width+j)*pkl->channel], color->color, &pkl->image[((t+w0)*pkl->width+j)*pkl->channel], ff, 1-ff);
			}else{
				for(s=w0; s<w1; s++){
					//エッジ処理
					if(edge){
						double x1 = (double)j - ((t+s) - (sy - a * sx))/a;
						double x2 = ((t+s) - (ey - a * ex))/a - (double)j;
						if(x1<0.0 || x2<0.0)
							continue;
						if(x1<1.0 && (j-1>=0 && j-1<pkl->width && t+s>=0 && t+s<pkl->width))
							blend_color(pkl->channel, &pkl->image[((t+s)*pkl->width+j-1)*pkl->channel], color->color, &pkl->image[((t+s)*pkl->width+j-1)*pkl->channel], x1, 1-x1);
						else if(x2<1.0 && (j+1>=0 && j+1<pkl->width && t+s>=0 && t+s<pkl->width))
							blend_color(pkl->channel, &pkl->image[((t+s)*pkl->width+j+1)*pkl->channel], color->color, &pkl->image[((t+s)*pkl->width+j+1)*pkl->channel], x2, 1-x2);
					}
					
					if(ft-t >= 0){
						p0=t+s-1; p1=t+s; p2=t+s+1;
						we0=1-ff; we1=ff;
					}else{
						p0=t+s-2; p1=t+s-1; p2=t+s;
						we0=ff; we1=1-ff;
					}
					if(PKL_RANGE_CHECK(p0, 0, pkl->height-1))
						blend_color(pkl->channel, &pkl->image[(p0*pkl->width+j)*pkl->channel], color->color, &pkl->image[(p0*pkl->width+j)*pkl->channel], we0, we1);
					if(PKL_RANGE_CHECK(p1, 0, pkl->height-1))
						memcpy(&pkl->image[(p1*pkl->width+j)*pkl->channel], color->color, pkl->channel);
					if(PKL_RANGE_CHECK(p2, 0, pkl->height-1))
						blend_color(pkl->channel, &pkl->image[(p2*pkl->width+j)*pkl->channel], color->color, &pkl->image[(p2*pkl->width+j)*pkl->channel], we1, we0);
				}
			}
			ft += f;
		}
	}else{
		if(_sy > _ey){
			tmp=sx; sx=ex; ex=tmp;
			tmp=sy; sy=ey; ey=tmp;
		}
		f = (double)(ex-sx)/(double)(ey-sy);
		
		//太さ調整
		wt = abs(sqrt(width*width + width*f * width*f)) - 1;
		w0 = (wt%2) ? -(wt/2+1) : -(wt/2);
		w1 = (wt==1) ? 2 : (wt%2) ? wt/2+1 : wt/2;
		
		//a=直行する直線の傾き & y方向の範囲補正
		a = -f;
		sn = sy;
		en = ey;
		ft = (double)sx;
		if(edge){
			sn = sy - width;
			en = ey + width;
			sn = sn<0 ? 0 : sn>=pkl->height ? pkl->height : sn;
			en = en<0 ? 0 : en>=pkl->height ? pkl->height : en;
			//y 方向に位置調整したので、x 起点の調整
			ft = f * (double)(sn - sy) + (double)sx;
		}
		
		for(i=sn; i<en; i++){
			t = ft;
			ff = fabs(ft - t);
			
			if(width == 1){
				if(PKL_RANGE_CHECK(t+w0-1, 0, pkl->width-1))
					blend_color(pkl->channel, &pkl->image[(i*pkl->width+t+w0-1)*pkl->channel], color->color, &pkl->image[(i*pkl->width+t+w0-1)*pkl->channel], 1-ff, ff);
				if(PKL_RANGE_CHECK(t+w0, 0, pkl->width-1))
					blend_color(pkl->channel, &pkl->image[(i*pkl->width+t+w0)*pkl->channel], color->color, &pkl->image[(i*pkl->width+t+w0)*pkl->channel], ff, 1-ff);
			}else{
				for(s=w0; s<w1; s++){
					//エッジ処理
					if(edge){
						double y1 = (double)i - (a * (t+s) + (sy - a * sx));
						double y2 = a * (t+s) + (ey - a * ex) - (double)i;
						if(y1<0.0 || y2<0.0)
							continue;
						if(y1<1.0 && (i-1>=0 && i-1<pkl->height && t+s>=0 && t+s<pkl->width))
							blend_color(pkl->channel, &pkl->image[((i-1)*pkl->width+t+s)*pkl->channel], color->color, &pkl->image[((i-1)*pkl->width+t+s)*pkl->channel], y1, 1-y1);
						else if(y2<1.0 && (i+1>=0 && i+1<pkl->height && t+s>=0 && t+s<pkl->width))
							blend_color(pkl->channel, &pkl->image[((i+1)*pkl->width+t+s)*pkl->channel], color->color, &pkl->image[((i+1)*pkl->width+t+s)*pkl->channel], y2, 1-y2);
					}
					
					if(ft-t >= 0){
						p0=t+s-1; p1=t+s; p2=t+s+1;
						we0=1-ff; we1=ff;
					}else{
						p0=t+s-2; p1=t+s-1; p2=t+s;
						we0=ff; we1=1-ff;
					}
					if(PKL_RANGE_CHECK(p0, 0, pkl->width-1))
						blend_color(pkl->channel, &pkl->image[(i*pkl->width+p0)*pkl->channel], color->color, &pkl->image[(i*pkl->width+p0)*pkl->channel], we0, we1);
					if(PKL_RANGE_CHECK(p1, 0, pkl->width-1))
						memcpy(&pkl->image[(i*pkl->width+p1)*pkl->channel], color->color, pkl->channel);
					if(PKL_RANGE_CHECK(p2, 0, pkl->width-1))
						blend_color(pkl->channel, &pkl->image[(i*pkl->width+p2)*pkl->channel], color->color, &pkl->image[(i*pkl->width+p2)*pkl->channel], we1, we0);
				}
			}
			ft += f;
		}
	}
	return(0);
}

//=================================================================================
// draw_line
//=================================================================================
PKLExport int draw_line(PKLImage pkl, int _sx, int _sy, int _ex, int _ey, int width, PKLColor color, int aa, int edge)
{
	struct _PKLColor c;
	
	if(!color)
		memset(c.color, 0, pkl->channel);
	else
		memcpy(c.color, color->color, pkl->channel);
	
	if(aa)
		return line_aa(pkl, _sx, _sy, _ex, _ey, width, &c, edge);

	return line(pkl, _sx, _sy, _ex, _ey, width, &c);
}

//=================================================================================
// draw_rect
//=================================================================================
PKLExport int draw_rect(PKLImage pkl, int sx, int sy, int ex, int ey, int width, PKLColor color, int fill)
{
	if(!color)
		return(1);

	if(fill){
		int i, j;
		
		if(ex < sx){
			int t=sx;
			sx=ex; ex=t;
		}
		if(ey < sy){
			int t=sy;
			sy=ey; ey=t;
		}
		
		//画像外なので描画の必要なし
		if(sx>pkl->width || sy>pkl->height || ex<0 || ey<0)
			return(0);
		//クリッピング
		sx = sx<0 ? 0 : sx;
		sy = sy<0 ? 0 : sy;
		ex = ex>pkl->width -1 ? pkl->width -1 : ex;
		ey = ey>pkl->height-1 ? pkl->height-1 : ey;
		
		for(i=sy; i<=ey; i++)
			for(j=sx; j<=ex; j++)
				memcpy(&pkl->image[(i*pkl->width+j)*pkl->channel], color->color, pkl->channel);
		
	}else{
		int x1h=sx, x1v=sx, y1h=sy, y1v=sy;
		int x2h=ex, x2v=ex, y2h=ey, y2v=ey;
		int h0, h1;
		
		if(width > 1){
			h0 = width / 2;
			h1 = width - h0;
			if(sy < ey){
				y1v = y1h - h0;
				y2v = y2h + h1;
			}else{
				y1v = y1h + h1;
				y2v = y2h - h0;
			}
		}
		draw_line(pkl, x1h, y1h, x2h, y1h, width, color, 0, 0);
		draw_line(pkl, x1h, y2h+1, x2h, y2h+1, width, color, 0, 0);
		draw_line(pkl, x1v, y1v, x1v, y2v, width, color, 0, 0);
		draw_line(pkl, x2v, y1v, x2v, y2v, width, color, 0, 0);
	}
	return(0);
}


////=================================================================================
//// draw_polygon
////=================================================================================
//PKLExport int draw_polygon(PKLImage pkl, PKLPoint *point, int num, PKLColor color, int width, int closed)
//{
//	int i, w0, w1;
//	
//	if(num<=1 || !point)
//		return(1);
//
//	if(closed && num>=3)
//		draw_line(pkl, point[num-1].x, point[num-1].y, point[0].x, point[0].y, width, color, 1, 1);
//
//	ellipse(pkl, point[0].x, point[0].y, width/2, width/2, color, 1);
//
//	for(i=1; i<num; i++){
//		draw_line(pkl, point[i-1].x, point[i-1].y, point[i].x, point[i].y, width, color, 1, 1);
//		ellipse(pkl, point[i].x, point[i].y, width/2, width/2, color, 1);
//	}
//	return(0);
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define IMTRUNC(x) ((int)((x)*16))
//#define coarse(x) ((x)/16)
//
//typedef struct {
//	int n;
//	double x;
//} p_slice;
//
//typedef struct {
//	int *line;
//	int linelen;
//} ss_scanline;
//
//typedef struct {
//	int n;
//	int x1,y1;
//	int x2,y2;
//	int miny,maxy;
//	int minx,maxx;
//	int updown;
//} p_line;
//
////=================================================================================
//// p_eval_aty
////=================================================================================
//static double p_eval_aty(p_line *l, int y)
//{
//	int t;
//	t=l->y2-l->y1;
//	if(t)
//		return( (y-l->y1)*l->x2 + (l->y2-y)*l->x1 )/t;
//	return (l->x1+l->x2)/2.0;
//}
//
////=================================================================================
//// p_eval_atx
////=================================================================================
//static double p_eval_atx(p_line *l, int x)
//{
//	int t;
//	t = l->x2-l->x1;
//	if(t)
//		return ( (x-l->x1)*l->y2 + (l->x2-x)*l->y1 )/t;
//	return (l->y1+l->y2)/2.0;
//}
//
////=================================================================================
//// lines_in_interval
////=================================================================================
//static int lines_in_interval(p_line *lset, int l, p_slice *tllist, int minc, int maxc)
//{
//	int k;
//	int count = 0;
//	for(k=0; k<l; k++){
//		if(lset[k].maxy > minc && lset[k].miny < maxc){
//			if(lset[k].miny != lset[k].maxy){
//				tllist[count].x=p_eval_aty(&lset[k],(minc+maxc)/2.0);
//				tllist[count].n=k;
//				count++;
//			}
//		}
//	}
//	return count;
//}
//
////=================================================================================
//// mark_updown_slices
////=================================================================================
//static void mark_updown_slices(p_line *lset, p_slice *tllist, int count)
//{
//	p_line *l, *r;
//	int k;
//	
//	for(k=0; k<count; k+=2){
//		l = lset + tllist[k].n;
//
//		if (l->y1 == l->y2)
//		  exit(3);
//
//		l->updown = (l->x1 == l->x2) ? 0 : (l->x1 > l->x2) ? (l->y1 > l->y2) ? -1 : 1 : (l->y1 > l->y2) ? 1 : -1;
//
//		if(k+1 >= count)
//			return;
//
//		r = lset + tllist[k+1].n;
//		if(r->y1 == r->y2)
//			exit(3);
//		r->updown = (r->x1 == r->x2) ? 0 : (r->x1 > r->x2) ? (r->y1 > r->y2) ? -1 : 1 : (r->y1 > r->y2) ? 1 : -1;
//	}
//}
//
////=================================================================================
//// p_compy
////=================================================================================
//static int p_compy(const void *_p1, const void *_p2)
//{
//	PKLPoint *p1 = (PKLPoint *)_p1;
//	PKLPoint *p2 = (PKLPoint *)_p2;
//	return(p1->y - p2->y);
//}
////=================================================================================
//// p_compx
////=================================================================================
//static int p_compx(const void *_p1, const void *_p2)
//{
//	p_slice *p1 = (p_slice *)_p1;
//	p_slice *p2 = (p_slice *)_p2;
//	return(p1->x - p2->x);
//}
//
////=================================================================================
//// flusher
////=================================================================================
//static void flusher(PKLImage pkl, ss_scanline *ss, int y, PKLColor color)
//{
//	int x, ch, tv;
//	
//	for(x=0; x<pkl->width; x++) {
//		tv = PKL_COLOR_CLIP(ss->line[x]);
//		for(ch=0; ch<pkl->channel; ch++)
//			pkl->image[(y*pkl->width+x)*pkl->channel+ch]
//				= tv/255.0 * color->color[ch] + (1.0-tv/255.0) * pkl->image[(y*pkl->width+x)*pkl->channel+ch];
//	}
//}
//
////=================================================================================
//// trap_square
////=================================================================================
//static int trap_square(int xlen, int ylen, double xl, double yl)
//{
//	return xlen*ylen-(xl*yl)/2.0;
//}
//
////=================================================================================
//// pixel_coverage
////=================================================================================
//static int pixel_coverage(p_line *line, int minx, int maxx, int  miny, int maxy)
//{
//	double lycross, rycross;
//	int l, r;
//
//	if (!line->updown) {
//		l = r = 0;
//	} else {
//		lycross = p_eval_atx(line, minx);
//		rycross = p_eval_atx(line, maxx);
//		l = lycross <= maxy && lycross >= miny; /* true if it enters through left side */
//		r = rycross <= maxy && rycross >= miny; /* true if it enters through left side */
//	}
//
//	if (l && r)
//		return line->updown == 1 ? 
//			(double)(maxx-minx) * (2.0*maxy-lycross-rycross)/2.0 : (double)(maxx-minx) * (lycross+rycross-2*miny)/2.0;  /* down case */
//
//		if(!l && !r)
//			return (maxy-miny)*(maxx*2-p_eval_aty(line, miny)-p_eval_aty(line, maxy))/2.0;
//
//	if (l && !r)
//		return line->updown == 1 ?
//			trap_square(maxx-minx, maxy-miny, p_eval_aty(line, miny)-minx, p_eval_atx(line, minx)-miny) : trap_square(maxx-minx, maxy-miny, p_eval_aty(line, maxy)-minx, maxy-p_eval_atx(line, minx));
//
//	if (!l && r){
//		int r = line->updown == 1 ?
//			(maxx-p_eval_aty(line, maxy))*(maxy-p_eval_atx(line, maxx))/2.0 : 
//			(maxx-p_eval_aty(line, miny))*(p_eval_atx(line, maxx)-miny)/2.0;
//		return r;
//	}
//	return 0; /* silence compiler warning */
//}
//
//
////=================================================================================
//// render_slice_scanline
////=================================================================================
//static void render_slice_scanline(ss_scanline *ss, int y, p_line *l, p_line *r, int miny, int maxy)
//{
//	int lminx, lmaxx, rminx, rmaxx;
//	int cpix, thin, startpix, stoppix;
//
//	lminx = PKL_MIN( p_eval_aty(l, maxy), p_eval_aty(l, miny) );
//	lmaxx = PKL_MAX( p_eval_aty(l, maxy), p_eval_aty(l, miny) );
//	rminx = PKL_MIN( p_eval_aty(r, maxy), p_eval_aty(r, miny) );
//	rmaxx = PKL_MAX( p_eval_aty(r, maxy), p_eval_aty(r, miny) );
//
//	thin = coarse(lmaxx) >= coarse(rminx);
//
//	startpix = PKL_MAX( coarse(lminx), 0 );
//	stoppix  = PKL_MIN( coarse(rmaxx-1), ss->linelen-1 );
//
//	for(cpix=startpix; cpix<=stoppix; cpix++) {
//		int lt = coarse(lmaxx-1) >= cpix;
//		int rt = coarse(rminx) <= cpix;
//		int A, B, C;
//
//		A = lt ? pixel_coverage(l, cpix*16, cpix*16+16, miny, maxy) : 0;
//		B = lt ? 0 : 16*(maxy-miny);
//		C = rt ? pixel_coverage(r, cpix*16, cpix*16+16, miny, maxy) : 0;
//
//		ss->line[cpix] += A+B-C;
//	}
//}
//
////=================================================================================
//// draw_polygon
////=================================================================================
//PKLExport int draw_polygon(PKLImage pkl, PKLPoint *point, int num, PKLColor color, int width, int closed, int fill)
//{
//	int i ,k, clc;
//	int tempy=0, cscl=0;
//	ss_scanline templine;
//	PKLPoint *pset;
//	p_line  *lset;
//	p_slice *tllist;
//
//	tllist  = malloc(sizeof(p_slice) * num);
//
//	templine.line = malloc(sizeof(int) * pkl->width);
//	memset(templine.line, 0, sizeof(int)*pkl->width);
//	templine.linelen = pkl->width;
//
//	pset = malloc(sizeof(PKLPoint) * num);
//	for(i=0; i<num; i++) {
//		pset[i].x=IMTRUNC(point[i].x);
//		pset[i].y=IMTRUNC(point[i].y);
//	}
//	qsort(pset, num, sizeof(PKLPoint), p_compy);
//
//	lset = malloc(sizeof(p_line) * num);
//	for(i=0; i<num; i++) {
//		lset[i].x1 = IMTRUNC(point[i].x);
//		lset[i].y1 = IMTRUNC(point[i].y);
//		lset[i].x2 = IMTRUNC(point[(i+1)%num].x);
//		lset[i].y2 = IMTRUNC(point[(i+1)%num].y);
//		lset[i].miny=PKL_MIN(lset[i].y1,lset[i].y2);
//		lset[i].maxy=PKL_MAX(lset[i].y1,lset[i].y2);
//		lset[i].minx=PKL_MIN(lset[i].x1,lset[i].x2);
//		lset[i].maxx=PKL_MAX(lset[i].x1,lset[i].x2);
//	}
//
//	for(i=0; i<num-1; i++){
//		int startscan = PKL_MAX( coarse(pset[i].y), 0);
//		int stopscan = PKL_MIN( coarse(pset[i+1].y+15), pkl->height);
//		int miny, maxy;
//
//		if(pset[i].y == pset[i+1].y)
//			continue;
//
//		clc = lines_in_interval(lset, num, tllist, pset[i].y, pset[i+1].y);
//		qsort(tllist, clc, sizeof(p_slice), p_compx);
//
//		mark_updown_slices(lset, tllist, clc);
//
//		maxy = pkl->height * 16;
//		miny = 0;
//		for (k=0; k<clc; ++k) {
//			p_line const * line = lset + tllist[k].n;
//			if(line->miny > miny)
//				miny = line->miny;
//			if(line->maxy < maxy)
//				maxy = line->maxy;
//		}
//
//		for(cscl=startscan; cscl<stopscan; cscl++){
//			int scan_miny = PKL_MAX(miny, cscl*16);
//			int scan_maxy = PKL_MIN(maxy, (cscl+1) * 16);
//
//			tempy = PKL_MIN(cscl*16+16, pset[i+1].y);
//			for(k=0; k<clc-1; k+=2){
//				render_slice_scanline(&templine, cscl, lset+tllist[k].n, lset+tllist[k+1].n, scan_miny, scan_maxy);
//			}
//			if(16*coarse(tempy) == tempy){
//				flusher(pkl, &templine, cscl, color);
//				memset(templine.line, 0, sizeof(int) * templine.linelen);
//			}
//		}
//	}
//
//	if(16*coarse(tempy) != tempy) 
//		flusher(pkl, &templine, cscl-1, color);
//
//	free(templine.line);
//	free(pset);
//	free(lset);
//	free(tllist);
//	return(0);
//}
//
//
