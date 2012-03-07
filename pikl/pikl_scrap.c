#include "pikl_scrap.h"

static unsigned char lighting(unsigned char color, double dx, double dy, double dz, double mil, double env, double *light);

//=============================================================================
// pkl_rgb2hsb(色相:Hue/彩度:Saturation/明度:Brightness)
//=============================================================================
static float *pkl_rgb2hsb(unsigned char *rgb, float *hsb)
{
	#define _max(a,b) (a>b) ? a : b
	#define _min(a,b) (a>b) ? b : a

	float hue, saturation, brightness;
	unsigned char color_max, color_min;
	unsigned char r, g, b;
	int diff;
	
	r = rgb[0];
	g = rgb[1];
	b = rgb[2];
	
	color_max= _max(_max(r, g), b);
	color_min= _min(_min(r, g), b);
	diff= color_max - color_min;
	
	hue=0.0;
	if(diff != 0){
		if(color_max==r) hue=60*(g-b)/(float)diff;
		if(color_max==g) hue=60*(b-r)/(float)diff + 120.0;
		if(color_max==b) hue=60*(r-g)/(float)diff + 240.0;
	}
	saturation = color_max!=0 ? diff/(float)color_max : 0.0;
	brightness = color_max / 255.0;

	hsb[0] = hue;
	hsb[1] = saturation;
	hsb[2] = brightness;

	return(hsb);
}

//=============================================================================
// pkl_hsb2rgb
//=============================================================================
static unsigned char *pkl_hsb2rgb(float *hsb, unsigned char *rgb)
{
	float h, s, b, f;
	int br, p, q, t, hue;

	h = hsb[0];
	s = hsb[1];
	b = hsb[2];
	
	h = (h>360.0) ? h-360.0 : (h<0.0) ? h+360.0 : h;
	s = (s>1.0) ? 1.0 : (s<0.0) ? 0.0 : s;
	br=(int)(255*b);
	br = PKL_COLOR_CLIP(br);

	if(s==0.0){
		rgb[0]=br;
		rgb[1]=br;
		rgb[2]=br;
	}else{
		hue=(int)(h/60.0);
		f=h/60.0-hue;
		
		p=(int)(br*(1.0-s));
		p = PKL_COLOR_CLIP(p);
		
		q=(int)(br*(1.0-f*s));
		q = PKL_COLOR_CLIP(q);
		
		t=(int)(br*(1.0-(1.0-f)*s));
		t = PKL_COLOR_CLIP(t);
		
		switch(hue){
			case 1:rgb[0]=q;  rgb[1]=br; rgb[2]=p; break;
			case 2:rgb[0]=p;  rgb[1]=br; rgb[2]=t; break;
			case 3:rgb[0]=p;  rgb[1]=q;  rgb[2]=br; break;
			case 4:rgb[0]=t;  rgb[1]=p;  rgb[2]=br; break;
			case 5:rgb[0]=br; rgb[1]=p;  rgb[2]=q; break;
			case 0:
			default:
				rgb[0]=br; rgb[1]=t; rgb[2]=p; break;
		}
	}
	return(rgb);
}

//=============================================================================
// pkl_sobelpaint
//=============================================================================
PKLExport int pkl_sobelpaint(PKLImage pkl, int edgeweight, float mix, float saturation, float hue)
{
	//Sobel Filter
	double Fx[3][3] = { {-0.25,   0.0,  0.25},
						{-0.50,   0.0,  0.50},
						{-0.25,   0.0,  0.25}};
	double Fy[3][3] = { {-0.25, -0.50, -0.25},
						{  0.0,   0.0,   0.0},
						{ 0.25,  0.50,  0.25}};
	double kg[3] = {0.298912, 0.586611, 0.114478};
	double dx, dy, dR, dG, dB, weight;
	int i, j, tx, ty, offset=1;
	unsigned char *wrk;
	float hsb[3], bright;

	if(pkl->color != PKL_RGB) return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0xff, pkl->width*pkl->height*pkl->channel);

	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			if(j-offset<0 || j+offset>=pkl->width || i-offset<0 || i+offset>=pkl->height) continue;
			
			dR=dG=dB=0.0;
			for(ty=-offset; ty<=offset; ty++){
				for(tx=-offset; tx<=offset; tx++){
					//if(j+tx<0 || j+tx>=pkl->width || i+ty<0 || i+ty>=pkl->height) continue;
					weight = Fx[ty+offset][tx+offset];
					dR += (pkl->image[((i+ty)*pkl->width+j+tx)*pkl->channel+0] * weight);
					dG += (pkl->image[((i+ty)*pkl->width+j+tx)*pkl->channel+1] * weight);
					dB += (pkl->image[((i+ty)*pkl->width+j+tx)*pkl->channel+2] * weight);
				}
			}
			dx = kg[0]*dR + kg[1]*dG + kg[2]*dB;
			
			dR=dG=dB=0.0;
			for(ty=-offset; ty<=offset; ty++){
				for(tx=-offset; tx<=offset; tx++){
					//if(j+tx<0 || j+tx>=pkl->width || i+ty<0 || i+ty>=pkl->height) continue;
					weight = Fy[ty+offset][tx+offset];
					dR += (pkl->image[((i+ty)*pkl->width+j+tx)*pkl->channel+0] * weight);
					dG += (pkl->image[((i+ty)*pkl->width+j+tx)*pkl->channel+1] * weight);
					dB += (pkl->image[((i+ty)*pkl->width+j+tx)*pkl->channel+2] * weight);
				}
			}
			dy = kg[0]*dR + kg[1]*dG + kg[2]*dB;
			
			//RGB->HSB
			pkl_rgb2hsb(&pkl->image[(i*pkl->width+j)*pkl->channel], hsb);

			//明度にのみsobelを適用して元明度と合成する
			bright = hsb[2];
			hsb[2] = sqrt(dx*dx+dy*dy) * edgeweight * 0.01;
			hsb[2] = (hsb[2]>1.0) ? 0.0 : 1.0-hsb[2];
			hsb[2] = hsb[2]*(1.0-mix) + bright*mix;

			//彩度調整
			hsb[1] *= saturation;
			
			//色相調整
			hsb[0] += hue;
			
			//HSB->RGB
			pkl_hsb2rgb(hsb, &wrk[(i*pkl->width+j)*pkl->channel]);
		}
	}
	free(pkl->image);
	pkl->image=wrk;
	return(0);
}

