#include "../../include/Simulator/analysis.h"

const char* supported_analyses[] = {"op", NULL};

void populate_matrices_dc(Netlist* parsed_netlist, Matrix* coeff, Matrix* outputs) {
    if (parsed_netlist->num_nodes < 2) {
        fprintf(stderr, "Cannot populate matrix with num nodes less than 2\n");
        exit(-1);
    }
    char** devices_names = hashmap_keys(parsed_netlist->devices);
    char** vsources_names = (char**)calloc(parsed_netlist->num_vsources, sizeof(char*));
    int num_vsources = 0;
    for (int i = 0; devices_names[i] != NULL; i++) {
        char* device_name = devices_names[i];
        if (device_name[0] == 'v') {
            vsources_names[num_vsources++] = device_name;
            continue;
        } else if (device_name[0] == 'c') {
            // since jwc will be equal to zero.
            continue;
        }
        Device* device = hashmap_get(parsed_netlist->devices, devices_names[i]);
        stamp_device(coeff, outputs, device);
    }

    for (int i = 0; i < num_vsources; i++) {
        Device* device = hashmap_get(parsed_netlist->devices, vsources_names[i]);
        Vsource* curr_vsource = (Vsource*) device->device_data;
        vsource_stamp(coeff, outputs, curr_vsource, i, num_vsources);
    }

    parsed_netlist->vsources = vsources_names;
}

void print_op(FILE* logfile, Netlist* parsed_netlist, Matrix* output_vars) {
    int i;
    for (i = 1; parsed_netlist->nodes[i] != NULL; i++) {
        fprintf(logfile, "V(%s)=%.15lf\n", parsed_netlist->nodes[i], output_vars->pValues[i-1][0]);
    }
    i--;
    for (int j = 0; i < parsed_netlist->num_nodes+parsed_netlist->num_vsources; i++, j++) {
        fprintf(logfile, "I(%s)=%.15lf\n", parsed_netlist->vsources[j], output_vars->pValues[i][0]);
    }
    fprintf(logfile, "OP analysis finished successfully\n");
}

void run_op(Netlist* parsed_netlist, Matrix* coeff, Matrix* outputs) {
    FILE* logfile = fopen("output.log", "w+");
    fprintf(logfile, "running OP analysis\n");
    Matrix* vars_values = solve_matrix(coeff, outputs);
    print_op(logfile, parsed_netlist, vars_values);
    fclose(logfile);
}

void run_analyses(Netlist* parsed_netlist) {
    char** analyses_names = hashmap_keys(parsed_netlist->analyses);
    int matrix_size = parsed_netlist->num_nodes + parsed_netlist->num_vsources;
    Matrix* coeff_matrix = create_matrix(matrix_size, matrix_size, MFT_ZEROS);
    Matrix* outputs_matrix = create_matrix(matrix_size, 1, MFT_ZEROS);
    for (int i = 0; analyses_names[i] != NULL; i++) {
        Analysis* analysis = hashmap_get(parsed_netlist->analyses, analyses_names[i]);
        if (analysis->type == OP) {
            populate_matrices_dc(parsed_netlist, coeff_matrix, outputs_matrix);
            run_op(parsed_netlist, coeff_matrix, outputs_matrix);
        }
    }
    destroy_matrix(coeff_matrix);
    destroy_matrix(outputs_matrix);
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