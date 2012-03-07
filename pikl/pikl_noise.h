#ifndef _PIKL_NOISE_
#define _PIKL_NOISE_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "pikl.h"
#include "pikl_private.h"
#include "pikl_algorithm.h"

#define NOISE_B  0x100

struct PERLIN_NOISE_DATA {
	int p[NOISE_B + NOISE_B + 2];
	double g1[NOISE_B + NOISE_B + 2];
	double g2[NOISE_B + NOISE_B + 2][2];
	double g3[NOISE_B + NOISE_B + 2][3];
};

#endif
