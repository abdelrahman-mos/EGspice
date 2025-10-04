#ifndef NETLIST_H
#define NETLIST_H
#include "../utils/map.h"

typedef struct Netlist
{
    HashMap* devices;
    HashMap* options;
    HashMap* analyses;
    char** nodes;
    int num_nodes;
} Netlist;

#endif