//=============================================================================
// pkl_illust
//=============================================================================
PKLExport int pkl_illust(PKLImage pkl, int gap, int edge, int gammaint)
{
	int sobel1[9] = { 1, 0, -1, 2, 0, -2,  1,  0, -1 };	//sobel filter
	int sobel2[9] = { 1, 2,  1, 0, 0,  0, -1, -2, -1 };	//sobel filter
	int i, j, k, s, t, px, py, rp, r1, r2, gt;
	int wx[PKL_CHANNEL], wy[PKL_CHANNEL], rate[PKL_CHANNEL];
	unsigned char *wrk;
	double gamma;
	int th1, th2, th3;

	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	
	gamma = 1.0/((double)gammaint/100.0);
	th1 = (int)(pow(128.0/255.0, gamma)*255.0);
	th2 = (int)(pow( 96.0/255.0, gamma)*255.0);
	th3 = (int)(pow( 64.0/255.0, gamma)*255.0);
	
	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			memset(wx, 0, sizeof(wx));
			memset(wy, 0, sizeof(wy));
			
			for(s=0; s<3; s++){
				for(t=0; t<3; t++){
					px = (j+t-1)<0 ? 0 : (j+t-1)>=pkl->width ? pkl->width-1 : (j+t-1);
					py = (i+s-1)<0 ? 0 : (i+s-1)>=pkl->height ? pkl->height-1 : (i+s-1);
					for(k=0; k<pkl->channel; k++){
						wx[k] += pkl->image[(py*pkl->width+px)*pkl->channel+k] * sobel1[s*3+t];
						wy[k] += pkl->image[(py*pkl->width+px)*pkl->channel+k] * sobel2[s*3+t];
					}
				}
			}
			
			for(k=0; k<pkl->channel; k++)
				rate[k] = (int)(sqrt((double)(wx[k]*wx[k]+wy[k]*wy[k]))/8.0);
			rp = 0;
			for(k=0; k<pkl->channel; k++)
				if(rate[k]>rp) rp=rate[k];
			r1 = (rp<edge) ? 255 : (rp<edge+gap) ? 255-255*(rp-edge)/gap : 0;

			gt = 0;
			for(k=0; k<pkl->channel; k++)
				gt += pkl->image[(i*pkl->width+j)*pkl->channel+k];
			gt /= pkl->channel;
			r2 = (gt>th1) ? 255 : (gt>th2) ? 128 : (gt>th3) ? 64 : 0;
			
			for(k=0; k<pkl->channel; k++){
				int d = pkl->image[(i*pkl->width+j)*pkl->channel+k] * r1/256*r2/256;
				wrk[(i*pkl->width+j)*pkl->channel+k] = PKL_COLOR_CLIP(d);
			}
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=============================================================================
// pkl_rock
//=============================================================================
PKLExport int pkl_rock(PKLImage pkl, double mil, double env)
{
	int sobel1[9] = { 1, 0, -1, 2, 0, -2,  1,  0, -1 };	//sobel filter
	int sobel2[9] = { 1, 2,  1, 0, 0,  0, -1, -2, -1 };	//sobel filter
	int i, j, k, s, t, wx, wy, gt, px, py;
	unsigned char *wrk, color[PKL_CHANNEL];
	double dx, dy, dz, light[3], abl;

	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);
	
	//光源ベクトルを単位ベクトルに補正する
	abl = sqrt(-1.0*-1.0 + -1.0*-1.0 + 1.0*1.0);
	light[0] = -1.0/abl;
	light[1] = -1.0/abl;
	light[2] =  1.0/abl;

	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			wx=wy=0;
			for(s=0; s<3; s++){
				for(t=0; t<3; t++){
					px = (j+t-1)<0 ? 0 : (j+t-1)>=pkl->width ? pkl->width-1 : (j+t-1);
					py = (i+s-1)<0 ? 0 : (i+s-1)>=pkl->height ? pkl->height-1 : (i+s-1);
					
					gt=0;
					for(k=0; k<pkl->channel; k++)
						gt += pkl->image[(py*pkl->width+px)*pkl->channel+k];
					gt /= pkl->channel;
					wx += gt*sobel1[s*3+t];
					wy += gt*sobel2[s*3+t];
				}
			}
			dx = (double)wx;
			dy = (double)wy;
			dz = 16.0;
			for(k=0; k<pkl->channel; k++)
				color[k] = lighting(pkl->image[(py*pkl->width+px)*pkl->channel+k], dx, dy, dz, mil, env, light);
			memcpy(&wrk[(i*pkl->width+j)*pkl->channel], color, pkl->channel);
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	
	return(0);
}

