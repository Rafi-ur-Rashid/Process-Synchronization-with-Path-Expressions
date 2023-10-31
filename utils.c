//
// Created by mukit on 9/29/23.
//
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include "hash_table.c"



int get_char_type(char c){
    switch (c) {
        case '{':
            return BRACE_START;
        case '}':
            return BRACE_END;
        case '(':
            return PAREN_START;
        case ')':
            return PAREN_END;
        case ',':
            return COMMA;
        case '+':
            return COMMA;
        case ';':
            return SEMICOLON;
        case ' ':
            return WHITESPACE;
        default:
            return FUNCTION_NAME;
    }
}

void set_path_expression_token(path_expression_token* tok,
        int expression_type, char* op_exp) {
    tok->expression_type = expression_type;
    tok->operator_expression = (char*)malloc(strlen(op_exp) + 1);
    //printf("Type is %d\n", tok->expression_type);
    strcpy(tok->operator_expression, op_exp);
}

char* get_function_name(const char* path_exp, int start_position){
    char *func_name = (char*)malloc(MAX_FUNCNAME_SIZE* sizeof(char) + 1);
    int curr_pos = start_position;
    while (curr_pos < strlen(path_exp) && get_char_type(path_exp[curr_pos]) == FUNCTION_NAME){
        func_name[curr_pos - start_position] = path_exp[curr_pos];
        curr_pos += 1;
    }
    func_name[curr_pos] = '\0';
    return func_name;
}

char* get_operator_string(const char *path_exp, int start_position){
    char *func_name = (char*)malloc(2* sizeof(char) + 1);
    switch (get_char_type(path_exp[start_position])) {
        case BRACE_START:
            func_name[0] = '{';
            func_name[1] = '\0';
            return func_name;
        case BRACE_END:
            func_name[0] = '}';
            func_name[1] = '\0';
            return func_name;
        case PAREN_START:
            func_name[0] = '(';
            func_name[1] = '\0';
            return func_name;
        case PAREN_END:
            func_name[0] = ')';
            func_name[1] = '\0';
            return func_name;
        case COMMA:
            func_name[0] = ',';
            func_name[1] = '\0';
            return func_name;
        case SEMICOLON:
            func_name[0] = ';';
            func_name[1] = '\0';
            return func_name;
    }
    func_name[0] = ';';
    func_name[1] = '\0';
    return func_name;
}


void append_to_path_exp_list(path_expression_token_list *list ,
        path_expression_token * tok){
    path_expression_token * new_tok =
            (path_expression_token *) malloc(sizeof(path_expression_token));
    set_path_expression_token(new_tok, tok->expression_type, tok->operator_expression);

    if(list->head == NULL){
        list->head = new_tok;
        list->top = new_tok;

    } else{
        path_expression_token* curr_ptr = list->head;
        while (curr_ptr->next != NULL) {
            curr_ptr = curr_ptr->next;
        }
        curr_ptr->next = new_tok;
        new_tok->prev = curr_ptr;

        new_tok->next = NULL;
        list->top = new_tok;
    }
}

path_expression_token* pop_from_path_exp_list(path_expression_token_list *list){
    if(list->top != NULL){
        if(list->top->prev != NULL){
            path_expression_token *old_top = list->top;
            list->top = list->top->prev;
            list->top->next = NULL;

            old_top->prev = NULL;
            old_top->next = NULL;

            return old_top;
        } else{
            path_expression_token *old_top = list->top;
            list->top = NULL;
            list->head = NULL;

            old_top->prev = NULL;
            old_top->next = NULL;

            return old_top;
        }
    }
    return NULL;
}

path_expression_token* peek_top_from_path_exp_list(path_expression_token_list* list){
    return list->top;
}

int isEmpty_path_exp_list(path_expression_token_list* list){
    if(list->head == NULL || list->top == NULL){
        return 1;
    }
    return 0;
}

