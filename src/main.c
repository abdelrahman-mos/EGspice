#include "../include/utils/map.h"
#include "../include/utils/strutils.h"
#include "../include/Parser/SPICE/spiceParser.h"

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
    // free_parser(parsed_netlist);
    char my_text[] = "Hello my name is abdelrahman mostafa";
    char** my_text_split = splittext(my_text, " ");
    if (my_text_split == NULL) {
        printf("null output\n");
        return 0;
    }
    for (int i = 0; my_text_split[i] != NULL; i++) {
        printf("index %d: %s\n", i, my_text_split[i]);
    }
    for (int i = 0; my_text_split[i] != NULL; i++) {
        free(my_text_split[i]);
    }
    // free(my_text_split[i]);
    free(my_text_split);
    return 0;
}