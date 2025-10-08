#include "../../include/utils/mathutils.h"
#include <stdio.h>

double* expand_freq_dec(int num_points, double start, double end) {
    // num_points here represent number of points per decade
    double log_start = log10(start);
    double log_end = log10(end);
    int total_points = (int)((log_end - log_start) * num_points);
    double* output = (double*)calloc(total_points+2, sizeof(double));

    double step = (log_end - log_start) / total_points;
    double exponent = log_start;
    output[0] = start;
    for (int i = 1; i < total_points; i++) {
        exponent += step;
        output[i] = pow(10.0, exponent);
    }
    output[total_points] = end;
    output[total_points+1] = -1.0;
    return output;
}

double* expand_freq_oct(int num_points, double start, double end) {
    // num_points here represent number of points per octave
    double log_start = log2(start);
    double log_end = log2(end);
    int total_points = (int)((log_end - log_start) * num_points);
    double* output = (double*)calloc(total_points+2, sizeof(double));
    double step = 1.0/num_points;
    double exponent = log_start;
    for (int i = 0; i < total_points+1; i++) {
        output[i] = pow(2.0, exponent);
        exponent += step;
    }
    output[total_points+1] = -1.0;
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