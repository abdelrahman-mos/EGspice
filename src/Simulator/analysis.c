#include "../../include/Simulator/Analysis/analysis.h"

const char* supported_analyses[] = {"op", "ac", NULL};

// will populate matrices normally but skip inductors and capacitors
void populate_matrices(Netlist* parsed_netlist, Matrix* coeff, Matrix* outputs) {
    if (parsed_netlist->num_nodes < 2) {
        fprintf(stderr, "Cannot populate matrix with num nodes less than 2\n");
        exit(-1);
    }
    char** devices_names = hashmap_keys(parsed_netlist->devices);
    int vsources_size = parsed_netlist->num_vsources+1;
    int inductors_size = parsed_netlist->num_inductors+1;
    char** vsources_names = (char**)calloc(vsources_size, sizeof(char*));
    char** inductors_names = (char**)calloc(inductors_size, sizeof(char*));
    vsources_names[vsources_size-1] = NULL;
    inductors_names[inductors_size-1] = NULL;
    size_t num_vsources = 0;
    size_t num_inductors = 0;
    for (int i = 0; devices_names[i] != NULL; i++) {
        char* device_name = devices_names[i];
        if ((device_name[0] == 'v')) {
            vsources_names[num_vsources++] = my_strdup(device_name);
            continue;
        } else if (device_name[0] == 'c') {
            // since jwc will be equal to zero. Just skip the hassle of stamping it in DC
            continue;
        } else if (device_name[0] == 'l') {
            inductors_names[num_inductors++] = my_strdup(device_name);
            continue;
        }
        Device* device = hashmap_get(parsed_netlist->devices, device_name);
        stamp_device(coeff, outputs, device, 0.0);
    }
    for (size_t i = 0; i < num_vsources; i++) {
        Device* device = hashmap_get(parsed_netlist->devices, vsources_names[i]);
        Vsource* curr_vsource = (Vsource*) device->device_data;
        vsource_stamp(coeff, outputs, curr_vsource, i, num_vsources);
    }

    parsed_netlist->vsources = vsources_names;
    parsed_netlist->inductors = inductors_names;
    free_split_text(devices_names);
}

// replace inductors with zero voltage sources for reshaped matrices
void populate_dc_inductors(Netlist* parsed_netlist, Matrix* coeff, Matrix* outputs) {
    int num = parsed_netlist->num_vsources + parsed_netlist->num_inductors;
    for (int i = parsed_netlist->num_vsources, j=0; i < num; i++, j++) {
        Device* device = hashmap_get(parsed_netlist->devices, parsed_netlist->inductors[j]);
        // populated as a 0 voltage source between the nodes to act as a SC
        // Vx = Vy
        Vsource* curr_vsource = (Vsource*) device->device_data;
        double val = curr_vsource->val;
        if (device->type == INDUCTOR) {
            curr_vsource->val = 0.0;
        }
        vsource_stamp(coeff, outputs, curr_vsource, i, num);
        curr_vsource->val = val;
    }
}

void populate_ac_devices(Netlist* parsed_netlist, Matrix* ac_coeff_matrix, double freq) {
    char** devices_names = hashmap_keys(parsed_netlist->devices);
    for (int i = 0; devices_names[i] != NULL; i++) {
        char* device_name = devices_names[i];
        if ((device_name[0] == 'c') || (device_name[0] == 'l')) {
            Device* device = hashmap_get(parsed_netlist->devices, device_name);
            stamp_device(ac_coeff_matrix, NULL, device, freq);
        }
    }
    free_split_text(devices_names);
}


void print_op(FILE* logfile, Netlist* parsed_netlist, Matrix* output_vars) {
    fprintf(logfile, "###### OPERATING POINT RESULTS ######\n\n");
    int i;
    for (i = 1; parsed_netlist->nodes[i] != NULL; i++) {
        fprintf(logfile, "V(%s)=%.15lf\n", parsed_netlist->nodes[i], cabs(output_vars->pValues[i-1][0]));
    }
    i--;
    for (int j = 0; j < parsed_netlist->num_vsources; i++, j++) {
        fprintf(logfile, "I(%s)=%.15lf\n", parsed_netlist->vsources[j], cabs(output_vars->pValues[i][0]));
    }
    i--;
    for (int j = 0; j < parsed_netlist->num_inductors; i++, j++) {
        fprintf(logfile, "I(%s)=%.15lf\n", parsed_netlist->inductors[j], cabs(output_vars->pValues[i][0]));
    }
    fprintf(logfile, "\nOP analysis finished successfully\n");
}

