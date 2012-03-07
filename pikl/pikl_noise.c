#include "pikl_noise.h"

//=================================================================================
// perlin_init
//=================================================================================
static struct PERLIN_NOISE_DATA *perlin_init(struct PERLIN_NOISE_DATA *data)
{
	int i, j, k;
	
	memset(data, 0, sizeof(struct PERLIN_NOISE_DATA));
	
	srand(1);
	for(i=0; i<NOISE_B; i++)
		data->p[i] = i;
	
	for(i=NOISE_B-1; i>=0; i--){
		k = data->p[i];
		j = (rand() & RAND_MAX) % NOISE_B;
		data->p[i] = data->p[j];
		data->p[j] = k;
	}
	
	for(i=0; i<NOISE_B+2; i++)
		data->p[NOISE_B+i] = data->p[i];

	return(data);
}

//=================================================================================
// perlin_1d
//=================================================================================
static struct PERLIN_NOISE_DATA *perlin_1d(struct PERLIN_NOISE_DATA *data)
{
	int i;
	
	perlin_init(data);
	
	for(i=0; i<NOISE_B; i++)
		data->g1[i] = (double)(((rand() & RAND_MAX) % (NOISE_B + NOISE_B)) - NOISE_B) / NOISE_B;

	for(i=0; i<NOISE_B+2; i++)
		data->g1[NOISE_B+i] = data->g1[i];
	
	return(data);
}

//=================================================================================
// perlin_2d
//=================================================================================
static struct PERLIN_NOISE_DATA *perlin_2d(struct PERLIN_NOISE_DATA *data)
{
	int i, j;
	double s;
	
	perlin_init(data);
	
	for(i=0; i<NOISE_B; i++){
		for(j=0; j<2; j++)
			data->g2[i][j] = (double)(((rand() & RAND_MAX) % (NOISE_B + NOISE_B)) - NOISE_B) / NOISE_B;
		
		s = sqrt(data->g2[i][0]*data->g2[i][0] + data->g2[i][1]*data->g2[i][1]);
		data->g2[i][0] = data->g2[i][0] / s;
		data->g2[i][1] = data->g2[i][1] / s;
	}

	for(i=0; i<NOISE_B+2; i++){
		for(j=0; j<2; j++)
			data->g2[NOISE_B+i][j] = data->g2[i][j];
	}
	
	return(data);
}

//=================================================================================
// perlin_3d
//=================================================================================
static struct PERLIN_NOISE_DATA *perlin_3d(struct PERLIN_NOISE_DATA *data)
{
	int i, j;
	double s;

	perlin_init(data);
	
	for(i=0; i<NOISE_B; i++){
		for(j=0; j<3; j++)
			data->g3[i][j] = (double)(((rand() & RAND_MAX) % (NOISE_B + NOISE_B)) - NOISE_B) / NOISE_B;

		s = sqrt(data->g3[i][0] * data->g3[i][0] + data->g3[i][1] * data->g3[i][1] + data->g3[i][2] * data->g3[i][2]);
		data->g3[i][0] = data->g3[i][0] / s;
		data->g3[i][1] = data->g3[i][1] / s;
		data->g3[i][2] = data->g3[i][2] / s;
	}
	
	for(i=0; i<NOISE_B+2; i++){
		for(j=0; j<3; j++)
			data->g3[NOISE_B+i][j] = data->g3[i][j];
	}
	
	return(data);
}

