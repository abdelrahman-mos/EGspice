#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "../Parser/netlist.h"
#include "../utils/MxAlg.h"
#include "../utils/strutils.h"
#include "../Parser/Devices/device.h"
#include <string.h>

typedef enum {
    OP
} Analysis_type;

typedef enum {
    DEC,
    OCT,
    LIN
} AC_TYPE;

typedef struct Analysis
{
    Analysis_type type;
    char* analysis_name;
} Analysis;


// takes the parsed netlist, populates the matrix, and runs different analyses that are found in the netlist
void run_analyses(Netlist* parsed_netlist, FILE* logfile);
char is_supported_analysis(char* analysis);

#endif