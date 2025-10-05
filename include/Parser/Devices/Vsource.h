#include "../../utils/MxAlg.h"

typedef struct Vsource
{
    char* name;
    int node1;
    int node2;
    double val;
} Vsource;

void vsource_stamp(Matrix* coeff, Matrix* outputs, Vsource* device, int curr_num, int total_vsources);
