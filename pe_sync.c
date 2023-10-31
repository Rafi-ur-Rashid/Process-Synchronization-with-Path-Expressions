/* Toy synchronizer: Sample template
 * Your synchronizer should implement the three functions listed below. 
 */
#include "pe_sync.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#include "utils.c"

hash_table *func_op_ds_ht, *sem_counter_ht;

int sem_pp(int* c1, sem_t *s3, sem_t* s_i){
    sem_wait(s3);
    *c1 = *c1 + 1;
    if(*c1 == 1){
        sem_wait(s_i);
    }
    sem_post(s3);
    return 0;
}

int sem_vv(int* c1, sem_t *s3, sem_t* s_j){
    sem_wait(s3);
    *c1 = *c1 - 1;
    if(*c1 == 0){
        sem_post(s_j);
    }
    sem_post(s3);
    return 0;
}

//take the semaphore element, retrieve its arguments from the hastable and execute relevant function
void execute_semaphore(semaphore_element *element,
                       hash_table *sem_counter_ht_){
    hash_table_node_data *ht_element_sem_arg1, *ht_element_sem_arg2, *ht_element_counter;
    if(element->semaphore_type == P_SEMAPHORE){
        ht_element_sem_arg1 = hash_table_retrieve(sem_counter_ht_, element->semaphore_arg1_name);
        sem_wait(ht_element_sem_arg1->semaphore);
    }else if(element->semaphore_type == V_SEMAPHORE){
        ht_element_sem_arg1 = hash_table_retrieve(sem_counter_ht_, element->semaphore_arg1_name);
        sem_post(ht_element_sem_arg1->semaphore);
    }else if(element->semaphore_type == PP_SEMAPHORE){
        ht_element_counter = hash_table_retrieve(sem_counter_ht_, element->counter_name);
        ht_element_sem_arg1 = hash_table_retrieve(sem_counter_ht_, element->semaphore_arg1_name);
        ht_element_sem_arg2 = hash_table_retrieve(sem_counter_ht_, element->semaphore_arg2_name);

        sem_pp(ht_element_counter->counter,
               ht_element_sem_arg1->semaphore,
               ht_element_sem_arg2->semaphore);

    } else if(element->semaphore_type == VV_SEMAPHORE){
        ht_element_counter = hash_table_retrieve(sem_counter_ht_, element->counter_name);
        ht_element_sem_arg1 = hash_table_retrieve(sem_counter_ht_, element->semaphore_arg1_name);
        ht_element_sem_arg2 = hash_table_retrieve(sem_counter_ht_, element->semaphore_arg2_name);

        sem_vv(ht_element_counter->counter,
               ht_element_sem_arg1->semaphore,
               ht_element_sem_arg2->semaphore);
    }
}


void ENTER_OPERATION(const char *op_name)
{


//    if(strcmp(op_name, "CaregiverArrive") == 0){
//        printf("Here 6\n");
//    }

    hash_table_node_data *val = hash_table_retrieve(func_op_ds_ht, op_name);
    semaphore_element *sem_element = val->data_element->entry_sem_element;
    execute_semaphore(sem_element, sem_counter_ht);

    if(strcmp(op_name, "EastCrossing") == 0){
        ENTER_OPERATION("ReqEastCrossing");
    }

    if(strcmp(op_name, "WestCrossing") == 0){
        ENTER_OPERATION("ReqWestCrossing");
    }

//    if(strcmp(op_name, "CaregiverArrive") == 0){
//        printf("Here 7\n");
//    }

}

void EXIT_OPERATION(const char *op_name)
{


    if(strcmp(op_name, "EastCrossing") == 0){
        EXIT_OPERATION("ReqEastCrossing");
    }
    if(strcmp(op_name, "WestCrossing") == 0){
        EXIT_OPERATION("ReqWestCrossing");
    }

    hash_table_node_data *val = hash_table_retrieve(func_op_ds_ht, op_name);
    semaphore_element *sem_element = val->data_element->exit_sem_element;
    execute_semaphore(sem_element, sem_counter_ht);

}

void INIT_SYNCHRONIZER(const char *path_exp)
{


    int i;
    struct path_expression_tree_node* pathexp_tree;
    func_op_ds_ht = (hash_table*) malloc(sizeof(hash_table));
    sem_counter_ht = (hash_table*) malloc(sizeof(hash_table));

    char* processedStrings[MAX_STRINGS] = {0};
    PEstringToArray(path_exp, processedStrings);    //for multiple path expression

    for(i = 0; i < MAX_STRINGS && processedStrings[i] != NULL; i++){
        // path expression string--> path expression list --> default list to postfix list-->postfix list to PE tree
        pathexp_tree = build_pathexp_tree(
                convert_infix_to_postfix
                        (convert_pathexp_string_to_list
                                 (processedStrings[i])));

        populate_data_structure_hashtable(func_op_ds_ht,

                                          build_path_exp_w_semaphore_list
                                                  (pathexp_tree, sem_counter_ht));

    }

    printf("Initializing Synchronizer with path_exp %s\n", path_exp);
}
