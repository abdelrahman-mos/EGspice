#include "../include/utils/map.h"
#include "../include/utils/strutils.h"

int cmp_func(const void* a, const void* b) {
    return strcmp((const char *) a, (const char *) b);
}

int main() {
    HashMap* my_map = hashmap_create(10, hash_string, cmp_func, free, free);

    hashmap_insert(my_map, my_strdup("name"), my_strdup("badass"));

    int* age = malloc(sizeof(int));
    *age = 23;

    hashmap_insert(my_map, my_strdup("age"), age);

    printf("name: %s\n", (char*) hashmap_get(my_map, "name"));
    printf("age: %d\n", *(int*) hashmap_get(my_map, "age"));

    hashmap_destroy(my_map);
    return 0;
}