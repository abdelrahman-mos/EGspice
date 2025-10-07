#include "../../include/Simulator/analysis.h"

const char* supported_analyses[] = {"op", NULL};

void populate_matrices_dc(Netlist* parsed_netlist, Matrix* coeff, Matrix* outputs) {
    if (parsed_netlist->num_nodes < 2) {
        fprintf(stderr, "Cannot populate matrix with num nodes less than 2\n");
        exit(-1);
    }
    char** devices_names = hashmap_keys(parsed_netlist->devices);
    int vsources_size = parsed_netlist->num_vsources+parsed_netlist->num_inductors+1;
    char** vsources_names = (char**)calloc(vsources_size, sizeof(char*));
    vsources_names[vsources_size-1] = NULL;
    int num_vsources = 0;
    for (int i = 0; devices_names[i] != NULL; i++) {
        char* device_name = devices_names[i];
        if ((device_name[0] == 'v') || (device_name[0] == 'l')) {
            vsources_names[num_vsources++] = my_strdup(device_name);
            continue;
        } else if (device_name[0] == 'c') {
            // since jwc will be equal to zero.
            continue;
        }
        Device* device = hashmap_get(parsed_netlist->devices, device_name);
        stamp_device(coeff, outputs, device);
    }
    for (int i = 0; i < num_vsources; i++) {
        Device* device = hashmap_get(parsed_netlist->devices, vsources_names[i]);
        Vsource* curr_vsource = (Vsource*) device->device_data;
        double val = curr_vsource->val;
        if (device->type == INDUCTOR) {
            curr_vsource->val = 0.0;
        }
        vsource_stamp(coeff, outputs, curr_vsource, i, num_vsources);
        curr_vsource->val = val;
    }

    parsed_netlist->vsources = vsources_names;
    free_split_text(devices_names);
}

void print_op(FILE* logfile, Netlist* parsed_netlist, Matrix* output_vars) {
    int i;
    for (i = 1; parsed_netlist->nodes[i] != NULL; i++) {
        fprintf(logfile, "V(%s)=%.15lf\n", parsed_netlist->nodes[i], output_vars->pValues[i-1][0]);
    }
    i--;
    int num = parsed_netlist->num_nodes+parsed_netlist->num_vsources+parsed_netlist->num_inductors;
    for (int j = 0; i < num; i++, j++) {
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
    destroy_matrix(vars_values);
}

void run_analyses(Netlist* parsed_netlist) {
    char** analyses_names = hashmap_keys(parsed_netlist->analyses);
    int matrix_size = parsed_netlist->num_nodes + parsed_netlist->num_vsources;
    Matrix* coeff_matrix = create_matrix(matrix_size, matrix_size, MFT_ZEROS);
    Matrix* outputs_matrix = create_matrix(matrix_size, 1, MFT_ZEROS);
    for (int i = 0; analyses_names[i] != NULL; i++) {
        Analysis* analysis = hashmap_get(parsed_netlist->analyses, analyses_names[i]);
        if (analysis->type == OP) {
            Matrix* reshaped_coeff_matrix = coeff_matrix;
            Matrix* reshaped_outputs_matrix = outputs_matrix;
            if (parsed_netlist->num_inductors > 0) {
                reshaped_coeff_matrix = reshape_matrix(
                    coeff_matrix, 
                    coeff_matrix->nRows+parsed_netlist->num_inductors, 
                    coeff_matrix->nCols+parsed_netlist->num_inductors,
                    MFT_ZEROS
                );
                reshaped_outputs_matrix = reshape_matrix(
                    outputs_matrix, 
                    outputs_matrix->nRows+parsed_netlist->num_inductors, 
                    1,
                    MFT_ZEROS
                );
            }
            populate_matrices_dc(parsed_netlist, reshaped_coeff_matrix, reshaped_outputs_matrix);
            run_op(parsed_netlist, reshaped_coeff_matrix, reshaped_outputs_matrix);
            // felt cute, might delete later
            if (reshaped_coeff_matrix != coeff_matrix) {
                destroy_matrix(reshaped_coeff_matrix);
                destroy_matrix(reshaped_outputs_matrix);
            }
        }
    }
    destroy_matrix(coeff_matrix);
    destroy_matrix(outputs_matrix);
    free_split_text(analyses_names);
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