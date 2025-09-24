#include "include/utils/map.h"
#include <stdlib.h>

unsigned long hash_string(const void* str) {
    const char* s = str;
    // super random number idk
    unsigned long hash = 5381;
    int c;
    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/* 
capacity is how many buckets this can hold
buckets are the number of members inside the hashmap
*/
HashMap* hashmap_create(
    size_t capacity,
    unsigned long (*hash_func)(const void *),
    int (*cmp_func)(const void*, const void*),
    void (*free_key)(void*),
    void (*free_val)(void*)
) {
    HashMap* map = malloc(sizeof(HashMap));
    map->capacity = capacity;
    map->size = 0;
    map->buckets = calloc(capacity, sizeof(HashNode*));
    map->hash_func = hash_func;
    map->cmp_func = cmp_func;
    map->free_key = free_key;
    map->free_val = free_val;
}

void hashmap_insert(HashMap* map, void* key, void* val) {
    unsigned long hash = map->hash_func(key);
    size_t index = hash % map->capacity; // don't understand this part ?

    HashNode* node = map->buckets[index];
    while (node) {
        if (map->cmp_func(key, node->key) == 0) {
            if (map->free_val) map->free_val(node->val);
            node->val = val;
            return;
        }
        node = node->next;
    }

    HashNode* new_node = malloc(sizeof(HashNode));
    new_node->key = key;
    new_node->val = val;
    new_node->next = map->buckets[index];
    map->buckets[index] = new_node;
    map->size++;
}

void* hashmap_get(HashMap* map, void* key) {
    unsigned long hash = map->hash_func(key);
    size_t index = hash % map->capacity;

    HashNode* node = map->buckets[index];
    while (node) {
        if (map->cmp_func(key, node->key) == 0) return node->val;
        node = node->next;
    }
    return NULL;
}

void hashmap_destroy(HashMap* map) {
    for (size_t i = 0; i < map->capacity; i++) {
        HashNode* node = map->buckets[i];
        while (node) {
            HashNode* next = node->next;
            if (map->free_key) map->free_key(node->key);
            if (map->free_val) map->free_val(node->val);
            free(node);
            node = next;
        }
    }
    free(map->buckets);
    free(map);
}