//=================================================================================
// pkl_marble
//=================================================================================
PKLExport int pkl_marble(PKLImage pkl, double xscale, double yscale, double turbulence, PKL_SAMPLE sample)
{
	unsigned char *wrk;
	double sintable[256], costable[256];
	double rx0, rx1, ry0, ry1, sx, sy, a, b, t, u, v, pnn;
	int i, j, bx0, bx1, by0, by1, pn;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	struct PERLIN_NOISE_DATA data;

	if(sample==PKL_SAMPLE_NN)
		sample = PKL_SAMPLE_BL;
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);

	
	perlin_2d(&data);
	for(i=0; i<256; i++){
		double angle = M_PI*2 * i / 256 * turbulence;
		sintable[i] = -yscale*sin(angle);
		costable[i] =  yscale*cos(angle);
	}

	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			double nx = j/xscale;
			double ny = i/xscale;
			
			t = nx + 4096;
			bx0 = ((int)t) & 0xff;
			bx1 = (bx0+1) & 0xff;
			rx0 = t - (int)t;
			rx1 = rx0 - 1.0;
			
			t = ny + 4096;
			by0 = ((int)t) & 0xff;
			by1 = (by0+1) & 0xff;
			ry0 = t - (int)t;
			ry1 = ry0 - 1.0;
			
			sx = rx0 * rx0 * (3.0 - 2.0 * rx0);
			sy = ry0 * ry0 * (3.0 - 2.0 * ry0);
			
			u = rx0 * data.g2[data.p[data.p[bx0] + by0]][0] + ry0 * data.g2[data.p[data.p[bx0] + by0]][1];
			v = rx1 * data.g2[data.p[data.p[bx1] + by0]][0] + ry0 * data.g2[data.p[data.p[bx1] + by0]][1];
			a = u + sx * (v - u);
			
			u = rx0 * data.g2[data.p[data.p[bx0] + by1]][0] + ry1 * data.g2[data.p[data.p[bx0] + by1]][1];
			v = rx1 * data.g2[data.p[data.p[bx1] + by1]][0] + ry1 * data.g2[data.p[data.p[bx1] + by1]][1];
			b = u + sx * (v - u);
			
			pnn = 127 * (1 + 1.5 * (a + sy*(b-a)));
			pn = PKL_COLOR_CLIP(pnn);
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], j+sintable[pn], i+costable[pn], &pkl->image[(i*pkl->width+j)*pkl->channel]);
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=================================================================================
// pkl_perlin2d
//=================================================================================
PKLExport int pkl_perlin2d(PKLImage pkl, double xlen, double ylen, double xweight, double yweight, PKLColor backcolor, PKL_SAMPLE sample)
{
	unsigned char *wrk;
	double rx0, rx1, ry0, ry1, sx, sy, a, b, t, u, v, pnn;
	int i, j, bx0, bx1, by0, by1;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	struct PERLIN_NOISE_DATA data;
	unsigned char color[PKL_CHANNEL];

	if(sample==PKL_SAMPLE_NN)
		sample = PKL_SAMPLE_BL;
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);

	xlen = (xlen == 0.0) ? 1.0 : xlen;
	ylen = (ylen == 0.0) ? 1.0 : ylen;
	
	if(backcolor)
		memcpy(color, backcolor->color, pkl->channel);
	else
		memset(color, 0xff, pkl->channel);
	
	perlin_2d(&data);
	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			double nx = i/xlen;
			double ny = j/ylen;
			
			t = nx + 4096;
			bx0 = ((int)t) & 0xff;
			bx1 = (bx0+1) & 0xff;
			rx0 = t - (int)t;
			rx1 = rx0 - 1.0;
			
			t = ny + 4096;
			by0 = ((int)t) & 0xff;
			by1 = (by0+1) & 0xff;
			ry0 = t - (int)t;
			ry1 = ry0 - 1.0;
			
			sx = rx0 * rx0 * (3.0 - 2.0 * rx0);
			sy = ry0 * ry0 * (3.0 - 2.0 * ry0);
			
			u = rx0 * data.g2[data.p[data.p[bx0] + by0]][0] + ry0 * data.g2[data.p[data.p[bx0] + by0]][1];
			v = rx1 * data.g2[data.p[data.p[bx1] + by0]][0] + ry0 * data.g2[data.p[data.p[bx1] + by0]][1];
			a = u + sx * (v - u);
			
			u = rx0 * data.g2[data.p[data.p[bx0] + by1]][0] + ry1 * data.g2[data.p[data.p[bx0] + by1]][1];
			v = rx1 * data.g2[data.p[data.p[bx1] + by1]][0] + ry1 * data.g2[data.p[data.p[bx1] + by1]][1];
			b = u + sx * (v - u);
			
			pnn = 1.5 * (a + sy*(b-a));
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], j+pnn*xweight, i+pnn*yweight, color);
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}


//=================================================================================
// pkl_perlin1d
//=================================================================================
PKLExport int pkl_perlin1d(PKLImage pkl, double xlen, double ylen, double xweight, double yweight, PKLColor backcolor, PKL_SAMPLE sample)
{
	unsigned char *wrk, color[PKL_CHANNEL];
	double s, r0, r1, t, u, v, pnx, pny;
	int i, j, b0, b1;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	struct PERLIN_NOISE_DATA data;
	
	if(sample==PKL_SAMPLE_NN)
		sample = PKL_SAMPLE_BL;
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);
	
	xlen = (xlen == 0.0) ? 1.0 : xlen;
	ylen = (ylen == 0.0) ? 1.0 : ylen;
	
	if(backcolor)
		memcpy(color, backcolor->color, pkl->channel);
	else
		memset(color, 0xff, pkl->channel);
	
	perlin_1d(&data);
	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			double nx = i/xlen;
			double ny = j/ylen;
			
			t = nx + 4096;
			b0 = ((int)t) & 0xff;
			b1 = (b0+1) & 0xff;
			r0 = t - (int)t;
			r1 = r0 - 1.0;
			s = r0 * r0 * (3.0 - 2.0 * r0);
			u = r0 * data.g1[data.p[b0]];
			v = r1 * data.g1[data.p[b1]];
			pnx = 2.3 * (u + s * (v - u));
			
			t = ny + 4096;
			b0 = ((int)t) & 0xff;
			b1 = (b0+1) & 0xff;
			r0 = t - (int)t;
			r1 = r0 - 1.0;
			s = r0 * r0 * (3.0 - 2.0 * r0);
			u = r0 * data.g1[data.p[b0]];
			v = r1 * data.g1[data.p[b1]];
			pny = 2.3 * (u + s * (v - u));
			
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], j+pnx*xweight, i+pny*yweight, color);
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}

