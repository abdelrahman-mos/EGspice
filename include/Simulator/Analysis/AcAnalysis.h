#ifndef AC_ANALYSIS_H
#define AC_ANALYSIS_H

typedef enum {
    OCT,
    DEC,
    LIN
} AC_TYPE;

typedef struct 
{
    AC_TYPE type;
    int num_points;
    double start;
    double end;
} AC_Analysis;
#endif