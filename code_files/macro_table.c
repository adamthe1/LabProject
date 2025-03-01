#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/macro_table.h"

static Macro* head = NULL;

int create_macro(char* name, int line_index){

    Macro* new_macro = (Macro*)malloc(sizeof(Macro));

    if(new_macro == NULL){
        return 0;
    }

    new_macro->name = name;
    new_macro->line_index = line_index;
    new_macro->code = NULL;
    new_macro->next = NULL;

    if(head == NULL){
        head = new_macro;
    }else{
        Macro* current = head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_macro;
    }
    return 1;
}

/**
 * @brief This function adds code lines to the last macro in the list.
 * 
 * @param code - the code line to add.
 * @return int - 1 if the code was added successfully, 0 otherwise.
 */
int add_code_to_macro(char* code){
    if(head == NULL){
        return 0;
    }
    Macro* current = head;
    while(current->next != NULL){
        current = current->next;
    }
    current->code = code;
    return 1;
}

/**
 * @brief This function returns the code of the macro with the given name.
 * 
 * @param name - the name of the macro.
 * @return char* - the code of the macro.
 */
char* get_macro_code(char* name){
    Macro* current = get_macro(name);
    if(current != NULL){
        return current->code;
    }
    return NULL;
}

/**
 * @brief This function returns the macro with the given name.
 * 
 * @param name - the name of the macro.
 * @return Macro* - the macro.
 */
Macro* get_macro(char* name){
    Macro* current = head;
    while(current != NULL){
        if(strcmp(current->name, name) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * @brief This function frees the memory allocated for the head list.
 * 
 */

void free_macros(){
    Macro* current = head;
    Macro* next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}