//=============================================================================
// 陰影付け
// (dx,dy,dz)を法線ベクトルとする（単位ベクトルでなくてもよい）
//=============================================================================
static unsigned char lighting(unsigned char color, double dx, double dy, double dz,
                                                    double mil, double env, double *light)
{
	double abl, rz, env2, cosT, cosT2;
	int res;

	//法線ベクトルを単位ベクトルに補正
	abl = sqrt(dx*dx + dy*dy + dz*dz);
	cosT = (dx/abl)*light[0] + (dy/abl)*light[1] + (dz/abl)*light[2];
	rz = 2.0*cosT*(dz/abl) - light[2];
	cosT2 = (rz>0.0) ? pow(rz, 12.0) : 0.0;
	cosT  = (cosT<0.0) ? 0.0 : cosT;
	
	env2=env*255.0/100.0;
	res=(int)((cosT*255.0+env2)*(double)color/255.0);
	
	if(res>color) res=color;
	res+=(mil*cosT2);
	
	return(PKL_COLOR_CLIP(res));
}

//=============================================================================
// pkl_swirl
//=============================================================================
PKLExport int pkl_swirl(PKLImage pkl, double factor, int x, int y, PKLColor backcolor, PKL_SAMPLE sample)
{
	unsigned char *wrk, color[PKL_CHANNEL];
	int i, j, tx, ty;
	double range, rad, fx, fy;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	if(fabs(factor) > 1) return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);
	
	if(backcolor)
		memcpy(color, backcolor->color, pkl->channel);
	else
		memset(color, 0xff, pkl->channel);
	
	//中心座標
	tx = (x<0 || x>=pkl->width) ? pkl->width/2 : x;
	ty = (y<0 || y>=pkl->height) ? pkl->height/2 : y;
	
	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			range = sqrt((j-tx)*(j-tx) + (i-ty)*(i-ty));
			rad = atan2(i-ty, j-tx) + (factor * range);
			fx = range * cos(rad) + tx;
			fy = range * sin(rad) + ty;
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, color);
		}
	}

	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=============================================================================
// pkl_wave
//=============================================================================
PKLExport int pkl_wave(PKLImage pkl, PKL_WAVE mode, double factor, double frequency, PKLColor backcolor, PKL_SAMPLE sample)
{
	unsigned char *wrk, color[PKL_CHANNEL];
	int i, j;
	double fx, fy;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);
	
	if(backcolor)
		memcpy(color, backcolor->color, pkl->channel);
	else
		memset(color, 0xff, pkl->channel);
	
	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			fx = (double)j;
			fy = (double)i;
			
			switch(mode){
				case PKL_WAVE_HORZ:
					fy += factor * sin(frequency * M_PI/180.0 * j);
					break;
				case PKL_WAVE_VERT:
					fx += factor * sin(frequency * M_PI/180.0 * i);
					break;
				case PKL_WAVE_BOTH:
				default:
					fx += factor * sin(frequency * M_PI/180.0 * i);
					fy += factor * sin(frequency * M_PI/180.0 * j);
			}
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, color);
		}
	}

	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=================================================================================
