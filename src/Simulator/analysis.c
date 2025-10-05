#include "../../include/Simulator/analysis.h"

Matrix* populate_matrix(Netlist* parsed_netlist) {
    if (parsed_netlist->num_nodes < 2) {
        fprintf(stderr, "Cannot populate matrix with num nodes less than 2\n");
        exit(-1);
    }

    Matrix* output_matrix = create_matrix(parsed_netlist->num_nodes, parsed_netlist->num_nodes, MFT_ZEROS);
    return output_matrix;
}
void run_op();

void run_analyses(Netlist* parsed_netlist) {
    char** analyses_names = hashmap_keys(parsed_netlist->analyses);
    Matrix* populated_matrix = populate_matrix(parsed_netlist);
    for (int i = 0; analyses_names[i] != NULL; i++) {
        Analysis* analysis = hashmap_get(parsed_netlist->analyses, analyses_names[i]);
        if (analysis->type == OP) {
            run_op();
        }
    }
}