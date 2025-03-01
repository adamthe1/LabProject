/* This file is to create a hash map for easier allocation of macro and functions */


#ifndef HASH_MAP_H
#define HASH_MAP_H

/* Includes */
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "helper.h" /* Added include for helper.h */

/* Define constants */
#define HASH_MAP_INITIAL_CAPACITY 16
#define HASH_MAP_LOAD_FACTOR 0.75

/* Forward declarations */
typedef struct hash_map_t hash_map_t;
typedef struct hash_entry_t hash_entry_t;

/* Hash function type */
typedef unsigned int (*hash_func_t)(const void* key);

/* Key comparison function type */
typedef int (*key_equals_func_t)(const void* a, const void* b);

/* Hash map entry structure */
struct hash_entry_t {
    void* key;
    void* value;
    hash_entry_t* next;
};

/* Hash map structure */
struct hash_map_t {
    hash_entry_t** buckets;
    unsigned int capacity;
    unsigned int size;
    hash_func_t hash_func;
    key_equals_func_t key_equals;
};

/* Function to create a new hash map */
hash_map_t* hash_map_create(hash_func_t hash_func, key_equals_func_t key_equals);

/* Function to free a hash map and its entries */
void hash_map_free(hash_map_t* map, BOOL free_keys, BOOL free_values);

/* Function to get a value from the hash map by key */
void* hash_map_get(const hash_map_t* map, const void* key);

/* Function to put a key-value pair into the hash map */
int hash_map_put(hash_map_t* map, void* key, void* value);

/* Function to remove a key-value pair from the hash map */
int hash_map_remove(hash_map_t* map, const void* key, BOOL free_key, BOOL free_value);

/* Function to check if a key exists in the hash map */
int hash_map_contains(const hash_map_t* map, const void* key);

/* Function to get the number of entries in the hash map */
unsigned int hash_map_size(const hash_map_t* map);

/* Function to check if the hash map is empty */
int hash_map_is_empty(const hash_map_t* map);

/* Function to clear all entries from the hash map */
void hash_map_clear(hash_map_t* map, BOOL free_keys, BOOL free_values);

/* Some common hash functions */
unsigned int hash_string(const void* key);
unsigned int hash_int(const void* key);
unsigned int hash_pointer(const void* key);

/* Some common key comparison functions */
int string_equals(const void* a, const void* b);
int int_equals(const void* a, const void* b);
int pointer_equals(const void* a, const void* b);

#endif /* HASH_MAP_H */