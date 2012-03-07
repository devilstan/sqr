#include "pikl_bitmap.h"

//=============================================================================
// color_b2p
//	biBitCount = 1,4,8,16,24,32
//=============================================================================
static int color_b2p(int biBitCount)
{
	switch(biBitCount){
		case 24:
			return PKL_RGB;
		default:
			return PKL_UNKNOWN;
	}
	return PKL_UNKNOWN;
}

//=============================================================================
// bmp_channel
//	biBitCount = 1,4,8,16,24,32
//=============================================================================
static int bmp_channel(int biBitCount)
{
	switch(biBitCount){
		case 24:
			return 3;
		default:
			return 0;
	}
	return 0;
}

//=============================================================================
// convert_nv
//=============================================================================
static long convert_numeric(unsigned char *src, int size)
{
	int i;
	long dst=0;
	
	for(i=0; i<size; i++){
		dst += (src[i]<<(8*i));
	}
	return(dst);
}

//=============================================================================
// bmp_width
//=============================================================================
static int bmp_width(int width, int channel)
{
	int length;
	if( width*channel % 4 ){
		length = width*channel + (4 - width*channel % 4);
	}else{
		length = width*channel;
	}
	return(length);
}

//=============================================================================
// bmp_header
//=============================================================================
static void bmp_header(unsigned char *data, int offset, int value, int size)
{
	int i;
	for(i=0; i<size; i++){
		data[offset+i] = (unsigned char)(value >> (8*i));
	}
}

//=================================================================================
// _mread
//=================================================================================
static size_t _mread(void *buf, size_t size, size_t n, void *dp)
{
	struct pkl_bmp_data *data = (struct pkl_bmp_data *)dp;
	size_t s = size * n;
	
	if(data->offset+size > data->length)
		s = data->length - data->offset;
	
	if(s > 0){
		memcpy(buf, &data->data[data->offset], s);
		data->offset += s;
	}
	return s;
}

//=================================================================================
// _mseek
//=================================================================================
static int _mseek(void *dp, long offset, int origin)
{
	struct pkl_bmp_data *data = (struct pkl_bmp_data *)dp;
	
	switch(origin){
		case SEEK_CUR:
			data->offset += offset;
			break;
		case SEEK_SET:
			data->offset = offset;
			break;
		case SEEK_END:
			data->offset = data->length + offset;
			break;
		default:
			return -1;
	}
	
	return (data->offset > data->length || data->offset < 0) ? -1 : 0;
}

//=================================================================================
// _fread
//=================================================================================
static size_t _fread(void *buf, size_t size, size_t n, void *dp)
{
	return fread(buf, size, n, (FILE *)dp);
}

//=================================================================================
// _fseek
//=================================================================================
static int _fseek(void *dp, long offset, int origin)
{
	return fseek((FILE *)dp, offset, origin);
}

//=============================================================================
// load_bmp
//=============================================================================
static int load_bmp(PKLImage pkl, FILE *image, struct pkl_bmp_data *data)
{
	unsigned char header[BF_SIZE + BI_SIZE];
	unsigned char *p, stuck;
	int i, j, offset, scansize;
	size_t (*reader)(void *buf, size_t size, size_t n, void *dp);
	int (*seeker)(void *dp, long offset, int origin);
	void *dp;
	
	if(image){
		reader = _fread;
		seeker = _fseek;
		dp = (void *)image;
	}else{
		reader = _mread;
		seeker = _mseek;
		dp = (void *)data;
	}
	
	if(reader(header, 1, sizeof(header), dp) != sizeof(header))
		return(1);

	pkl->width   = convert_numeric(&header[18], 4);
	pkl->height  = convert_numeric(&header[22], 4);
	pkl->color   = color_b2p(convert_numeric(&header[28], 2));
	pkl->channel = bmp_channel(convert_numeric(&header[28], 2));
	if(pkl->color==PKL_UNKNOWN) return(1);
	
	pkl->image = malloc(pkl->width * pkl->height * pkl->channel);
	if(!pkl->image) return(1);
	
	offset = convert_numeric(&header[10], 4);
	scansize = bmp_width(pkl->width, pkl->channel);

	p = pkl->image + ((pkl->height-1) * pkl->width * pkl->channel);
	seeker(dp, offset, SEEK_SET);
	
	for(i=0; i<pkl->height; i++){
		reader(p, 1, pkl->width*pkl->channel, dp);
		for(j=0; j<pkl->width; j++){
			stuck = p[j*pkl->channel];
			p[j*pkl->channel] = p[j*pkl->channel+2];
			p[j*pkl->channel+2] = stuck;
		}
		p -= pkl->width*pkl->channel;
		offset += scansize;
		seeker(dp, offset, SEEK_SET);
	}
	
	return(0);
}