// pkl_cylinder
//=================================================================================
PKLExport int pkl_cylinder(PKLImage pkl, PKL_CYLINDER_TYPE type, PKL_SAMPLE sample)
{
	unsigned char *wrk, color[PKL_CHANNEL];
	int i, j;
	double half, s, a, fx, fy;
	PKL_CYLINDER_TYPE hz, vt;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	hz = (type==PKL_CYLINDER_HORZ || type==PKL_CYLINDER_BOTH) ? 1 : 0;
	vt = (type==PKL_CYLINDER_VERT || type==PKL_CYLINDER_BOTH) ? 1 : 0;
	if(!hz && !vt)
		return(1);
	
	memset(color, 0, PKL_CHANNEL);
	
	//horizon変形
	if(hz){
		wrk = malloc(pkl->width*pkl->height*pkl->channel);
		if(!wrk) return(1);
		memset(wrk, 0, pkl->width*pkl->height*pkl->channel);
	
		half = (double)pkl->height/2.0;
		for(i=0; i<pkl->height; i++){
			s = (double)i - half;
			a = atan2(sqrt(half*half-s*s), s);
			fy = 2.0 * half * (1.0 - a / M_PI);
			
			for(j=0; j<pkl->width; j++){
				fx = (double)j;
				op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, color);
			}
		}
		free(pkl->image);
		pkl->image = wrk;
	}
	//vertical変形
	if(vt){
		wrk = malloc(pkl->width*pkl->height*pkl->channel);
		if(!wrk) return(1);
		memset(wrk, 0, pkl->width*pkl->height*pkl->channel);
		
		half = (double)pkl->width/2.0;
		for(j=0; j<pkl->width; j++){
			s = (double)j - half;
			a = atan2(sqrt(half*half-s*s), s);
			fx = 2.0 * half * (1.0 - a / M_PI);
			
			for(i=0; i<pkl->height; i++){
				fy = (double)i;
				op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, color);
			}
		}
		free(pkl->image);
		pkl->image = wrk;
	}
	
	return(0);
}

//=============================================================================
// pkl_dots
//=============================================================================
PKLExport int pkl_dots(PKLImage pkl, int zone, PKLColor dotscolor, PKLColor backcolor)
{
	unsigned char *wrk, *image;
	int i, j, k, t, s, gray, count, stock;
	double propc, radius;
	struct _PKLColor dot;
	long color[PKL_CHANNEL];
	
	if(zone<3 || zone>pkl->width-1 || zone>pkl->height-1)
		return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	
	//背景色
	if(backcolor)
		for(i=0; i<pkl->width*pkl->height*pkl->channel; i+=pkl->channel)
			memcpy(&wrk[i], backcolor->color, pkl->channel);
	else
		memset(wrk, 0xff, pkl->width*pkl->height*pkl->channel);
	
	//ドット色
	if(dotscolor)
		memcpy(&(dot.color), dotscolor->color, pkl->channel);
	
	//propc = zone/32.0;
	propc = zone/28.5;
	
	image = pkl->image;
	pkl->image = wrk;
	
	for(i=0; i<pkl->height; i+=zone){
		for(j=0; j<pkl->width; j+=zone){
			count=0;
			stock=0;
			memset(color, 0, sizeof(color));
			for(t=i; t<i+zone; t++){
				for(s=j; s<j+zone; s++){
					if(t>=pkl->height || s>=pkl->width) continue;
					//p = &image[(t*pkl->width+s)*pkl->channel];
					for(k=0,gray=0; k<pkl->channel; k++){
						color[k] += image[(t*pkl->width+s)*pkl->channel+k];
						gray += image[(t*pkl->width+s)*pkl->channel+k];
					}
					stock += (255-gray/pkl->channel);
					count++;
				}
			}
			if(count==0)
				continue;
			
			if(dotscolor == NULL){
				for(k=0; k<pkl->channel; k++)
					dot.color[k] = PKL_COLOR_CLIP(color[k]/count);
			}
			radius = sqrt(stock/count)*propc;
			draw_ellipse(pkl, j+zone/2, i+zone/2, radius, radius, &dot, 0, 0);
		}
	}
	
	free(image);
	return(0);
}