//=================================================================================
// pkl_perlin3d
//=================================================================================
PKLExport int pkl_perlin3d(PKLImage pkl, double xlen, double ylen, double xweight, double yweight, PKLColor backcolor, PKL_SAMPLE sample)
{
	unsigned char *wrk;
	double rx0, rx1, ry0, ry1, rz0, rz1, sy, sz, a, b, c, d, t, u, v, pnn;
	int i, j, bx0, bx1, by0, by1, bz0, bz1;
	void (*op)(PKLImage pkl, unsigned char *dst, double fx, double fy, unsigned char *color);
	struct PERLIN_NOISE_DATA data;
	unsigned char color[PKL_CHANNEL];

	if(sample==PKL_SAMPLE_NN)
		sample = PKL_SAMPLE_BL;
	if((op=select_sampler(sample)) == NULL)
		return(1);
	
	wrk = malloc(pkl->width*pkl->height*pkl->channel);
	if(!wrk) return(1);
	memset(wrk, 0, pkl->width*pkl->height*pkl->channel);

	xlen = (xlen == 0.0) ? 1.0 : xlen;
	ylen = (ylen == 0.0) ? 1.0 : ylen;
	
	if(backcolor)
		memcpy(color, backcolor->color, pkl->channel);
	else
		memset(color, 0xff, pkl->channel);
	
	perlin_3d(&data);
	for(i=0; i<pkl->height; i++){
		for(j=0; j<pkl->width; j++){
			double nx = i/xlen;
			double ny = j/ylen;
			double nz = time(NULL);
			
			t = nx + 4096;
			bx0 = ((int)t) & 0xff;
			bx1 = (bx0+1) & 0xff;
			rx0 = t - (int)t;
			rx1 = rx0 - 1.0;
			
			t = ny + 4096;
			by0 = ((int)t) & 0xff;
			by1 = (by0+1) & 0xff;
			ry0 = t - (int)t;
			ry1 = ry0 - 1.0;
			
			t = nz + 4096;
			bz0 = ((int)t) & 0xff;
			bz1 = (bz0+1) & 0xff;
			rz0 = t - (int)t;
			rz1 = rz0 - 1.0f;
			
			t  = rx0 * rx0 * (3.0f - 2.0f * rx0);
			sy = ry0 * ry0 * (3.0f - 2.0f * ry0);
			sz = rz0 * rz0 * (3.0f - 2.0f * rz0);

			u = rx0 * data.g3[data.p[data.p[bx0] + by0] + bz0][0] + ry0 * data.g3[data.p[data.p[bx0] + by0] + bz0][1] + rz0 * data.g3[data.p[data.p[bx0] + by0] + bz0][2];
			v = rx1 * data.g3[data.p[data.p[bx1] + by0] + bz0][0] + ry0 * data.g3[data.p[data.p[bx1] + by0] + bz0][1] + rz0 * data.g3[data.p[data.p[bx1] + by0] + bz0][2];
			a = u + t * (v - u);

			u = rx0 * data.g3[data.p[data.p[bx0] + by1] + bz0][0] + ry1 * data.g3[data.p[data.p[bx0] + by1] + bz0][1] + rz0 * data.g3[data.p[data.p[bx0] + by1] + bz0][2];
			v = rx1 * data.g3[data.p[data.p[bx1] + by1] + bz0][0] + ry1 * data.g3[data.p[data.p[bx1] + by1] + bz0][1] + rz0 * data.g3[data.p[data.p[bx1] + by1] + bz0][2];
			b = u + t * (v - u);

			c = a + sy * (b - a);

			u = rx0 * data.g3[data.p[data.p[bx0] + by0] + bz1][0] + ry0 * data.g3[data.p[data.p[bx0] + by0] + bz1][1] + rz1 * data.g3[data.p[data.p[bx0] + by0] + bz1][2];
			v = rx1 * data.g3[data.p[data.p[bx1] + by0] + bz1][0] + ry0 * data.g3[data.p[data.p[bx1] + by0] + bz1][1] + rz1 * data.g3[data.p[data.p[bx1] + by0] + bz1][2];
			a = u + t * (v - u);

			u = rx0 * data.g3[data.p[data.p[bx0] + by1] + bz1][0] + ry1 * data.g3[data.p[data.p[bx0] + by1] + bz1][1] + rz1 * data.g3[data.p[data.p[bx0] + by1] + bz1][2];
			v = rx1 * data.g3[data.p[data.p[bx1] + by1] + bz1][0] + ry1 * data.g3[data.p[data.p[bx1] + by1] + bz1][1] + rz1 * data.g3[data.p[data.p[bx1] + by1] + bz1][2];
			b = u + t * (v - u);

			d = a + sy * (b - a);

			pnn = 1.5f*(c + sz * (d - c));
			op(pkl, &wrk[(i*pkl->width+j)*pkl->channel], j+pnn*xweight, i+pnn*yweight, color);
		}
	}
	
	free(pkl->image);
	pkl->image = wrk;
	return(0);
}