void print_pathexp_list(path_expression_token_list* list){
    path_expression_token* curr_tok = list->head;
    while (curr_tok !=NULL){
        printf( "%s ", curr_tok->operator_expression);
        curr_tok = curr_tok->next;
    }
    printf("\n");
}


path_expression_tree_node* create_tree_node(int expression_type, char* op_exp){
    path_expression_tree_node* new_leaf =
            (path_expression_tree_node*)malloc(sizeof(path_expression_tree_node));
    new_leaf->expression_type = expression_type;
    new_leaf->operator_expression = (char*)malloc(strlen(op_exp) + 1);
    strcpy(new_leaf->operator_expression, op_exp);
    if(expression_type == FUNCTION_NAME){
        new_leaf->is_leaf = 1;
    }else{
        new_leaf->is_leaf = 0;
    }
    return new_leaf;
}


void append_to_path_exp_treenode_stack(path_expression_treenode_stack *stack ,
                             path_expression_tree_node * node){

    path_expression_treenode_stack_element* new_element =
            (path_expression_treenode_stack_element*)malloc(sizeof(path_expression_treenode_stack_element));
    new_element->element = node;

    if(stack->head == NULL){
        stack->head = new_element;
        stack->top = new_element;

    } else{
        path_expression_treenode_stack_element* curr_ptr = stack->head;
        while (curr_ptr->next != NULL) {
            curr_ptr = curr_ptr->next;
        }
        curr_ptr->next = new_element;
        new_element->prev = curr_ptr;

        curr_ptr->next = NULL;
        stack->top = new_element;
    }
}

path_expression_tree_node* pop_from_path_exp_treenode_stack(path_expression_treenode_stack *stack){
    if(stack->top != NULL){
        if(stack->top->prev != NULL){
            path_expression_treenode_stack_element* old_top = stack->top;
            stack->top = stack->top->prev;
            stack->top->next = NULL;

            old_top->prev = NULL;
            old_top->next = NULL;

            return old_top->element;
        } else{
            path_expression_treenode_stack_element* old_top = stack->top;
            stack->top = NULL;
            stack->head = NULL;

            old_top->prev = NULL;
            old_top->next = NULL;

            return old_top->element;
        }
    }
    return NULL;
}




path_expression_token_list* convert_pathexp_string_to_list(const char* pathexp_string){
    //printf("%s\n", pathexp_string);
    path_expression_token_list* pathexp_list = (path_expression_token_list*)
            malloc(sizeof(path_expression_token_list));
    int  curr_char = 0;

    while(curr_char < strlen(pathexp_string)){
        //printf("%d\n", curr_char);
        if(get_char_type(pathexp_string[curr_char]) == FUNCTION_NAME){
            path_expression_token*new_tok =
                    (path_expression_token*) malloc(sizeof(path_expression_token));
            char* function_name = get_function_name(pathexp_string, curr_char);
            if(strcmp(function_name, "path") == 0){
                set_path_expression_token(new_tok, PATH, function_name);
            } else if (strcmp(function_name, "end") == 0){
                set_path_expression_token(new_tok, END, function_name);
            }else{
                set_path_expression_token(new_tok, FUNCTION_NAME, function_name);
            }
            append_to_path_exp_list(pathexp_list, new_tok);
            curr_char += (int) strlen(function_name);
        } else if(get_char_type(pathexp_string[curr_char]) != WHITESPACE){
            path_expression_token *new_tok =
                    (path_expression_token*) malloc(sizeof(path_expression_token));
            char* op_string = get_operator_string(pathexp_string, curr_char);
            set_path_expression_token(new_tok, get_char_type(pathexp_string[curr_char]),
                                      op_string);
            append_to_path_exp_list(pathexp_list, new_tok);
            curr_char += (int) strlen(op_string);
        } else {
            curr_char += 1;
        }
    }
    //print_pathexp_list(pathexp_list);
    return pathexp_list;
}

