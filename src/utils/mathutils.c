#include "../../include/utils/mathutils.h"

double* expand_freq_dec(int num_points, double start, double end) {
    // num_points here represent number of points per decade
    double* output = (double*)calloc(3, sizeof(double));
    output[0] = start;
    output[1] = end;
    output[2] = -1.0;
    return output;
}

double* expand_freq_oct(int num_points, double start, double end) {
    // num_points here represent number of points per octave
    double* output = (double*)calloc(3, sizeof(double));
    output[0] = start;
    output[1] = end;
    output[2] = -1.0;
    return output;
}

double* expand_freq_lin(int num_points, double start, double end) {
    // num_points here represent total number of points between start and end
    double* output = (double*)calloc(num_points+2, sizeof(double));
    double step = (end - start) / num_points;
    double freq = start;
    for (int i = 0; i < num_points+1; i++) {
        output[i] = freq;
        freq += step;
    }
    output[num_points+1] = -1;
}
double* expand_freq(AC_Analysis* analysis) {
    double* output_frequencies;
    switch (analysis->type)
    {
    case DEC:
        output_frequencies = expand_freq_dec(analysis->num_points, analysis->start, analysis->end);
        break;
    case OCT:
        output_frequencies = expand_freq_oct(analysis->num_points, analysis->start, analysis->end);
        break;
    case LIN:
        output_frequencies = expand_freq_lin(analysis->num_points, analysis->start, analysis->end);
        break;
    default:
        break;
    }
}