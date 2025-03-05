#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/label_table.h"

static Label* head = NULL;

int create_label(char* name, char type, int line_index){

    Label* new_label = (Label*)malloc(sizeof(Label));

    if(new_label == NULL){
        return 0;
    }

    new_label->name = name;
    new_label->line_index = line_index;
    new_label->type = type;
    new_label->next = NULL;

    if(head == NULL){
        head = new_label;
    }else{
        Label* current = head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_label;
    }
    return 1;
}


/**
 * @brief This function returns the label with the given name.
 * 
 * @param name - the name of the label.
 * @return label* - the label.
 */
Label* get_label(char* name){
    Label* current = head;
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

void free_labels(){
    Label* current = head;
    Label* next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}