path_expression_token_list* convert_infix_to_postfix(path_expression_token_list* infix_list){
    //print_pathexp_list(infix_list);
    path_expression_token_list* postfix_list =
            (path_expression_token_list*) malloc(sizeof(path_expression_token_list));

    path_expression_token_list* operator_stack =
            (path_expression_token_list*) malloc(sizeof(path_expression_token_list));

    path_expression_token* curr_token_in_infix = infix_list->head;
    path_expression_token* popped_operator, *peeked_operator, *brace_tok;

    while (curr_token_in_infix != NULL){
        if(curr_token_in_infix->expression_type == FUNCTION_NAME){
            //printf("Placing FUNCTION_NAME %s\n", curr_token_in_infix->operator_expression);
            append_to_path_exp_list(postfix_list, curr_token_in_infix);
        }
        else if(curr_token_in_infix-> expression_type == PAREN_START){
            append_to_path_exp_list(operator_stack, curr_token_in_infix);
        }
        else if(curr_token_in_infix-> expression_type == PAREN_END){
            popped_operator = NULL;
            do{
                popped_operator = pop_from_path_exp_list(operator_stack);
                if(popped_operator != NULL && popped_operator->expression_type != PAREN_START){
                    //printf("Placing PAREN_END %s\n", popped_operator->operator_expression);
                    append_to_path_exp_list(postfix_list, popped_operator);
                }
            } while (isEmpty_path_exp_list(operator_stack) == 0 &&
            !(popped_operator != NULL && popped_operator->expression_type == PAREN_START));



        }
        else if(curr_token_in_infix->expression_type == BRACE_START){
            append_to_path_exp_list(operator_stack, curr_token_in_infix);
        }
        else if(curr_token_in_infix->expression_type == BRACE_END){
            popped_operator = NULL;
            do{

                popped_operator = pop_from_path_exp_list(operator_stack);
                if(popped_operator != NULL && popped_operator->expression_type != BRACE_START){
                    //printf("Placing BRACE_START %s\n", popped_operator->operator_expression);
                    append_to_path_exp_list(postfix_list, popped_operator);
                }
            } while (isEmpty_path_exp_list(operator_stack) == 0 && !(popped_operator != NULL &&
            popped_operator->expression_type == BRACE_START));

            brace_tok = (path_expression_token*) malloc(sizeof (path_expression_token));
            set_path_expression_token(brace_tok, BRACE_OPERATOR, "{}");
            //printf("Placing BRACE_OPERATOR %s\n", brace_tok->operator_expression);
            append_to_path_exp_list(postfix_list, brace_tok);
        }
        else if(curr_token_in_infix->expression_type == COMMA
        || curr_token_in_infix-> expression_type == SEMICOLON){
            peeked_operator = NULL;

            do{
                peeked_operator = peek_top_from_path_exp_list(operator_stack);
                if(peeked_operator != NULL &&
                peeked_operator->expression_type >= curr_token_in_infix->expression_type){
                    popped_operator = pop_from_path_exp_list(operator_stack);
                    //printf("Placing CS %s\n", popped_operator->operator_expression);
                    append_to_path_exp_list(postfix_list, popped_operator);
                } else{
                    break;
                }
            } while (isEmpty_path_exp_list(operator_stack) == 0);
            append_to_path_exp_list(operator_stack, curr_token_in_infix);
        }
        curr_token_in_infix = curr_token_in_infix->next;
    }

    while (!(isEmpty_path_exp_list(operator_stack))){
        popped_operator = pop_from_path_exp_list(operator_stack);
        if(popped_operator != NULL){
            //printf("Placing LAST %s\n", popped_operator->operator_expression);
            append_to_path_exp_list(postfix_list, popped_operator);
        }
    }

    //print_pathexp_list(postfix_list);
    return postfix_list;
}