//=================================================================================
// pkl_sphere
//=================================================================================
PKLExport int pkl_sphere(PKLImage pkl, PKLColor backcolor, PKL_SAMPLE sample)
{
	unsigned char *wrk, color[PKL_CHANNEL];
	int i, j, mx, my, tx, ty;
	double th, fx, fy, nr, radius;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	wrk = malloc(pkl->width * pkl->height * pkl->channel);
	if(!wrk) return(1);
	
	if(backcolor)
		memcpy(color, backcolor->color, pkl->channel);
	else
		memset(color, 0xff, pkl->channel);
	
	mx = pkl->width / 2;
	my = pkl->height / 2;

	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			tx = j - mx;
			ty = i - my;
			
			if(tx == 0)
				th = (ty>=0) ? M_PI/2 : -(M_PI/2);
			else
				th = atan2(ty, tx);
			
			radius = sqrt(tx*tx + ty*ty);
			nr = radius*radius / PKL_MAX(mx, my);
			
			fx = mx + nr*cos(th);
			fy = my + nr*sin(th);
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, color);
		}
	}
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=================================================================================
// pkl_fisheye
//=================================================================================
PKLExport int pkl_fisheye(PKLImage pkl, double weight, PKL_SAMPLE sample)
{
	unsigned char *wrk, color[PKL_CHANNEL];
	int i, j, mx, my, radius;
	double fx, fy, rp;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	radius = pkl->width<pkl->height ? pkl->width : pkl->height;
	
	memset(color, 0, PKL_CHANNEL);
	wrk = malloc(pkl->width * pkl->height * pkl->channel);
	if(!wrk) return(1);
	
	mx = pkl->width / 2;
	my = pkl->height / 2;
	
	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			rp = sqrt(weight*weight + (j-mx)*(j-mx) + (i-my)*(i-my));
			fx = rp*(j-mx)/radius + mx;
			fy = rp*(i-my)/radius + my;
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, color);
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=================================================================================
// pkl_sinusoidal
//=================================================================================
PKLExport int pkl_sinusoidal(PKLImage pkl, double weight, PKL_SAMPLE sample)
{
	unsigned char *wrk;
	int i, j;
	double fx, fy;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	wrk = malloc(pkl->width * pkl->height * pkl->channel);
	if(!wrk) return(1);

	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			fx = weight * sin(j) + j;
			fy = weight * sin(i) + i;
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, &pkl->image[(i*pkl->width+j)*pkl->channel]);
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=================================================================================
// pkl_water
//=================================================================================
PKLExport int pkl_water(PKLImage pkl, int centerx, int centery, int r,
			double wavelength, double amplitude, double phase, PKL_SAMPLE sample)
{
	int i, j, s;
	double fx, fy, cx, cy, radius, amount;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);

	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	//中心座標
	cx = (centerx<0 || centerx>=pkl->width) ? pkl->width * 0.5 : centerx;
	cy = (centery<0 || centery>=pkl->height) ? pkl->height * 0.5 : centery;
	
	//半径
	s = pkl->width > pkl->height ? pkl->height : pkl->width;
	radius = r<0 ? (s*0.5)*(s*0.5) : r*r;

	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			double dx = j - cx;
			double dy = i - cy;
			double distance = dx*dx + dy*dy;
			
			if(distance > radius)
				continue;
			
			distance = sqrt(distance);
			amount = amplitude * sin( (distance / wavelength) * M_PI - phase);
			amount *= (radius-distance)/radius;
			if( distance != 0)
				amount *= wavelength/distance;
			
			fx = j + dx*amount;
			fy = i + dy*amount;
			op(pkl, &pkl->image[(i*pkl->width+j)*pkl->channel], fx, fy, &pkl->image[(i*pkl->width+j)*pkl->channel]);
		}
	}
	return(0);
}

//=============================================================================
// pkl_tv
//=============================================================================
PKLExport int pkl_tv(PKLImage pkl, int seg, int height)
{
	unsigned char *wrk;
	int mpx, mpy;	//モザイクの大きさ
	int mlx, mly;	//画像の分割数
	int i, j, k, sx, sy, ex, ey, px, py, mm, s;
	long color[PKL_CHANNEL];

	if(pkl->color != PKL_RGB) return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);

	mpx = (seg<1) ? 3 : (seg*3>pkl->width) ? pkl->width/3 : seg*3;
	mpy = (height<1) ? 1 : (height>pkl->height) ? pkl->height : height;
	mlx = (pkl->width+mpx-1)/mpx;
	mly = (pkl->height+mpy-1)/mpy;
	
	for(i=0; i<=mly; i++){
		for(j=0; j<=mlx; j++){
			sx = j*mpx;
			sy = i*mpy;
			ex = sx+mpx>pkl->width ? pkl->width : sx+mpx;
			ey = sy+mpy>pkl->height ? pkl->height : sy+mpy;
			
			//1セルの総和算出
			mm=0;
			memset(color, 0, sizeof(color));
			for(py=sy; py<ey; py++){
				for(px=sx; px<ex; px++){
					mm++;
					for(k=0; k<pkl->channel; k++)
						color[k] += pkl->image[(py*pkl->width+px)*pkl->channel+k];
				}
			}
			if(!mm) continue;
			
			//セルの平均色を算出
			for(k=0; k<pkl->channel; k++)
				color[k] = PKL_COLOR_CLIP(color[k]/mm);
			
			//RGBごとに色を配置
			for(py=sy; py<ey; py++){
				for(px=sx,k=0; k<pkl->channel; px+=seg,k++){
					for(s=0; s<seg; s++){
						if(px+s > pkl->width-1)
							break;
						wrk[(py*pkl->width+px+s)*pkl->channel+k] = color[k];
					}
				}
			}
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=================================================================================
// pkl_disk
//=================================================================================
PKLExport int pkl_disk(PKLImage pkl, int mx, int my, double weight, PKL_SAMPLE sample)
{
	unsigned char *wrk;
	int i, j, x, y;
	double fx, fy, r, a;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);

	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	if(mx<0 || mx>pkl->width-1)  mx=pkl->width/2;
	if(my<0 || my>pkl->height-1) my=pkl->height/2;
	
	wrk = malloc(pkl->width * pkl->height * pkl->channel);
	if(!wrk) return(1);

	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			x = j - mx;
			y = i - my;
			r  = sqrt(x*x + y*y);
			a  = atan2(x, y);
			fx = weight * sin(a) * cos(r) + j;
			fy = weight * cos(a) * sin(r) + i;
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, &pkl->image[(i*pkl->width+j)*pkl->channel]);
		}
	}

	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=================================================================================
