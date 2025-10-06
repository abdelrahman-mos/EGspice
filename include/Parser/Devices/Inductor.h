#include "../../utils/MxAlg.h"

typedef struct Inductor
{
    char* name;
    int node1;
    int node2;
    double val;
} Inductor;

// in DC or OP, it should just act as a short circuit, this will introduce GL=-INF,
// Thus we introduce a new equation for DC where Vn1 = Vn2, or just by adding a dummy
// voltage source instead of the inductor when stamping the matrix
void inductor_stamp(Matrix* coeff, Matrix* outputs, Inductor* device);
