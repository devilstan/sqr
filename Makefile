CPP=g++
DEBUG = -DDEBUG -g -I.

CFLAGS=-O3 -Wall -c
LFLAGS=

PIKL_CFLAGS =
PIKL_LFLAGS = -L. -lpikl

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
	qdr_group.o
PIKL = libpikl.dll

OBJECTS = $(QR_ENCODE) $(QR_DRAW)

#=============================================================================
# all
#=============================================================================
SAMPLES_CFLAGS=-Iencode -Idraw

all: $(PIKL) $(QR_ENCODE) $(QR_DRAW)
	$(CPP) $(SAMPLES_CFLAGS) $(CFLAGS) sample.cpp
	$(CPP) $(OBJECTS) $(LFLAGS) sample.o

#=============================================================================
# libpikl
#=============================================================================
libpikl.dll:
	cd draw/pikl; make;
	mv draw/pikl/libpikl.dll .

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
	$(CPP) $(CFLAGS) draw/qdr_draw.c
qdr_largest.o:
	$(CPP) $(CFLAGS) draw/qdr_largest.c
qdr_filter.o:
	$(CPP) $(CFLAGS) draw/qdr_filter.c
qdr_stack.o:
	$(CPP) $(CFLAGS) draw/qdr_stack.c
qdr_version.o:
	$(CPP) $(CFLAGS) draw/qdr_version.c
qdr_group.o:
	$(CPP) $(CFLAGS) draw/qdr_group.c

#=============================================================================
# clean
#=============================================================================
clean:
	rm -rf *.o *.exe
	cd draw/pikl; make clean

