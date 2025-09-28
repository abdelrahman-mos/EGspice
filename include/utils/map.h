#ifndef MAP_H
#define MAP_H
#include <stdio.h>

typedef struct HashNode
{
    void* key;
    void* val;
    struct HashNode* next;
} HashNode;

typedef struct
{
    HashNode** buckets;
    size_t capacity; // number of buckets
    size_t size; // number of elements
    unsigned long (*hash_func)(const void* key);
    int (*cmp_func)(const void* a, const void* b);
    void (*free_key)(void* key);
    void (*free_val)(void* val);
} HashMap;

unsigned long hash_string(const void* str);
HashMap* hashmap_create(
    size_t capacity,
    unsigned long (*hash_func)(const void *),
    int (*cmp_func)(const void*, const void*),
    void (*free_key)(void*),
    void (*free_val)(void*)
);

void hashmap_insert(HashMap* map, void* key, void* val);
void* hashmap_get(HashMap* map, void* key);
//void hashmap_remove(HashMap* map, void* key);
void hashmap_destroy(HashMap* map);
void hashmap_resize(HashMap* map, size_t new_capacity);


#endif