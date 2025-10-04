#include "../include/utils/map.h"
#include "../include/utils/strutils.h"
#include "../include/Parser/SPICE/spiceParser.h"
#include "../include/utils/MxAlg.h"

// int cmp_func(const void* a, const void* b) {
//     return strcmp((const char *) a, (const char *) b);
// }

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("missing netlist\n");
        exit(-1);
    }
    if (strcmp(argv[1], "-h") == 0) {
        printf("EGspice:\nUsage:\n\tEGspice <netlist_path>\n");
        exit(0);
    }
    // HashMap* my_map = hashmap_create(10, hash_string, cmp_func, free, free);

    // hashmap_insert(my_map, my_strdup("name"), my_strdup("badass"));

    // int* age = malloc(sizeof(int));
    // *age = 23;

    // hashmap_insert(my_map, my_strdup("age"), age);

    // printf("name: %s\n", (char*) hashmap_get(my_map, "name"));
    // printf("age: %d\n", *(int*) hashmap_get(my_map, "age"));

    // hashmap_destroy(my_map);

    Netlist* parsed_netlist = parse_netlist(argv[1]);
    char* analysis = (char*)hashmap_get(parsed_netlist->analyses, "an1");
    printf("analysis: %s\n", analysis);
    char** devices_names = hashmap_keys(parsed_netlist->devices);
    for (int i = 0; devices_names[i] != NULL; i++) {
        char* device_name = devices_names[i];
        Device* device = hashmap_get(parsed_netlist->devices, device_name);
        if (device->type == VSOURCE) {
            Vsource* device_data = (Vsource*) device->device_data;
            print_device(device_data);
        } else if (device->type == ISOURCE)
        {
            Isource* device_data = (Isource*) device->device_data;
            print_device(device_data);
        } else if (device->type == RESISTOR)
        {
            Resistor* device_data = (Resistor*) device->device_data;
            print_device(device_data);
        }        
    }
    for (int i = 0; parsed_netlist->nodes[i] != NULL; i++) {
        printf("node num %d corresponds to node name %s\n", i, parsed_netlist->nodes[i]);
    }
    free_parser(parsed_netlist);

    // char* line = "v1 va 0 32v";
    // char** line_split = splittext(line, " ");
    // for (int i = 0; line_split[i] != NULL; i++) {
    //     printf("curr = %s\n", line_split[i]);
    //     printf("I cannot take this anymore %d\n", strcmp(line_split[i], "0"));
    // }

    // char my_text[] = "Hello my name is abdelrahman mostafa";
    // char** my_text_split = splittext(my_text, " ");
    // if (my_text_split == NULL) {
    //     printf("null output\n");
    //     return 0;
    // }
    // for (int i = 0; my_text_split[i] != NULL; i++) {
    //     printf("index %d: %s\n", i, my_text_split[i]);
    // }
    // for (int i = 0; my_text_split[i] != NULL; i++) {
    //     free(my_text_split[i]);
    // }
    // free(my_text_split[i]);
    // free(my_text_split);
    // char* my_text = "  x   =  22n hello";
    // char* output_text = regex_replace("\\s*=\\s*", my_text, "=");

    // if (output_text) {
    //     printf("Original: %s\n", my_text);
    //     printf("Output: %s\n", output_text);
    //     free(output_text);
    // }

    // Matrix* mat_A = create_matrix(3, 3, MFT_NONE);
    // Matrix* mat_B = create_matrix(3, 1, MFT_NONE);
    // double A[3][3] = {
    //     {2.0, 1.0, -1.0},
    //     {-3.0, -1.0, 2.0},
    //     {-2.0, 1.0, 2.0}
    // };
    // double B[3][1] = {
    //     {8},
    //     {-11},
    //     {-3}
    // };
    // for(int i = 0; i < 3; i++) {
    //     memcpy(mat_A->pValues[i], A[i], 3*sizeof(double));
    // }
    // for(int i = 0; i < 3; i++) {
    //     memcpy(mat_B->pValues[i], B[i], sizeof(double));
    // }
    // Matrix* mat2 = create_matrix(4, 4, MFT_ONES);
    // Matrix* mat3 = create_matrix(2, 4, MFT_IDENTITY);
    // Matrix* mat5 = create_matrix(4, 4, MFT_IDENTITY);
    // printf("matrix A:\n");
    // print_matrix(mat_A);
    // printf("matrix B:\n");
    // print_matrix(mat_B);
    // Matrix* output = solve_matrix(mat_A, mat_B);
    // printf("variables:\n");
    // print_matrix(output);
    // printf("matrix3:\n");
    // print_matrix(mat5);
    // Matrix* mat4 = mat_add(mat, mat2);
    // printf("matrix1 + matrix2:\n");
    // print_matrix(mat4);
    // Matrix* mat6 = mat_add(mat2, mat5);
    // printf("matrix2 + matrix3:\n");
    // print_matrix(mat6);
    // Matrix* mat7 = mat_sum(3, mat, mat2, mat5);
    // printf("matrix1 + matrix2 + matrix3:\n");
    // print_matrix(mat7);
    // mat_add(mat, mat3);
    // destroy_matrix(mat_A);
    // destroy_matrix(mat_B);
    // destroy_matrix(output);
    // destroy_matrix(mat3);
    // destroy_matrix(mat2);
    // destroy_matrix(mat3);
    // destroy_matrix(mat4);
    // destroy_matrix(mat5);
    // destroy_matrix(mat6);
    // destroy_matrix(mat7);
    
    return 0;
}