#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/label_data_struct.h"
#include "../headers/define.h"
#include "../headers/errors.h"
#include "../headers/helper.h"

static Label* head = NULL;
static Data_binary* data_head = NULL;
static Code_binary* code_head = NULL;
static Unknown_label *unknown_head = NULL;

int create_label(char* name, char type, int line_index){

    Label* new_label = (Label*)malloc(sizeof(Label));

    if(new_label == NULL){
        
        return 0;
    }

    new_label->name = copy_string(name);
    new_label->line_index = line_index;
    new_label->type = type;
    new_label->is_entry = 0;
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

Label* get_label_head(){
    if(head){
        return head;
    }
    return NULL;
}

int add_entry(char* label_name){
    Label* temp = get_label(label_name);
    if(!temp){
        return 0; /*error - not a labael name*/
    }
    temp->is_entry = 1;
    return 1;
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
        free(current->name);
        free(current);
        current = next;
    }
}

int create_binary_data(int binary, int DC_index){
    Data_binary* new_data = (Data_binary*)malloc(sizeof(Data_binary));

    if(new_data == NULL){
        return 0;
    }

    new_data->binary = binary;
    new_data->DC_index = DC_index;
    new_data->next = NULL;

    if(data_head == NULL){
        data_head = new_data;
    }else{
        Data_binary* current = data_head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_data;
    }
    return 1;
}



/**
 * @brief This function returns the data with the given line index.
 * 
 * @param line_index - the index of the line in the file.
 * @return Data_binary* - the data.
 */

Data_binary* get_binary_data(int DC_index){
    Data_binary* current = data_head;
    while(current != NULL){
        if(current->DC_index == DC_index){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int add_IC_to_DC(int IC_index){
    Data_binary* current = data_head;
    Label *label = head;
    while(current != NULL){
        current->DC_index += IC_index;
        current = current->next;
    }
    while(label != NULL){
        if (label->type == DATA_TYPE){
            label->line_index += IC_index;
        }
        label = label->next;
    }
    return 1;
}

/**
 * @brief This function frees the memory allocated for the data list.
 * 
 */

void free_binary_data(){
    Data_binary* current = data_head;
    Data_binary* next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}

int create_binary_code(int binary, int IC_index){
    Code_binary* new_code = (Code_binary*)malloc(sizeof(Code_binary));

    if(new_code == NULL){
        return 0;
    }

    new_code->binary = binary;
    new_code->IC_index = IC_index;
    new_code->next = NULL;

    if(code_head == NULL){
        code_head = new_code;
    }else{
        Code_binary* current = code_head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = new_code;
    }
    return 1;
}

Code_binary* get_binary_code(int IC_index){
    Code_binary* current = code_head;
    while(current != NULL){
        if(current->IC_index == IC_index){
            return (Code_binary*)current;
        }
        current = current->next;
    }
    return NULL;
}

void free_binary_code(){
    Code_binary* current = code_head;
    Code_binary* next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}

int create_unknown_label(char* name, int IC_index, int type, int line_number){
    Unknown_label* new_label = (Unknown_label*)malloc(sizeof(Unknown_label));

    if(new_label == NULL) {
        return 0;
    }

    new_label->name = copy_string(name);
    new_label->IC_index = IC_index;
    new_label->type = type;
    new_label->line_number = line_number;
    new_label->next = NULL;

    if(unknown_head == NULL) {
        unknown_head = new_label;
    } else {
        Unknown_label* current = unknown_head;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = new_label;
    }
    return 1;
}

Unknown_label* get_unknown_label(char* name) {
    Unknown_label* current = unknown_head;
    while(current != NULL) {
        if(strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free_unknown_labels() {
    Unknown_label* current = unknown_head;
    Unknown_label* next;
    while(current != NULL) {
        next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

Unknown_label* get_unknown_head(){
    if(unknown_head){
        return unknown_head;
    }
    return NULL;
}



void print_label_table(){
    Label* current = head;
    while(current != NULL){
        printf("Label: %s, Type: %d, Line: %d\n", current->name, current->type, current->line_index);
        current = current->next;
    }
}

void print_data_table(){
    Data_binary* current = data_head;
    while(current != NULL){
        printf("Data: %c  %d, DC: %d\n", current->binary, current->binary, current->DC_index);
        current = current->next;
    }
}

void print_code_table(){
    Code_binary* current = code_head;
    while(current != NULL){
        int i;
        /* print in binary parsed */
        printf("Binary code: ");
        for (i = 23; i >= 0; i--) {
            printf("%d", (current->binary >> i) & 1);
            if (i == 18 || i == 16 || i == 13 || i == 11 || i == 8 || i == 3)
            printf(", ");
        }
        printf("  ");
        
        printf("Code: %d, IC: %d\n", current->binary, current->IC_index);
        current = current->next;
    }
}

void print_unknown_table(){
    Unknown_label* current = unknown_head;
    while(current != NULL){
        printf("Unknown label: %s, Type: %d, IC: %d, Line: %d\n", current->name, current->type, current->IC_index, current->line_number);
        current = current->next;
    }
}