path_expression_tree_node* build_pathexp_tree(path_expression_token_list* postfix_list){
    path_expression_tree_node* curr_postfix_element_treenode, *popped_operand1_treenode, *popped_operand2_treenode;
    path_expression_treenode_stack* stack =
            (path_expression_treenode_stack*)malloc(sizeof(path_expression_treenode_stack));


    path_expression_token* curr_postfix_element = postfix_list->head;
    while(curr_postfix_element != NULL){
        curr_postfix_element_treenode =
                create_tree_node(curr_postfix_element->expression_type, curr_postfix_element->operator_expression);
        if(curr_postfix_element_treenode->expression_type == FUNCTION_NAME){
            append_to_path_exp_treenode_stack(stack, curr_postfix_element_treenode);
        } else if(curr_postfix_element_treenode->expression_type == BRACE_OPERATOR){
            popped_operand1_treenode = pop_from_path_exp_treenode_stack(stack);
            curr_postfix_element_treenode->is_leaf = 0;
            curr_postfix_element_treenode->left_child = popped_operand1_treenode;
            append_to_path_exp_treenode_stack(stack, curr_postfix_element_treenode);
        } else if(curr_postfix_element_treenode->expression_type == COMMA
        ||curr_postfix_element_treenode->expression_type == SEMICOLON){
            popped_operand1_treenode = pop_from_path_exp_treenode_stack(stack);
            popped_operand2_treenode = pop_from_path_exp_treenode_stack(stack);

            curr_postfix_element_treenode->is_leaf = 0;
            curr_postfix_element_treenode->right_child = popped_operand1_treenode;
            curr_postfix_element_treenode->left_child = popped_operand2_treenode;
            append_to_path_exp_treenode_stack(stack, curr_postfix_element_treenode);
        }
        curr_postfix_element = curr_postfix_element->next;
    }

    return pop_from_path_exp_treenode_stack(stack);
}



void print_semaphore_element(semaphore_element* element){
    if(element->semaphore_type == P_SEMAPHORE){
        printf("P(%s) ", element->semaphore_arg1_name);
    }else if(element->semaphore_type == V_SEMAPHORE){
        printf("V(%s) ", element->semaphore_arg1_name);
    }else if(element->semaphore_type == PP_SEMAPHORE){
        printf("PP(%s,%s,%s) ", element->counter_name,
               element->semaphore_arg1_name,
               element->semaphore_arg2_name);
    }else if(element->semaphore_type == VV_SEMAPHORE){
        printf("VV(%s,%s,%s) ", element->counter_name,
               element->semaphore_arg1_name,
               element->semaphore_arg2_name);
    }
}

void print_pathexp_w_semaphore_element(path_expression_w_semaphore_element* element){
    if(element->element_type >= P_SEMAPHORE
    && element->element_type <= VV_SEMAPHORE){
        print_semaphore_element(element->sem_element);
    }else{
        printf(" <%s> ", element->treeNode->operator_expression);
    }
}

path_expression_w_semaphore_element*
create_pe_w_sem_element_from_treenode(path_expression_tree_node* treenode){
    path_expression_w_semaphore_element* new_element =
            (path_expression_w_semaphore_element*)
                    malloc(sizeof(path_expression_w_semaphore_element));
    new_element->element_type = PATH_EXPRESSION;
    new_element->treeNode = treenode;
    return new_element;
}

path_expression_w_semaphore_element*
create_pe_w_sem_element_semaphore(semaphore_element* sem_element){
    path_expression_w_semaphore_element* new_element =
            (path_expression_w_semaphore_element*)
                    malloc(sizeof(path_expression_w_semaphore_element));
    new_element->element_type = sem_element->semaphore_type;
    new_element->sem_element = sem_element;
    return new_element;
}

