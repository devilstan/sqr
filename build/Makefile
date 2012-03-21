CPP=g++
CC=gcc
DEBUG = -DDEBUG

CFLAGS=-O3 -Wall -c
LFLAGS=

PIKL_CFLAGS = -I. -Ipikl
PIKL_LFLAGS = -ljpeg -lpng -lgif

CAIRO_CFLAGS = -I/usr/include/cairo
CAIRO_LFLAGS = -lcairo 

CFLAGS += $(PIKL_CFLAGS) $(CAIRO_CFLAGS)
LFLAGS += $(PIKL_LFLAGS) $(CAIRO_LFLAGS)

#=============================================================================
# object
#=============================================================================
QR_ENCODE = QR_Encode.o
QR_DRAW = \
	qdr_draw.o \
	qdr_largest.o \
	qdr_filter.o \
	qdr_stack.o \
	qdr_version.o \
	qdr_group.o \
	qdr_mark.o \
	qdr_eye.o \
	qdr_text.o \
	qdr_paste.o \
	qdr_import.o \
	qdr_test.o

PIKL = \
	pikl_io.o   \
	pikl_pixel.o \
	pikl_jpeg.o \
	pikl_png.o \
	pikl_bitmap.o \
	pikl_rotate.o \
	pikl_resize.o \
	pikl_enhance.o \
	pikl_affine.o \
	pikl_effect.o \
	pikl_pattern.o \
	pikl_blur.o \
	pikl_decrease.o \
	pikl_divide.o \
	pikl_composite.o \
	pikl_camera.o \
	pikl_special.o \
	pikl_voronoi.o \
	pikl_scrap.o \
	pikl_algorithm.o \
	pikl_noise.o \
	pikl_draw.o

DBGOBJ = \
	dbg_malloc.o \
	dbg_malloc_print.o \
	dbg_malloc_sub.o

OBJECTS = $(QR_ENCODE) $(QR_DRAW) $(PIKL)

ifdef DEBUG
	OBJECTS += $(DBGOBJ)
endif

#=============================================================================
# all
#=============================================================================
SAMPLES_CFLAGS=-Iencode -Idraw

all: $(OBJECTS)
	$(CPP) $(SAMPLES_CFLAGS) $(CFLAGS) sample.cpp $(DEBUG)
	$(CPP) $(OBJECTS) $(LFLAGS) sample.o

#=============================================================================
# encoder
#=============================================================================
QR_ENCODE_CFLAGS=-O3 -c
QR_Encode.o:
	$(CPP) $(QR_ENCODE_CFLAGS) encode/QR_Encode.cpp

#=============================================================================
# draw
#=============================================================================
qdr_draw.o:
	$(CC) $(CFLAGS) draw/qdr_draw.c $(DEBUG)
qdr_largest.o:
	$(CC) $(CFLAGS) draw/qdr_largest.c $(DEBUG)
qdr_filter.o:
	$(CC) $(CFLAGS) draw/qdr_filter.c $(DEBUG)
qdr_stack.o:
	$(CC) $(CFLAGS) draw/qdr_stack.c $(DEBUG)
qdr_version.o:
	$(CC) $(CFLAGS) draw/qdr_version.c $(DEBUG)
qdr_group.o:
	$(CC) $(CFLAGS) draw/qdr_group.c $(DEBUG)
qdr_mark.o:
	$(CC) $(CFLAGS) draw/qdr_mark.c $(DEBUG)
qdr_eye.o:
	$(CC) $(CFLAGS) draw/qdr_eye.c $(DEBUG)
qdr_text.o:
	$(CC) $(CFLAGS) draw/qdr_text.c $(DEBUG)
qdr_paste.o:
	$(CC) $(CFLAGS) draw/qdr_paste.c $(DEBUG)
qdr_import.o:
	$(CC) $(CFLAGS) draw/qdr_import.c $(DEBUG)
qdr_test.o:
	$(CC) $(CFLAGS) draw/qdr_test.c $(DEBUG)

#=============================================================================
# pikl
#=============================================================================
pikl_io.o:
	$(CC) $(CFLAGS) pikl/pikl_io.c $(DEBUG)
pikl_pixel.o:
	$(CC) $(CFLAGS) pikl/pikl_pixel.c $(DEBUG)
pikl_jpeg.o:
	$(CC) $(CFLAGS) pikl/pikl_jpeg.c $(DEBUG)
pikl_bitmap.o:
	$(CC) $(CFLAGS) pikl/pikl_bitmap.c $(DEBUG)
pikl_png.o:
	$(CC) $(CFLAGS) pikl/pikl_png.c $(DEBUG)
pikl_rotate.o:
	$(CC) $(CFLAGS) pikl/pikl_rotate.c $(DEBUG)
pikl_resize.o:
	$(CC) $(CFLAGS) pikl/pikl_resize.c $(DEBUG)
pikl_affine.o:
	$(CC) $(CFLAGS) pikl/pikl_affine.c $(DEBUG)
pikl_enhance.o:
	$(CC) $(CFLAGS) pikl/pikl_enhance.c $(DEBUG)
pikl_effect.o:
	$(CC) $(CFLAGS) pikl/pikl_effect.c $(DEBUG)
pikl_pattern.o:
	$(CC) $(CFLAGS) pikl/pikl_pattern.c $(DEBUG)
pikl_blur.o:
	$(CC) $(CFLAGS) pikl/pikl_blur.c $(DEBUG)
pikl_decrease.o:
	$(CC) $(CFLAGS) pikl/pikl_decrease.c $(DEBUG)
pikl_divide.o:
	$(CC) $(CFLAGS) pikl/pikl_divide.c $(DEBUG)
pikl_composite.o:
	$(CC) $(CFLAGS) pikl/pikl_composite.c $(DEBUG)
pikl_scrap.o:
	$(CC) $(CFLAGS) pikl/pikl_scrap.c $(DEBUG)
pikl_camera.o:
	$(CC) $(CFLAGS) pikl/pikl_camera.c $(DEBUG)
pikl_special.o:
	$(CC) $(CFLAGS) pikl/pikl_special.c $(DEBUG)
pikl_voronoi.o:
	$(CC) $(CFLAGS) pikl/pikl_voronoi.c $(DEBUG)
pikl_algorithm.o:
	$(CC) $(CFLAGS) pikl/pikl_algorithm.c $(DEBUG)
pikl_draw.o:
	$(CC) $(CFLAGS) pikl/pikl_draw.c $(DEBUG)
pikl_noise.o:
	$(CC) $(CFLAGS) pikl/pikl_noise.c $(DEBUG)

#=============================================================================
# debug
#=============================================================================
dbg_malloc.o:
	$(CC) $(CFLAGS) dbg/dbg_malloc.c $(DEBUG)
dbg_malloc_print.o:
	$(CC) $(CFLAGS) dbg/dbg_malloc_print.c $(DEBUG)
dbg_malloc_sub.o:
	$(CC) $(CFLAGS) dbg/dbg_malloc_sub.c $(DEBUG)

#=============================================================================
# clean
#=============================================================================
clean:
	rm -rf *.o *.exe

