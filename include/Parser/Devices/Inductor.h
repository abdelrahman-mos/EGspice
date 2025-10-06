#include "../../utils/MxAlg.h"

typedef struct Inductor
{
    char* name;
    int node1;
    int node2;
    double val;
} Inductor;

void inductor_stamp(Matrix* coeff, Matrix* outputs, Inductor* device);