// pkl_ball
//=================================================================================
PKLExport int pkl_ball(PKLImage pkl, PKLColor backcolor, PKL_SAMPLE sample)
{
	unsigned char *wrk, color[PKL_CHANNEL];
	int i, j;
	double tx, ty, rr, fx, fy, halfw, halfh, vx, vy, hh2;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);

	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);
	
	if(backcolor)
		memcpy(color, backcolor->color, pkl->channel);
	else
		memset(color, 0xff, pkl->channel);
	
	halfw = (double)pkl->width / 2.0;
	halfh = (double)pkl->height / 2.0;
	hh2 = halfh * halfh;
	
	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			
			//tx = halfh * ((double)j-halfw) / halfw;
			tx = halfh * ((double)j-halfw) / halfh;
			ty = (double)i-halfh;
			rr = sqrt(tx*tx + ty*ty);
			
			if(rr <= halfh){
				vx = (tx==0.0) ? 0.0 : (tx>0.0) ? 1.0/sqrt((hh2-ty*ty)/(tx*tx)) : -(1.0/sqrt((hh2-ty*ty)/(tx*tx)));
				vy = (ty==0.0) ? 0.0 : (ty>0.0) ? 1.0/sqrt((hh2-tx*tx)/(ty*ty)) : -(1.0/sqrt((hh2-tx*tx)/(ty*ty)));
				fx = vx * halfw + halfw;
				fy = vy * halfh + halfh;
				
				op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, color);
    		}else{
				memcpy(&wrk[(i*pkl->width+j)*pkl->channel], color, pkl->channel);
    		}
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=================================================================================
// pkl_anaglyph
//=================================================================================
PKLExport int pkl_anaglyph(PKLImage left, PKLImage right, int xpos, int ypos, int is_gray, int is_trim)
{
	unsigned char gray;
	PKLImage b=NULL, r=NULL;
	int tw, th, px, py, cx, cy;
	int i, j, s, t, is_copy=0;
	
	if(!left) return(1);
	if(left->color != PKL_RGB) return(1);
	b = left;
	
	if(right){
		if(right->color != PKL_RGB) return(1);
		r = right;
	}else{
		r = pkl_dup(b);
		is_copy=1;
	}

	//blue-image
	for(i=0; i<b->height; i++){
		for(j=0; j<b->width; j++){
			if(is_gray){
				gray = b->image[(i*b->width+j)*b->channel+0] * 0.298912 +
					   b->image[(i*b->width+j)*b->channel+1] * 0.586611 +
					   b->image[(i*b->width+j)*b->channel+2] * 0.114478;
				b->image[(i*b->width+j)*b->channel+0] = gray;
			}
			b->image[(i*b->width+j)*b->channel+1] = 0xff;
			b->image[(i*b->width+j)*b->channel+2] = 0xff;
		}
	}

	//red-image
	for(i=0; i<r->height; i++){
		for(j=0; j<r->width; j++){
			if(is_gray){
				gray = r->image[(i*r->width+j)*r->channel+0] * 0.298912 +
					   r->image[(i*r->width+j)*r->channel+1] * 0.586611 +
					   r->image[(i*r->width+j)*r->channel+2] * 0.114478;
				r->image[(i*r->width+j)*r->channel+1] = gray;
				r->image[(i*r->width+j)*r->channel+2] = gray;
			}
			r->image[(i*r->width+j)*r->channel+0] = 0xff;
		}
	}
	
	tw = xpos<0 ? r->width+xpos : (xpos+r->width >= b->width) ? b->width-xpos : r->width;
	th = ypos<0 ? r->height+ypos : (ypos+r->height >= b->height) ? b->height-ypos : r->height;
	px = xpos<0 ? 0 : xpos;
	py = ypos<0 ? 0 : ypos;
	cx = xpos<0 ? -xpos : 0;
	cy = ypos<0 ? -ypos : 0;
	
	for(i=py,j=cy; i<th+py; i++,j++){
		for(s=px,t=cx; t<cx+tw; s++,t++){
			//b->image[(i*b->width+s)*b->channel] = 
			b->image[(i*b->width+s)*b->channel+1] = r->image[(j*r->width+t)*r->channel+1];
			b->image[(i*b->width+s)*b->channel+2] = r->image[(j*r->width+t)*r->channel+2];
		}
	}
	
	if(is_trim)
		pkl_trim(b, px, py, tw, th);

	if(is_copy)
		pkl_close(r);
	
	return(0);
}


