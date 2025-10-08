#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "../../Parser/netlist.h"
#include "../../utils/strutils.h"
#include "../../utils/mathutils.h"
#include "../../Parser/Devices/device.h"
#include "AcAnalysis.h"
#include <string.h>

typedef enum {
    OP,
    AC
} Analysis_type;

typedef struct Analysis
{
    Analysis_type type;
    char* analysis_name;
    void* analysis_data;
} Analysis;

// takes the parsed netlist, populates the matrix, and runs different analyses that are found in the netlist
void run_analyses(Netlist* parsed_netlist, FILE* logfile);
char is_supported_analysis(char* analysis);

#endif