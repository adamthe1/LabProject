#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../headers/helper.h"

char* get_file_name(char* file_name){
    char* name = (char*)malloc(strlen(file_name) + 1);
    if(name == NULL){
        return NULL;
    }
    name = add_suffix(file_name, ".as");
    return name;
}

FILE* look_for_file(char* file_name_suffix){
    FILE* file = fopen(file_name_suffix, "r");
    return file;
}

char* add_suffix(char* file_name, char* suffix){
    char* new_file_name = (char*)malloc(strlen(file_name) + strlen(suffix) + 1);
    if(new_file_name == NULL){
        return NULL;
    }
    strcpy(new_file_name, file_name);
    strcat(new_file_name, suffix);
    return new_file_name;
}

char* change_suffix(char* file_name, char* suffix){
    char* new_file_name = (char*)malloc(strlen(file_name) + strlen(suffix) + 1);
    char* dot;
    if(new_file_name == NULL){
        return NULL;
    }
    strcpy(new_file_name, file_name);
    /* look for last dot in the file_name*/
    dot = strrchr(new_file_name, '.');
    if(dot == NULL){
        return NULL;
    }
    strcpy(dot, suffix);
    return new_file_name;
}

char* skip_whitespace(char* str) {
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}