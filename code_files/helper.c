#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../headers/helper.h"
#include "../headers/define.h"

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


/**
 * @brief This function chaecks if the macro name is a name of a known word of assembly (operation/instruction/register).
 * 
 * @param name - the name of the macro.
 * @return int - returns 1 if the macro name is valid and 0 if it isn't.
 */
int cmp_mcro_name(char* name){
    int i;
    for (i = 0; i < 8; i++)
    {
        if(!strcmp(name, reg[i])){
            /*error*/
            return 0;
        }
    } 

    for (i = 0; i < 16; i++)
    {
        if(!strcmp(name, op_codes[i].name)){
            /*error*/
            return 0;
        }
    } 

    for (i = 0; i < 4; i++)
    {
        if(!strcmp(name, insturctions[i])){
            /*error*/
            return 0;
        }
    } 

    return 1;
}
/**
 * @brief This function checks if the mcro name starts with a letter and contains alphanumeric characters or _
    and that it is no longer than 31 chars.
 * 
 * @param name - the name of the macro.
 * @return int - returns 1 if the macro name is valid and 0 if it isn't.
 */
int mcro_letters(char* name){
    /*assums that name is not null*/
    int i = 0;
    if(!isalpha(name[i])){
        /*error*/
        return 0;
    }
    i++;
    while (name[i] != '\0')
    {
        if(i == MAX_MCRO_LENGTH){
            /*error*/
            return 0;
        }

        if(name[i] != '_' && !isalnum(name[i])){
            /*error*/
            return 0;
        }
        i++;
    }
    return 1;
}

/**
 * @brief This function removes extra white chars from s.
 * 
 * @param s - the string to remove extra white chars from.
 * @return void.
 */
void extra_spaces(char* s){
    char* temp_s;
    int i=0,j=0, space=1;
    while(s[i] != '\0'){
        if(s[i] == ' ' || s[i] == '\t'){
            if(s[i] == '\t'){
                s[i] = ' ';
            }
            if(space == 0){
                space=1;
                temp_s[j] = s[i];
                i++;
                j++;
            }
            else{
                i++;
            }
        }
        else{
            if(space == 1){
                space=0;
            }
            temp_s[j] = s[i];
            i++;
            j++;
        }
    }
    temp_s[++j] = '\0';
    strcpy(s,temp_s);
}


char* skip_whitespace(char* str) {
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}
