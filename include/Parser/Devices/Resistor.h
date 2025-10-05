#include "../../utils/MxAlg.h"

typedef struct Resistor
{
    char* name;
    int node1;
    int node2;
    double val;
} Resistor;

void resistor_stamp(Matrix* coeff, Matrix* outputs, Resistor* device);