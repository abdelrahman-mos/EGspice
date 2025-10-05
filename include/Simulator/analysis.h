#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "../Parser/netlist.h"
#include "../utils/MxAlg.h"

const char* supported_analyses = {"op"};

typedef enum {
    OP
} Analysis_type;

typedef struct Analysis
{
    Analysis_type type;
    char* analysis_name;
} Analysis;


// takes the parsed netlist, populates the matrix, and runs different analyses that are found in the netlist
void run_analyses(Netlist* parsed_netlist);

#endif