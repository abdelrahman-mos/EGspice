#include "../../utils/MxAlg.h"

typedef struct Isource
{
    char* name;
    int node1;
    int node2;
    double val;
} Isource;

void isource_stamp(Matrix* coeff, Matrix* outputs, Isource* device);