//=================================================================================
// pkl_thistle
//=================================================================================
PKLExport int pkl_thistle(PKLImage pkl, int zone, int weight)
{
	unsigned char *wrk, *image;
	int i, j, k, t, s, gray, count, stock;
	double propc, radius;
	struct _PKLColor dot;
	long color[PKL_CHANNEL];
	int sx, sy, ex, ey;
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0xff, pkl->width*pkl->height*pkl->channel);

	image = pkl->image;
	pkl->image = wrk;
	
	zone = zone < 2 ? 2 : zone > 10 ? 10 : zone;
	propc = zone/28.5;
	//propc = zone/32.0;
	
	//srand(time(NULL));
	for(i=0; i<pkl->height; i+=zone){
		for(j=0; j<pkl->width; j+=zone){
			count=0;
			stock=0;
			memset(color, 0, sizeof(color));
			for(t=i; t<i+zone; t++){
				for(s=j; s<j+zone; s++){
					if(t>=pkl->height || s>=pkl->width) continue;
					//p = &image[(t*pkl->width+s)*pkl->channel];
					for(k=0,gray=0; k<pkl->channel; k++){
						color[k] += image[(t*pkl->width+s)*pkl->channel+k];
						gray += image[(t*pkl->width+s)*pkl->channel+k];
					}
					stock += (255-gray/pkl->channel);
					count++;
				}
			}
			if(count==0)
				continue;
			
			for(k=0; k<pkl->channel; k++)
				dot.color[k] = PKL_COLOR_CLIP(color[k]/count);
			radius = sqrt(stock/count)*propc;
			sx = j + rand() % (zone*2 + 1) - zone;
			sy = i + rand() % (zone*2 + 1) - zone;
			ex = sx + (rand() % (zone*2 + 1) - zone) * weight;
			ey = sy + (rand() % (zone*2 + 1) - zone) * weight;
			draw_line(pkl, sx, sy, ex, ey, radius*2, &dot, 0, 0);
		}
	}
	free(image);
	return(0);
}

//=================================================================================
// pkl_thorn
//=================================================================================
PKLExport int pkl_thorn(PKLImage pkl, int zone, int weight)
{
	unsigned char *wrk, *image;
	int i, j, tx, ty;
	struct _PKLColor mark;
	
	if(pkl_posterize(pkl, 3)) return(1);
	if(pkl_noisecut(pkl)) return(1);
	if(pkl_edge(pkl, 1)) return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0xff, pkl->width*pkl->height*pkl->channel);
	memset(mark.color, 0, 4);

	image = pkl->image;
	pkl->image = wrk;

	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			if(!memcmp(&image[(i*pkl->width+j)*pkl->channel], mark.color, pkl->channel)){
				tx = j + (rand() % (zone*2 + 1) - zone) * weight;
				ty = i + (rand() % (zone*2 + 1) - zone) * weight;
				draw_line(pkl, j, i, tx, ty, 1, &mark, 0, 0);
			}
		}
	}

	free(image);
	return(0);
}

////=================================================================================
//// pkl_halftone
////=================================================================================
////PKLExport int pkl_halftone(PKLImage pkl)
//PKLExport int pkl_test(PKLImage pkl)
//{
//	unsigned char *wrk;
//	int i, j, k, t, v;
//	double f, f2, R;
//	
//	
//	double radius = 3.0;	//
//	double grid = 2 * radius * 1.414;
//	double angle[3] = { 108*M_PI/180, 162*M_PI/180, 90*M_PI/180 };
//	int mx[5] = { 0, -1, 1,  0, 0 };
//	int my[5] = { 0,  0, 0, -1, 1 };
//	double hgrid = grid / 2;
//	
//	
//	if(pkl->color != PKL_RGB)
//		return(1);
//	
//	wrk = malloc(pkl->width * pkl->height * pkl->channel);
//	memset(wrk, 0xff, pkl->width * pkl->height * pkl->channel);
//	
//	
//	for(i=0; i<pkl->height; i++){
//		//for(k=0; k<pkl->channel; k++){
//		for(k=2; k<3; k++){
//			int shift = 16-8*k;
//			int mask = 0x000000ff << shift;
//			double s = sin(angle[k]);
//			double c = cos(angle[k]);
//			
//			for(j=0; j<pkl->width; j++){
//				f = 1;
//				double tx =  j*c + i*s;
//				double ty = -j*s + i*c;
//				
//				tx = tx-fmod(tx-hgrid, grid) + hgrid;
//				ty = ty-fmod(ty-hgrid, grid) + hgrid;
//				
//				for(t=0; t<5; t++){
//                    double ttx = tx + mx[t]*grid;
//                    double tty = ty + my[t]*grid;
//                    double ntx = ttx*c - tty*s;
//                    double nty = ttx*s + tty*c;
//                    int nx = ntx < 0 ? 0 : ntx >= pkl->width ? pkl->width-1 : ntx;
//                    int ny = nty < 0 ? 0 : nty >= pkl->height ? pkl->height-1 : nty;
//                    //double l = pkl->image[(ny*pkl->width+nx)*pkl->channel+k] / 255;
//                    //l = (1 - l*l) * hgrid*1.414;
//                    double l = (1-pkl->image[(i*pkl->width+j)*pkl->channel+k]/255.0*pkl->image[(i*pkl->width+j)*pkl->channel+k]/255.0) * hgrid*1.414;
//                    R = sqrt( (j-ntx)*(j-ntx) + (i-nty)*(i-nty) );
//                    R = l<R ? 0 : l>=R+1 ? 1 : (l-R)*(l-R)*(3 - 2*(l-R));
//                    f2 = 1-R;
//                    f = f < f2 ? f : f2;
//                }
//                wrk[(i*pkl->width+j)*pkl->channel+k] = (int)(255 * f);
//			}
//			
//		}
//	}
//	
//	free(pkl->image);
//	pkl->image = wrk;
//	return(0);
//}


