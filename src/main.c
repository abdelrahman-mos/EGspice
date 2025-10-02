#include "../include/utils/map.h"
#include "../include/utils/strutils.h"
#include "../include/Parser/SPICE/spiceParser.h"
#include "../include/utils/MxAlg.h"

// int cmp_func(const void* a, const void* b) {
//     return strcmp((const char *) a, (const char *) b);
// }

int main() {
    // HashMap* my_map = hashmap_create(10, hash_string, cmp_func, free, free);

    // hashmap_insert(my_map, my_strdup("name"), my_strdup("badass"));

    // int* age = malloc(sizeof(int));
    // *age = 23;

    // hashmap_insert(my_map, my_strdup("age"), age);

    // printf("name: %s\n", (char*) hashmap_get(my_map, "name"));
    // printf("age: %d\n", *(int*) hashmap_get(my_map, "age"));

    // hashmap_destroy(my_map);

    // spiceParser* parsed_netlist = parse_netlist("test_netlists/voltage_divider.cir");
    // char* analysis = (char*)hashmap_get(parsed_netlist->analyses, "an1");
    // printf("analysis: %s\n", analysis);
    // char** devices_names = hashmap_keys(parsed_netlist->devices);
    // for (int i = 0; devices_names[i] != NULL; i++) {
    //     char* device_name = devices_names[i];
    //     Device* device = hashmap_get(parsed_netlist->devices, device_name);
    //     if (device->type == VSOURCE) {
    //         Vsource* device_data = (Vsource*) device->device_data;
    //         print_device(device_data);
    //     } else if (device->type == ISOURCE)
    //     {
    //         Isource* device_data = (Isource*) device->device_data;
    //         print_device(device_data);
    //     } else if (device->type == RESISTOR)
    //     {
    //         Resistor* device_data = (Resistor*) device->device_data;
    //         print_device(device_data);
    //     }        
    // }
    // free_parser(parsed_netlist);
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

    Matrix* mat = create_matrix(2, 2, MFT_RAND);
    Matrix* mat_t = mat_transpose(mat);
    // Matrix* mat2 = create_matrix(4, 4, MFT_ONES);
    // Matrix* mat3 = create_matrix(2, 4, MFT_IDENTITY);
    // Matrix* mat5 = create_matrix(4, 4, MFT_IDENTITY);
    printf("matrix1:\n");
    print_matrix(mat);
    printf("matrix_t:\n");
    print_matrix(mat_t);
    Matrix* output = mat_mul(mat, mat_t);
    printf("matrix1 * matrix2:\n");
    print_matrix(output);
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
    destroy_matrix(mat);
    destroy_matrix(mat_t);
    destroy_matrix(output);
    // destroy_matrix(mat2);
    // destroy_matrix(mat3);
    // destroy_matrix(mat4);
    // destroy_matrix(mat5);
    // destroy_matrix(mat6);
    // destroy_matrix(mat7);
    
    return 0;
}