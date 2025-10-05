#include "../../include/Simulator/analysis.h"

const char* supported_analyses[] = {"op", NULL};

Matrix* populate_matrix(Netlist* parsed_netlist) {
    if (parsed_netlist->num_nodes < 2) {
        fprintf(stderr, "Cannot populate matrix with num nodes less than 2\n");
        exit(-1);
    }

    Matrix* output_matrix = create_matrix(parsed_netlist->num_nodes, parsed_netlist->num_nodes, MFT_ZEROS);
    return output_matrix;
}

void run_op() {
    FILE* logfile = fopen("output.log", "a");
    fprintf(logfile, "running OP analysis");
    return;
}

void run_analyses(Netlist* parsed_netlist) {
    char** analyses_names = hashmap_keys(parsed_netlist->analyses);
    Matrix* populated_matrix = populate_matrix(parsed_netlist);
    for (int i = 0; analyses_names[i] != NULL; i++) {
        Analysis* analysis = hashmap_get(parsed_netlist->analyses, analyses_names[i]);
        if (analysis->type == OP) {
            run_op();
        }
    }
    destroy_matrix(populated_matrix);
}

char is_supported_analysis(char* analysis) {
    for (int i = 0; supported_analyses[i] != NULL; i++) {
        if (analysis[0] == '.') {
            if (strcmp(analysis+1, supported_analyses[i]) == 0) {
                return 1;
            }
        } else {
            if (strcmp(analysis, supported_analyses[i]) == 0) {
                return 1;
            }
        }
    }
    return 0;
}