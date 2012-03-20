#ifndef __QDR_EYE__
#define __QDR_EYE__

#include "qdr_private.h"
#include "qdr_import.h"

void eye_mark_init(Qdr *qdr);
int eye_check(Qdr *qdr, int x, int y);
void eye_paint(Qdr *qdr, cairo_t *cr, int x, int y);
void qdr_eye_outer_color(Qdr *qdr, int index, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void qdr_eye_inner_color(Qdr *qdr, int index, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void qdr_eye_image(Qdr *qdr, int index, const char *file);
void qdr_eye_outer_color_reset(Qdr *qdr, int index);
void qdr_eye_inner_color_reset(Qdr *qdr, int index);
void qdr_eye_image_reset(Qdr *qdr, int index);

#endif