path_expression_w_semaphore_element* copy_pe_w_sem_element(
        path_expression_w_semaphore_element* original_copy
        ){
    path_expression_w_semaphore_element* new_element =
            (path_expression_w_semaphore_element*)
                    malloc(sizeof(path_expression_w_semaphore_element));
    new_element->element_type = original_copy->element_type;
    new_element->treeNode = original_copy->treeNode;
    new_element->sem_element = original_copy->sem_element;
    return new_element;
}

void connect_two_pe_w_sem_element(path_expression_w_semaphore_element* predecessor,
        path_expression_w_semaphore_element* successor){
    predecessor->next = successor;
    successor->prev = predecessor;
}


void append_to_pe_w_sem_list(path_expression_w_semaphore_list* list,
        path_expression_w_semaphore_element* element){
    if(list->head == NULL){
        list->head = element;

    } else{
        path_expression_w_semaphore_element* curr_ptr = list->head;
        while (curr_ptr->next != NULL) {
            curr_ptr = curr_ptr->next;
        }
        curr_ptr->next = element;
        element->prev = curr_ptr;

        element->next = NULL;
    }
}

void modify_pe_w_sem_list(path_expression_w_semaphore_list* list,
                          path_expression_w_semaphore_element* predecessor_element,
                          path_expression_w_semaphore_element* replacement_sublist_head,
                                   path_expression_w_semaphore_element* replacement_sublist_tail,
                                           path_expression_w_semaphore_element* successor_element){
    if(predecessor_element != NULL){
        predecessor_element->next = replacement_sublist_head;
    }else{
        list->head = replacement_sublist_head;
    }

    replacement_sublist_head->prev = predecessor_element;

    replacement_sublist_tail->next = successor_element;
    if(successor_element != NULL){
        successor_element->prev = replacement_sublist_tail;
    }
}

void print_pe_w_sem_list(path_expression_w_semaphore_list* list){
    path_expression_w_semaphore_element* curr_tok = list->head;
    while (curr_tok !=NULL){
        print_pathexp_w_semaphore_element(curr_tok);
        curr_tok = curr_tok->next;
    }
    printf("\n");
}


char* obtain_new_sem_or_counter(int sem_or_counter, int initial_value, hash_table* ht){
    if(sem_or_counter == 0){
        sem_t *new_semaphore = (sem_t*)malloc(sizeof(sem_t));
        sem_init(new_semaphore, 0, initial_value);

        hash_table_node_data* new_element = (hash_table_node_data*) malloc(sizeof(hash_table_node_data));
        new_element->semaphore = new_semaphore;

        GLOBAL_SEMAPHORE_COUNT += 1;
        char* sem_name = get_char_from_int(0, GLOBAL_SEMAPHORE_COUNT);
        hash_table_insert(ht, sem_name, new_element);

        return sem_name;

    } else{
        int* new_counter = (int*)malloc(sizeof(int));
        *new_counter = initial_value;

        hash_table_node_data* new_element = (hash_table_node_data*) malloc(sizeof(hash_table_node_data));
        new_element->counter = new_counter;

        GLOBAL_COUNTER_COUNT += 1;
        char* counter_name = get_char_from_int(1, GLOBAL_COUNTER_COUNT);
        hash_table_insert(ht, counter_name, new_element);

        return counter_name;
    }
}

semaphore_element* create_semaphore_element(int semaphore_type,
        char* counter_name, char* sem_arg1_name, char* sem_arg2_name){
    semaphore_element* new_sem_element =
            (semaphore_element*) malloc(sizeof(semaphore_element));
    new_sem_element->semaphore_type = semaphore_type;
    new_sem_element->counter_name = counter_name;
    new_sem_element->semaphore_arg1_name = sem_arg1_name;
    new_sem_element->semaphore_arg2_name = sem_arg2_name;

    return new_sem_element;
}



