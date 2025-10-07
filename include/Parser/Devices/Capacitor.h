#include "../../utils/MxAlg.h"

typedef struct Capacitor
{
    char* name;
    int node1;
    int node2;
    double val;
} Capacitor;

void capacitor_stamp(Matrix* coeff, Matrix* outputs, Capacitor* device, double frequency);