//=================================================================================
// load_bmp_fd
//=================================================================================
int load_bmp_fd(PKLImage pkl, FILE *image)
{
	if(!image)
		return 1;
	
	return load_bmp(pkl, image, NULL);
}

//=================================================================================
// load_bmp_data
//=================================================================================
int load_bmp_data(PKLImage pkl, unsigned char *image, unsigned long length)
{
	struct pkl_bmp_data data;
	
	if(!image || length<=0)
		return 1;
	
	data.data = image;
	data.length = length;
	data.offset = 0;
	return load_bmp(pkl, NULL, &data);
}

//=================================================================================
// _fwrite
//=================================================================================
static size_t _fwrite(const void *buf, size_t size, size_t n, void *dp)
{
	return fwrite(buf, size, n, (FILE *)dp);
}

//=================================================================================
// _mwrite
//=================================================================================
static size_t _mwrite(const void *buf, size_t size, size_t n, void *dp)
{
	struct pkl_bmp_data *data = (struct pkl_bmp_data *)dp;
	size_t s;
	
	if((s = size * n) <= 0)
		return 0;
	
	if(data->offset+s > data->length){
		data->length += (s / PKL_BMP_ALLOCATE + 1) * PKL_BMP_ALLOCATE;
		data->data = realloc(data->data, data->length);
	}
	
	memcpy(&data->data[data->offset], buf, s);
	data->offset += s;
	return s;
}

//=============================================================================
// save_bitmap
//=============================================================================
static int save_bmp(PKLImage pkl, FILE *image, struct pkl_bmp_data *data)
{
	unsigned char h[BF_SIZE+BI_SIZE], *wrk, stuck;
	int linesize, i, j;
	size_t (*writer)(const void *buf, size_t size, size_t n, void *dp);
	void *dp;
	
	if(pkl->color!=PKL_RGB)
		return(1);
	
	if(image){
		writer = _fwrite;
		dp = (void *)image;
	}else{
		writer = _mwrite;
		dp = (void *)data;
	}
	
	linesize = bmp_width(pkl->width, pkl->channel);
	
	h[0] = 'B';
	h[1] = 'M';
	bmp_header(h, 2, BF_SIZE + BI_SIZE + linesize*pkl->height, 4);
	bmp_header(h, 6, 0, 4);
	bmp_header(h, 10, BF_SIZE + BI_SIZE, 4);
	bmp_header(h, 14, BI_SIZE, 4);
	bmp_header(h, 18, pkl->width, 4);
	bmp_header(h, 22, pkl->height, 4);
	bmp_header(h, 26, 1, 2);
	bmp_header(h, 28, 24, 2);
	bmp_header(h, 30, 0, 24);
	writer(h, 1, BF_SIZE+BI_SIZE, dp);
	
	wrk = malloc(linesize);
	for(i=pkl->height-1; i>=0; i--){
		memcpy(wrk, &pkl->image[i*pkl->width*pkl->channel], pkl->width*pkl->channel);
		for(j=0; j<pkl->width; j++){
			stuck = wrk[j*pkl->channel];
			wrk[j*pkl->channel] = wrk[j*pkl->channel+2];
			wrk[j*pkl->channel+2] = stuck;
		}
		writer(wrk, 1, linesize, dp);
	}
	free(wrk);
	return(0);
}

//=================================================================================
// save_bmp_fd
//=================================================================================
int save_bmp_fd(PKLImage pkl, FILE *image)
{
	if(!image)
		return 1;
	
	return save_bmp(pkl, image, NULL);
}

//=================================================================================
// save_bmp_data
//=================================================================================
unsigned char *save_bmp_data(PKLImage pkl, unsigned long *length)
{
	struct pkl_bmp_data data;
	
	if(!length)
		return NULL;
	
	memset(&data, 0, sizeof(struct pkl_bmp_data));
	if(save_bmp(pkl, NULL, &data))
		return NULL;
	
	if(data.offset != data.length)
		realloc(data.data, data.offset);
	
	*length = data.offset;
	return data.data;
}
