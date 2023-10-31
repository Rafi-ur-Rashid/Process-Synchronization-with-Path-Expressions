//
// Created by mukit on 9/30/23.
//

#ifndef CSE511PROJECT_HASH_TABLE_H
#define CSE511PROJECT_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define TABLE_SIZE 1000
#define MAX_STRINGS 10 // Assuming a maximum of 10 such substrings for simplicity


typedef struct hash_table hash_table;
typedef struct hash_table_node hash_table_node;
typedef struct hash_table_node_data hash_table_node_data;
typedef struct semaphore_element semaphore_element;
typedef struct data_structure_for_function_operations_element data_structure_for_function_operations_element;
struct semaphore_element{
    int semaphore_type;
    char* counter_name;
    char* semaphore_arg1_name;
    char* semaphore_arg2_name;
};

struct data_structure_for_function_operations_element{
    struct semaphore_element* entry_sem_element;
    struct semaphore_element* exit_sem_element;
};

struct hash_table_node_data{
    int* counter;
    sem_t* semaphore;
    data_structure_for_function_operations_element* data_element;
};


struct hash_table_node {
    char *key;
    hash_table_node_data *value;
    struct hash_table_node *next;
};

struct hash_table {
    struct hash_table_node *table[TABLE_SIZE];
};



char* strncpy(char* dest, const char* src, size_t n);
void PEstringToArray(const char* input, char* output[MAX_STRINGS]);
char* get_char_from_int(int sem_or_counter, int int_number);

unsigned int hash(const char *key);
hash_table *create_table();
int hash_table_insert(hash_table *hash_table, const char *key, hash_table_node_data *value);
hash_table_node_data *hash_table_retrieve(hash_table *hash_table, const char *key);
void free_table(hash_table *hashtable);


#endif //CSE511PROJECT_HASH_TABLE_H

