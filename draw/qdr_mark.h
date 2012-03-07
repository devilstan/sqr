#ifndef __QDR_MARK__
#define __QDR_MARK__

#include <math.h>
#include "qdr_private.h"

void mark_rectangle(Qdr *qdr, cairo_t *cr, int _x, int _y);
void mark_arc(Qdr *qdr, cairo_t *cr, int _x, int _y);
void mark_blockarc(Qdr *qdr, cairo_t *cr, int _x, int _y);
void mark_metaball(Qdr *qdr, cairo_t *cr, int _x, int _y);

#endif
