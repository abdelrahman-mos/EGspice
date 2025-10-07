#ifndef NETLIST_H
#define NETLIST_H
#include "../utils/map.h"

typedef struct Netlist
{
    HashMap* devices;
    HashMap* options;
    HashMap* analyses;
    char** nodes;
    char** vsources;
    char** inductors;
    int num_nodes;
    int num_vsources;
    int num_inductors;
} Netlist;

#endif