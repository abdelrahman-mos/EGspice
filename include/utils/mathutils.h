#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include "../Simulator/Analysis/AcAnalysis.h"

#define MX_ATOL 1e-18
#define M_PI 3.14159265358979323846	/* pi */

double* expand_freq(AC_Analysis* analysis);

#endif