#include "../../include/utils/map.h"
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
    return map;
}

void hashmap_resize(HashMap* map, size_t new_capacity) {
    HashNode** new_buckets = calloc(new_capacity, sizeof(HashNode*));
    
    if (!new_buckets) {
        // Allocation failed → keep old map unchanged
        fprintf(stderr, "hashmap_resize: failed to allocate %zu buckets\n", new_capacity);
        return;
    }

    for (size_t i = 0; i < map->capacity; i++) {
        HashNode* node = map->buckets[i];
        while (node) {
            HashNode* next = node->next;
            unsigned long hash = map->hash_func(node->key);
            size_t new_index = hash % new_capacity;
            node->next = new_buckets[new_index];
            new_buckets[new_index] = node;
            node = next;
        }
    }
    // free buckets array
    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;
}

void hashmap_insert(HashMap* map, void* key, void* val) {
    unsigned long hash = map->hash_func(key);
    size_t index = hash % map->capacity; 

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

    // resize to keep faster lookup operatoin
    if ((float)map->size / (float)map->capacity > 0.75) {
        hashmap_resize(map, map->capacity * 2);
    }
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

char** hashmap_keys(HashMap* map) {
    char** keys = malloc((map->size + 1) * sizeof(char*));
    size_t index = 0;

    for (size_t i = 0; i < map->capacity; i++) {
        HashNode* node = map->buckets[i];
        while (node)
        {
            keys[index++] = my_strdup(node->key);
            node = node->next;
        }
        
    }

    keys[index] = NULL;
    return keys;
}

void hashmap_destroy(HashMap* map) {
    if (map == NULL) {
        return;
    }
    for (size_t i = 0; i < map->capacity; i++) {
        HashNode* node = map->buckets[i];
        while (node) {
            HashNode* next = node->next;
            if (map->free_key) {
                map->free_key(node->key);
                node->key = NULL;
            }
            if (map->free_val) {
                map->free_val(node->val);
                node->val = NULL;
            }
            free(node);
            node = next;
        }
    }
    free(map->buckets);
    map->buckets = NULL;
    free(map);
    map = NULL;
}