void build_pathexp_recursion(path_expression_w_semaphore_list* curr_pathexp_w_semaphore_list,
                path_expression_w_semaphore_element* curr_element_to_expand,
                hash_table* ht){
    //print_pe_w_sem_list(curr_pathexp_w_semaphore_list);
    path_expression_w_semaphore_element *pred, *succ,
            *elem1, *elem2, *elem3, *elem4, *elem5, *elem6;
    char* new_sem, *new_counter, *s_i, *s_j;

    pred = curr_element_to_expand->prev;
    succ = curr_element_to_expand->next;

    if(curr_element_to_expand->element_type == PATH_EXPRESSION
    && curr_element_to_expand->treeNode != NULL){
        if(curr_element_to_expand->treeNode->expression_type == SEMICOLON){
            elem1 = create_pe_w_sem_element_from_treenode(curr_element_to_expand->treeNode->left_child);
            new_sem = obtain_new_sem_or_counter(0, 0, ht);
            elem2 = create_pe_w_sem_element_semaphore(
                    create_semaphore_element(V_SEMAPHORE, NULL, new_sem, NULL)
                    );
            connect_two_pe_w_sem_element(elem1, elem2);
            elem3 = create_pe_w_sem_element_semaphore(
                    create_semaphore_element(P_SEMAPHORE, NULL, new_sem, NULL)
            );
            connect_two_pe_w_sem_element(elem2, elem3);
            elem4 = create_pe_w_sem_element_from_treenode(curr_element_to_expand->treeNode->right_child);
            connect_two_pe_w_sem_element(elem3, elem4);

            modify_pe_w_sem_list(curr_pathexp_w_semaphore_list, pred,
                                 elem1, elem4, succ);

            build_pathexp_recursion(curr_pathexp_w_semaphore_list,
                                    elem1, ht);
            build_pathexp_recursion(curr_pathexp_w_semaphore_list,
                                    elem4, ht);
        }
        else if(curr_element_to_expand->treeNode->expression_type == COMMA){
            elem1 = copy_pe_w_sem_element(pred);
            elem2 = create_pe_w_sem_element_from_treenode(curr_element_to_expand->treeNode->left_child);
            elem3 = copy_pe_w_sem_element(succ);
            elem4 = copy_pe_w_sem_element(pred);
            elem5 = create_pe_w_sem_element_from_treenode(curr_element_to_expand->treeNode->right_child);
            elem6 = copy_pe_w_sem_element(succ);

            connect_two_pe_w_sem_element(elem1, elem2);
            connect_two_pe_w_sem_element(elem2, elem3);
            connect_two_pe_w_sem_element(elem3, elem4);
            connect_two_pe_w_sem_element(elem4, elem5);
            connect_two_pe_w_sem_element(elem5, elem6);

            modify_pe_w_sem_list(curr_pathexp_w_semaphore_list,
                                 pred, elem1, elem6, succ);


            build_pathexp_recursion(curr_pathexp_w_semaphore_list,
                                    elem2, ht);
            build_pathexp_recursion(curr_pathexp_w_semaphore_list,
                                    elem5, ht);
        }
        else if(curr_element_to_expand->treeNode->expression_type == BRACE_OPERATOR){

            new_counter = obtain_new_sem_or_counter(1, 0, ht);
            new_sem = obtain_new_sem_or_counter(0, 1, ht);

            s_i = pred->sem_element->semaphore_arg1_name;
            s_j = succ->sem_element->semaphore_arg1_name;

            elem1 = create_pe_w_sem_element_semaphore(
                    create_semaphore_element(
                            PP_SEMAPHORE,
                            new_counter,
                            new_sem,
                            s_i
                            )
                    );

            elem2 = create_pe_w_sem_element_from_treenode(curr_element_to_expand->treeNode->left_child);
            elem3 = create_pe_w_sem_element_semaphore(
                    create_semaphore_element(
                            VV_SEMAPHORE,
                            new_counter,
                            new_sem,
                            s_j
                    )
            );

            connect_two_pe_w_sem_element(elem1, elem2);
            connect_two_pe_w_sem_element(elem2, elem3);

            modify_pe_w_sem_list(curr_pathexp_w_semaphore_list,
                                 pred,
                                 elem1,
                                 elem3, succ);

            build_pathexp_recursion(curr_pathexp_w_semaphore_list,
                                    elem2, ht);

        }

    }

}