////=============================================================================
//// pkl_test
////=============================================================================
//PKLExport int pkl_test(PKLImage pkl)
//{
//	unsigned char *wrk;
//	int i, j, s, t, k, clipx, clipy, weight=1;
//	int color[PKL_CHANNEL];
//
//	int matrix[9] = {
//		-1, -1,  0,
//		-1,  1,  1,
//		 0,  1,  1
//	};
//
//
//	for(i=0; i<9; i++)
//		weight += matrix[i];
//	
//	wrk = malloc(pkl->width*pkl->height*pkl->channel);
//	if(!wrk) return(1);
//	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);
//
//	for(i=0; i<pkl->height; i++){
//		for(j=0; j<pkl->width; j++){
//			memset(color, 0, sizeof(color));
//			for(s=0; s<3; s++){
//				for(t=0; t<3; t++){
//					clipx = (j+t)>=pkl->width  ? pkl->width-1  : j+t;
//					clipy = (i+s)>=pkl->height ? pkl->height-1 : i+s;
//					for(k=0; k<pkl->channel; k++)
//						//color[k] += pkl->image[(clipy*pkl->width+clipx) * pkl->channel + k] * matrix[t+s*3];
//						color[k] += pkl->image[(clipy*pkl->width+clipx) * pkl->channel + k] * matrix[t+s*3];
//				}
//			}
//			
//			for(k=0; k<pkl->channel; k++)
//				//wrk[(i*pkl->width+j)*pkl->channel+k] = PKL_COLOR_CLIP(color[k]/weight);
//				wrk[(i*pkl->width+j)*pkl->channel+k] = PKL_COLOR_CLIP(color[k]);
//		}
//	}
//
//	free(pkl->image);
//	pkl->image = wrk;
//	return(0);
//}
//

////=================================================================================
//// pkl_test
////=================================================================================
//PKLExport int pkl_test(PKLImage pkl)
//{
////	unsigned char *wrk;
////	int i, j;
////	double fx, fy, cx, cy, radius, amount;
////	
////double wavelength = 40.0;	//波紋長
////double amplitude = 1.5;	//振幅
////double phase = 10.0;
////	
////	wrk = malloc(pkl->width * pkl->height * pkl->channel);
////	if(!wrk) return(1);
////
////	//中心座標
////	cx = pkl->width * 0.5;
////	cy = pkl->height * 0.5;
////
////	//半径(暫定)
////	radius = cy*cy;
////
////	for(i=0; i<pkl->height; i++){
////		for(j=0; j<pkl->width; j++){
////			double dx = j - cx;
////			double dy = i - cy;
////			double distance = dx*dx + dy*dy;
////			
////			if(distance > radius)
////				continue;
////			
////			distance = sqrt(distance);
////			amount = amplitude * sin( (distance / wavelength) * M_PI - phase);
////			amount *= (radius-distance)/radius;
////			if( distance != 0)
////				amount *= wavelength/distance;
////			
////			fx = j + dx*amount;
////			fy = i + dy*amount;
////			
////			//pkl_bilinear(pkl, &wrk[(i*pkl->width+j)*pkl->channel], fx, fy, &pkl->image[(i*pkl->width+j)*pkl->channel]);
////			pkl_bilinear(pkl, &pkl->image[(i*pkl->width+j)*pkl->channel], fx, fy, &pkl->image[(i*pkl->width+j)*pkl->channel]);
////		}
////	}
//	return(0);
//}





//=================================================================================
// pkl_test
//=================================================================================
PKLExport int pkl_test(PKLImage pkl)
{
	return(0);
}

