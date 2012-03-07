CC    = gcc
CFLAGS = -Wall -O3
LIB    = -ljpeg -lpng
#DEBUG = -DDEBUG
#COVER = -fprofile-arcs -ftest-coverage
#PROF  = -pg
EXT    = dll

SHOPT = -shared
#SHOPT = -dynamiclib

#=============================================================================
# target object
#=============================================================================
OBJ = pikl_io.o   \
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
DBGOBJ = dbg_malloc.o dbg_malloc_print.o dbg_malloc_sub.o

ifdef DEBUG
	TARGET = $(OBJ) $(DBGOBJ)
else
	TARGET = $(OBJ)
endif

#=============================================================================
# target all
#=============================================================================
all: $(TARGET)
	$(CC) $(CFLAGS) $(SHOPT) -o libpikl.$(EXT) $(TARGET) $(LIB)

#install:
#	cp libpikl.$(EXT)  ../../lib/pikl/
#	cp ruby/libpikl.rb ../../lib/pikl/

test:
	$(CC) $(CFLAGS) test.c -I. -L. -lpikl $(DEBUG)

#=============================================================================
# pikl-core
#=============================================================================
pikl_io.o:
	$(CC) $(CFLAGS) -c pikl_io.c $(DEBUG) $(COVER) $(PROF)
pikl_pixel.o:
	$(CC) $(CFLAGS) -c pikl_pixel.c $(DEBUG) $(COVER) $(PROF)
pikl_jpeg.o:
	$(CC) $(CFLAGS) -c pikl_jpeg.c $(DEBUG) $(COVER) $(PROF)
pikl_bitmap.o:
	$(CC) $(CFLAGS) -c pikl_bitmap.c $(DEBUG) $(COVER) $(PROF)
pikl_png.o:
	$(CC) $(CFLAGS) -c pikl_png.c $(DEBUG) $(COVER) $(PROF)
pikl_rotate.o:
	$(CC) $(CFLAGS) -c pikl_rotate.c $(DEBUG) $(COVER) $(PROF)
pikl_resize.o:
	$(CC) $(CFLAGS) -c pikl_resize.c $(DEBUG) $(COVER) $(PROF)
pikl_affine.o:
	$(CC) $(CFLAGS) -c pikl_affine.c $(DEBUG) $(COVER) $(PROF)
pikl_enhance.o:
	$(CC) $(CFLAGS) -c pikl_enhance.c $(DEBUG) $(COVER) $(PROF)
pikl_effect.o:
	$(CC) $(CFLAGS) -c pikl_effect.c $(DEBUG) $(COVER) $(PROF)
pikl_pattern.o:
	$(CC) $(CFLAGS) -c pikl_pattern.c $(DEBUG) $(COVER) $(PROF)
pikl_blur.o:
	$(CC) $(CFLAGS) -c pikl_blur.c $(DEBUG) $(COVER) $(PROF)
pikl_decrease.o:
	$(CC) $(CFLAGS) -c pikl_decrease.c $(DEBUG) $(COVER) $(PROF)
pikl_divide.o:
	$(CC) $(CFLAGS) -c pikl_divide.c $(DEBUG) $(COVER) $(PROF)
pikl_composite.o:
	$(CC) $(CFLAGS) -c pikl_composite.c $(DEBUG) $(COVER) $(PROF)
pikl_scrap.o:
	$(CC) $(CFLAGS) -c pikl_scrap.c $(DEBUG) $(COVER) $(PROF)
pikl_camera.o:
	$(CC) $(CFLAGS) -c pikl_camera.c $(DEBUG) $(COVER) $(PROF)
pikl_special.o:
	$(CC) $(CFLAGS) -c pikl_special.c $(DEBUG) $(COVER) $(PROF)
pikl_voronoi.o:
	$(CC) $(CFLAGS) -c pikl_voronoi.c $(DEBUG) $(COVER) $(PROF)
pikl_algorithm.o:
	$(CC) $(CFLAGS) -c pikl_algorithm.c $(DEBUG) $(COVER) $(PROF)
pikl_draw.o:
	$(CC) $(CFLAGS) -c pikl_draw.c $(DEBUG) $(COVER) $(PROF)
pikl_noise.o:
	$(CC) $(CFLAGS) -c pikl_noise.c $(DEBUG) $(COVER) $(PROF)

#=============================================================================
# extra
#=============================================================================
pikl_decrease_median.o:
	$(CC) $(CFLAGS) -c pikl_decrease_median.c $(DEBUG) $(COVER) $(PROF)

pikl_decrease_neuquan5.o:
	$(CC) $(CFLAGS) -c pikl_decrease_neuquan5.c $(DEBUG) $(COVER) $(PROF)

pikl_decrease_wu.o:
	$(CC) $(CFLAGS) -c pikl_decrease_wu.c $(DEBUG) $(COVER) $(PROF)

#dl3q3.o:
#	$(CC) $(CFLAGS) -c decrease/dl3q3.c $(DEBUG) $(COVER) $(PROF)
#
#ccmap.o:
#	$(CC) $(CFLAGS) -c decrease/ccmap.c $(DEBUG) $(COVER) $(PROF)
#
#=============================================================================
# debug
#=============================================================================
dbg_malloc.o:
	$(CC) $(CFLAGS) -c dbg/dbg_malloc.c $(DEBUG) $(COVER) $(PROF)
dbg_malloc_print.o:
	$(CC) $(CFLAGS) -c dbg/dbg_malloc_print.c $(DEBUG) $(COVER) $(PROF)
dbg_malloc_sub.o:
	$(CC) $(CFLAGS) -c dbg/dbg_malloc_sub.c $(DEBUG) $(COVER) $(PROF)

#=============================================================================
# cleanup
#=============================================================================
clean:
	rm -rf *.o *.exe *.dat *.$(EXT) *.gcda *.gcno *.gcov gmon.out *.obj

