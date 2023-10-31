//
// Created by mukit on 10/5/23.
//

#ifndef CSE511PROJECT_UTILS_H
#define CSE511PROJECT_UTILS_H

#include <string.h>
#include "hash_table.h"

int MAX_FUNCNAME_SIZE = 1000;
int GLOBAL_COUNTER_COUNT = 0;
int GLOBAL_SEMAPHORE_COUNT = 0;

enum Constants{
    WHITESPACE,
    FUNCTION_NAME,
    PATH,
    END,
    BRACE_START,
    BRACE_END,
    PAREN_START,
    PAREN_END,
    SEMICOLON,
    COMMA,
    BRACE_OPERATOR,
    PATH_EXPRESSION,
    P_SEMAPHORE,
    V_SEMAPHORE,
    PP_SEMAPHORE,
    VV_SEMAPHORE,
};

typedef struct path_expression_token path_expression_token;
typedef struct path_expression_token_list path_expression_token_list;
typedef struct path_expression_tree_node path_expression_tree_node;
typedef struct path_expression_treenode_stack_element path_expression_treenode_stack_element;
typedef struct path_expression_treenode_stack path_expression_treenode_stack;
typedef struct path_expression_w_semaphore_element path_expression_w_semaphore_element;
typedef struct path_expression_w_semaphore_list path_expression_w_semaphore_list;

struct path_expression_token{
    int expression_type;
    char *operator_expression;
    struct path_expression_token *next;
    struct path_expression_token *prev;
};

struct path_expression_token_list{
    struct path_expression_token* head;
    struct path_expression_token* top;
};

struct path_expression_tree_node{
    int expression_type;
    char *operator_expression;
    int is_leaf;
    struct path_expression_tree_node *left_child;
    struct path_expression_tree_node *right_child;
};

struct path_expression_treenode_stack_element{
    struct path_expression_tree_node* element;
    struct path_expression_treenode_stack_element* prev;
    struct path_expression_treenode_stack_element* next;
};

struct path_expression_treenode_stack{
    struct path_expression_treenode_stack_element* head;
    struct path_expression_treenode_stack_element* top;
};

struct path_expression_w_semaphore_element{
    int element_type; //PATH_EXP or SEMAPHORE
    struct path_expression_tree_node* treeNode;
    struct semaphore_element* sem_element;
    struct path_expression_w_semaphore_element* prev;
    struct path_expression_w_semaphore_element* next;
};

struct path_expression_w_semaphore_list{
    struct path_expression_w_semaphore_element* head;
};


int get_char_type(char c);
void set_path_expression_token(path_expression_token* tok,
                               int expression_type, char* op_exp);

char* get_function_name(const char* path_exp, int start_position);
char* get_operator_string(const char *path_exp, int start_position);


void append_to_path_exp_list(path_expression_token_list *list ,
                             path_expression_token * tok);
path_expression_token* pop_from_path_exp_list(path_expression_token_list *list);
path_expression_token* peek_top_from_path_exp_list(path_expression_token_list* list);
int isEmpty_path_exp_list(path_expression_token_list* list);
void print_pathexp_list(path_expression_token_list* list);


path_expression_tree_node* create_tree_node(int expression_type, char* op_exp);
void append_to_path_exp_treenode_stack(path_expression_treenode_stack *stack ,
                                       path_expression_tree_node * node);
path_expression_tree_node* pop_from_path_exp_treenode_stack(path_expression_treenode_stack *stack);


path_expression_token_list* convert_pathexp_string_to_list(const char* pathexp_string);
path_expression_token_list* convert_infix_to_postfix(path_expression_token_list* infix_list);
path_expression_tree_node* build_pathexp_tree(path_expression_token_list* postfix_list);

void print_semaphore_element(semaphore_element* element);
void print_pathexp_w_semaphore_element(path_expression_w_semaphore_element* element);
path_expression_w_semaphore_element*
create_pe_w_sem_element_from_treenode(path_expression_tree_node* treenode);
path_expression_w_semaphore_element*
create_pe_w_sem_element_semaphore(semaphore_element* sem_element);
path_expression_w_semaphore_element* copy_pe_w_sem_element(
        path_expression_w_semaphore_element* original_copy
);


void connect_two_pe_w_sem_element(path_expression_w_semaphore_element* predecessor,
                                  path_expression_w_semaphore_element* successor);

void append_to_pe_w_sem_list(path_expression_w_semaphore_list* list,
                             path_expression_w_semaphore_element* element);

void modify_pe_w_sem_list(path_expression_w_semaphore_list* list,
                          path_expression_w_semaphore_element* predecessor_element,
                          path_expression_w_semaphore_element* replacement_sublist_head,
                           path_expression_w_semaphore_element* replacement_sublist_tail,
                           path_expression_w_semaphore_element* successor_element);
void print_pe_w_sem_list(path_expression_w_semaphore_list* list);


char* obtain_new_sem_or_counter(int sem_or_counter, int initial_value, hash_table* ht);
semaphore_element* create_semaphore_element(int semaphore_type,
                                                   char* counter_name, char* sem_arg1_name, char* sem_arg2_name);
void build_pathexp_recursion(path_expression_w_semaphore_list* curr_pathexp_w_semaphore_list,
                             path_expression_w_semaphore_element* curr_element_to_expand,
                             hash_table* ht);
path_expression_w_semaphore_list*
build_path_exp_w_semaphore_list(path_expression_tree_node* pathexp_tree_root,
                                hash_table *ht);
void populate_data_structure_hashtable(
        hash_table* ds_hash_table,
        path_expression_w_semaphore_list* pathexp_w_semaphore_list
);

#endif //CSE511PROJECT_UTILS_H
