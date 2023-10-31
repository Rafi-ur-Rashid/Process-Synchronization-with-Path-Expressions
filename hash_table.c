#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Custom implementation of strncpy()
char* strncpy(char* dest, const char* src, size_t n) {
    char* ret = dest;
    while (n && (*dest++ = *src++))
        n--;
    if (n)
        while (--n)
            *dest++ = 0;
    return ret;
}

void PEstringToArray(const char* input, char* output[MAX_STRINGS]) {
    const char* start;
    const char* end;
    int len, index = 0;

    start = input;
    while (start && (start = strstr(start, "path"))) {
        end = strstr(start, " end")+1;
        if (!end) break;

        len = end - start + 3; // 3 characters for "end"
        output[index] = (char *)malloc(len + 1);
        strncpy(output[index], start, len);
        output[index][len] = '\0'; // Null terminate the string

        index++;
        start = end + 3; // Move past this "end"
    }
}

char* get_char_from_int(int sem_or_counter, int int_number){
    int i, curr_digit, total_digits;
    char rev_number[1000], *front_number;
    total_digits = 0;
    while (int_number > 0){
        curr_digit = int_number % 10;
        rev_number[total_digits] = (char)(curr_digit + '0');
        int_number /= 10;
        total_digits += 1;
    }
    front_number = (char *) malloc((total_digits + 2)*sizeof(char));
    if(sem_or_counter == 0){
        front_number[0] = 's';
    } else {
        front_number[0] = 'c';
    }

    for(i = 0; i < total_digits; i++){
        front_number[total_digits - 1 - i + 1] = rev_number[i];
    }
    front_number[total_digits + 1] = '\0';
    return front_number;
}

// Simple hash function
unsigned int hash(const char *key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    value = value % TABLE_SIZE;

    return value;
}

// Initialize hash table
hash_table *create_table() {
    hash_table *newTable;
    if ((newTable = malloc(sizeof(hash_table))) == NULL) return NULL;
    for (int i = 0; i < TABLE_SIZE; i++) newTable->table[i] = NULL;
    return newTable;
}

// Insert key-value pair into hash table
int hash_table_insert(hash_table *hash_table, const char *key, hash_table_node_data *value) {
    unsigned int slot = hash(key);
    hash_table_node *newhash_table_node = malloc(sizeof(hash_table_node));
    if (!newhash_table_node) return -1;

    newhash_table_node->key = strdup(key);
    newhash_table_node->value = value;
    newhash_table_node->next = hash_table->table[slot];
    hash_table->table[slot] = newhash_table_node;

    return 0;
}

// Retrieve a key-value pair from a hash table.
hash_table_node_data *hash_table_retrieve(hash_table *hash_table, const char *key) {
    unsigned int slot = hash(key);
    hash_table_node *current = hash_table->table[slot];

    while (current != NULL && strcmp(current->key, key) != 0) {
        current = current->next;
    }

    if (current == NULL) return NULL;
    return current->value;
}

// Free the hash table
void free_table(hash_table *hash_table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table_node *list = hash_table->table[i];
        while (list) {
            hash_table_node *temp = list;
            list = list->next;
            free(temp->key);
            free(temp);
        }
    }
    free(hash_table);
}