path_expression_w_semaphore_list*
        build_path_exp_w_semaphore_list(path_expression_tree_node* pathexp_tree_root,
                hash_table *ht){
    path_expression_w_semaphore_element* new_pe_w_sem_elem, *pathexp_elem;
    char* new_sem;
    path_expression_w_semaphore_list* pathexp_w_semaphore_list
            = (path_expression_w_semaphore_list*)malloc(sizeof(path_expression_w_semaphore_list));


    if(pathexp_tree_root->expression_type == FUNCTION_NAME
    && (strcmp(pathexp_tree_root->operator_expression, "ReqWestCrossing") == 0 ||
    strcmp(pathexp_tree_root->operator_expression, "ReqEastCrossing") == 0)){
        new_sem = obtain_new_sem_or_counter(0, 5, ht);
    } else{
        new_sem = obtain_new_sem_or_counter(0, 1, ht);
    }

    //char* new_sem = obtain_new_sem_or_counter(0, 1, ht);
    semaphore_element* new_sem_elem = create_semaphore_element
            (P_SEMAPHORE, NULL, new_sem, NULL);
    new_pe_w_sem_elem =
            create_pe_w_sem_element_semaphore(new_sem_elem);
    append_to_pe_w_sem_list(pathexp_w_semaphore_list, new_pe_w_sem_elem);

    pathexp_elem =
            create_pe_w_sem_element_from_treenode(pathexp_tree_root);
    append_to_pe_w_sem_list(pathexp_w_semaphore_list, pathexp_elem);

    new_sem_elem = create_semaphore_element
            (V_SEMAPHORE, NULL, new_sem, NULL);
    new_pe_w_sem_elem =
            create_pe_w_sem_element_semaphore(new_sem_elem);
    append_to_pe_w_sem_list(pathexp_w_semaphore_list, new_pe_w_sem_elem);

    build_pathexp_recursion(pathexp_w_semaphore_list,
                            pathexp_elem,
                            ht);

    return pathexp_w_semaphore_list;
}

void populate_data_structure_hashtable(
        hash_table* ds_hash_table,
        path_expression_w_semaphore_list* pathexp_w_semaphore_list
        ){
    semaphore_element* entry_sem , *exit_sem;
    data_structure_for_function_operations_element* new_ds_elem;
    hash_table_node_data* new_element;

    path_expression_w_semaphore_element* curr_elem = pathexp_w_semaphore_list->head;
    while (curr_elem != NULL){
        if(curr_elem->element_type == PATH_EXPRESSION){
            if(curr_elem->treeNode->expression_type == FUNCTION_NAME){
                entry_sem = curr_elem->prev->sem_element;
                exit_sem = curr_elem->next->sem_element;

//                printf("Function  %s\n", curr_elem->treeNode->operator_expression);
//                printf("Entry ");
//                print_semaphore_element(entry_sem);
//                printf("\n");
//                printf("Exit ");
//                print_semaphore_element(exit_sem);
//                printf("\n\n");

                new_ds_elem
                        = (data_structure_for_function_operations_element*)
                                malloc(sizeof(data_structure_for_function_operations_element));

                new_ds_elem->entry_sem_element = entry_sem;
                new_ds_elem->exit_sem_element = exit_sem;

                new_element =
                        (hash_table_node_data*) malloc(sizeof(hash_table_node_data));

                new_element->data_element = new_ds_elem;
                hash_table_insert(ds_hash_table,
                                  curr_elem->treeNode->operator_expression,
                                  new_element);
            }
        }
        curr_elem = curr_elem->next;
    }

}