void print_ac(FILE* logfile, Netlist* parsed_netlist, Matrix** ac_outputs, double* freqs) {
    fprintf(logfile, "###### AC Analysis RESULTS ######\n\n");
    int i;
    for (i = 1; parsed_netlist->nodes[i] != NULL; i++) {
        fprintf(logfile, "\t\tFREQ\t\tV(%s)\n", parsed_netlist->nodes[i]);
        for (int j = 0; freqs[j] > 0.0; j++) {
            double real = creal(ac_outputs[j]->pValues[i-1][0]);
            double imag = cimag(ac_outputs[j]->pValues[i-1][0]);
            fprintf(logfile, "\t\t%lf\t\t%lf%s%lfi\n",
                freqs[j],
                real,
                (imag >= 0.0) ? "+" : "-",
                cabs(imag)
            );
            // double abs = cabs(ac_outputs[j]->pValues[i-1][0]);
            // double arg = carg(ac_outputs[j]->pValues[i-1][0]) * 180.0 / M_PI;
            // fprintf(logfile, "\t\t%lf\t\t%lf<%lf\n", freqs[j],
            //     abs,
            //     arg
            // );
        }
        fprintf(logfile, "\n\n");
    }
    i--;
    for (int j = 0; j < parsed_netlist->num_vsources; i++, j++) {
        fprintf(logfile, "\t\tFREQ\t\tI(%s)\n", parsed_netlist->vsources[j]);
        for (int k = 0; freqs[k] > 0.0; k++) {
            double real = creal(ac_outputs[k]->pValues[i][0]);
            double imag = cimag(ac_outputs[k]->pValues[i][0]);
            fprintf(logfile, "\t\t%lf\t\t%lf%s%lfi\n", freqs[k],
                real,
                (imag >= 0.0) ? "+" : "",
                cabs(imag)
            );
            // double abs = cabs(ac_outputs[k]->pValues[i][0]);
            // double arg = carg(ac_outputs[k]->pValues[i][0]);
            // fprintf(logfile, "\t\t%lf\t\t%lf<%lfi\n", freqs[k],
            //     abs,
            //     arg
            // );
        }
    }
    fprintf(logfile, "\nAC analysis finished successfully\n");
}

void run_op(Netlist* parsed_netlist, Matrix* coeff, Matrix* outputs, FILE* logfile) {
    Matrix* reshaped_coeff_matrix = coeff;
    Matrix* reshaped_outputs_matrix = outputs;
    if (parsed_netlist->num_inductors > 0) {
        reshaped_coeff_matrix = reshape_matrix(
            coeff, 
            coeff->nRows+parsed_netlist->num_inductors, 
            coeff->nCols+parsed_netlist->num_inductors,
            MFT_ZEROS
        );
        reshaped_outputs_matrix = reshape_matrix(
            outputs, 
            outputs->nRows+parsed_netlist->num_inductors, 
            1,
            MFT_ZEROS
        );
        populate_dc_inductors(parsed_netlist, reshaped_coeff_matrix, reshaped_outputs_matrix);
    }
    fprintf(logfile, "running OP analysis\n");
    Matrix* vars_values = solve_matrix(reshaped_coeff_matrix, reshaped_outputs_matrix);
    print_op(logfile, parsed_netlist, vars_values);
    destroy_matrix(vars_values);
    // felt cute, might delete later
    if (reshaped_coeff_matrix != coeff) {
        destroy_matrix(reshaped_coeff_matrix);
        destroy_matrix(reshaped_outputs_matrix);
    }
}

void run_ac(Netlist* parsed_netlist, AC_Analysis* analysis, Matrix* coeff, Matrix* outputs, FILE* logfile) {
    double* freqs = expand_freq(analysis);
    int num_freqs = 0;
    for (num_freqs = 0; freqs[num_freqs] > 0.0; num_freqs++);
    Matrix** ac_outputs = calloc(num_freqs, sizeof(Matrix*));
    fprintf(logfile, "running AC analysis\n");
    fprintf(logfile, "\tfstart = %lf\n", analysis->start);
    fprintf(logfile, "\tfend = %lf\n", analysis->end);
    fprintf(logfile, "\ttype = %s\n", (analysis->type == DEC) ? "dec" : (analysis->type == OCT) ? "oct" : "lin");
    for (int i = 0; i < num_freqs; i++) {
        Matrix* ac_coeff_matrix = copy_matrix(coeff);
        populate_ac_devices(parsed_netlist, ac_coeff_matrix, freqs[i]);
        ac_outputs[i] = solve_matrix(ac_coeff_matrix, outputs);
        destroy_matrix(ac_coeff_matrix);
    }
    print_ac(logfile, parsed_netlist, ac_outputs, freqs);
    for (int i = 0; i < num_freqs; i++) {
        destroy_matrix(ac_outputs[i]);
    }
    free(ac_outputs);
    free(freqs);
}

void run_analyses(Netlist* parsed_netlist, FILE* logfile) {
    char** analyses_names = hashmap_keys(parsed_netlist->analyses);
    int matrix_size = parsed_netlist->num_nodes + parsed_netlist->num_vsources;
    Matrix* coeff_matrix = create_matrix(matrix_size, matrix_size, MFT_ZEROS);
    Matrix* outputs_matrix = create_matrix(matrix_size, 1, MFT_ZEROS);
    populate_matrices(parsed_netlist, coeff_matrix, outputs_matrix);
    for (int i = 0; analyses_names[i] != NULL; i++) {
        Analysis* analysis = hashmap_get(parsed_netlist->analyses, analyses_names[i]);
        if (analysis->type == OP) {
            run_op(parsed_netlist, coeff_matrix, outputs_matrix, logfile);
        } else if (analysis->type == AC) {
            run_ac(parsed_netlist, analysis->analysis_data, coeff_matrix, outputs_matrix, logfile);
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

void free_analysis(void* input) {
    Analysis* analysis = (Analysis*) input;
    if (analysis->analysis_data != NULL) {
        free(analysis->analysis_data);
    }
    free(analysis);
}