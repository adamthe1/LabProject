#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../headers/helper.h"
#include "../headers/define.h"

char* reg[8] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

/* The format is {name_opcode, opcode_number, funct_number, operand_count, addressing_modes_src, addressing_modes_dest} */
Op_code op_codes[16] = {
    { "mov", 0, 0, 2, 
      ADDR_IMMEDIATE_BIT | ADDR_DIRECT_BIT | ADDR_REGISTER_BIT,  /* src: modes 0,1,3 */
      ADDR_DIRECT_BIT | ADDR_REGISTER_BIT                        /* dest: modes 1,3 */
    },
    { "cmp", 1, 0, 2, 
      ADDR_IMMEDIATE_BIT | ADDR_DIRECT_BIT | ADDR_REGISTER_BIT,  /* src: modes 0,1,3 */
      ADDR_IMMEDIATE_BIT | ADDR_DIRECT_BIT | ADDR_REGISTER_BIT   /* dest: modes 0,1,3 */
    },
    { "add", 2, 1, 2, 
      ADDR_IMMEDIATE_BIT | ADDR_DIRECT_BIT | ADDR_REGISTER_BIT,  /* src: modes 0,1,3 */
      ADDR_DIRECT_BIT | ADDR_REGISTER_BIT                        /* dest: modes 1,3 */
    },
    { "sub", 2, 2, 2, 
      ADDR_IMMEDIATE_BIT | ADDR_DIRECT_BIT | ADDR_REGISTER_BIT,  /* src: modes 0,1,3 */
      ADDR_DIRECT_BIT | ADDR_REGISTER_BIT                        /* dest: modes 1,3 */
    },
    { "lea", 4, 0, 2, ADDR_DIRECT_BIT, ADDR_DIRECT_BIT | ADDR_REGISTER_BIT }, /* src: 1, dest: 1,3 */
    { "clr", 5, 1, 1, ADDR_NONE_BIT, ADDR_DIRECT_BIT | ADDR_REGISTER_BIT },   /* no src, dest: 1,3 */
    { "not", 5, 2, 1, ADDR_NONE_BIT, ADDR_DIRECT_BIT | ADDR_REGISTER_BIT },   /* no src, dest: 1,3 */
    { "inc", 5, 3, 1, ADDR_NONE_BIT, ADDR_DIRECT_BIT | ADDR_REGISTER_BIT },   /* no src, dest: 1,3 */
    { "dec", 5, 4, 1, ADDR_NONE_BIT, ADDR_DIRECT_BIT | ADDR_REGISTER_BIT },   /* no src, dest: 1,3 */
    { "jmp", 9, 1, 1, ADDR_NONE_BIT, ADDR_DIRECT_BIT | ADDR_RELATIVE_BIT },   /* no src, dest: 1,2 */
    { "bne", 9, 2, 1, ADDR_NONE_BIT, ADDR_DIRECT_BIT | ADDR_RELATIVE_BIT },   /* no src, dest: 1,2 */
    { "jsr", 9, 3, 1, ADDR_NONE_BIT, ADDR_DIRECT_BIT | ADDR_RELATIVE_BIT },   /* no src, dest: 1,2 */
    { "red", 12, 0, 1, ADDR_NONE_BIT, ADDR_DIRECT_BIT | ADDR_REGISTER_BIT },  /* no src, dest: 1,3 */
    { "prn", 13, 0, 1, 
      ADDR_NONE_BIT,                                             /* no src */
      ADDR_IMMEDIATE_BIT | ADDR_DIRECT_BIT | ADDR_REGISTER_BIT   /* dest: 0,1,3 */
    },
    { "rts", 14, 0, 0, ADDR_NONE_BIT, ADDR_NONE_BIT },  /* no operands */
    { "stop", 15, 0, 0, ADDR_NONE_BIT, ADDR_NONE_BIT }  /* no operands */
};

char* insturctions[4] = {"data","string","entry","extern"};

Op_code* get_opcode(char* name){
    int i;
    for (i = 0; i < 16; i++)
    {
        if(!strcmp(name, op_codes[i].name)){
            return &op_codes[i];
        }
    }
    return NULL;
}

int get_reg(char* name){
    int i;
    for (i = 0; i < 8; i++)
    {
        if(!strcmp(name, reg[i])){
            return i;
        }
    }
    return -1;
}

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
        printf("error7");
        return 0;
    }
    i++;
    while (name[i] != '\0')
    {
        if(i == MAX_MCRO_LENGTH){
            /*error*/
            printf("error8\n");
            return 0;
        }

        if(name[i] != '_' && !isalnum(name[i])){
            /*error*/
            printf("error9 %s, %c",name, name[i]);
            return 0;
        }
        i++;
    }
    return 1;
}

/**
 * @brief This function converts a number to binary with 2s complement.
 * @brief The number of bits is given in DEFINE.h.
 * 
 * @param num - the decimal number to convert.
 * @param binary - the array to store the binary number.
 * @return int - returns 1 if the conversion was successful and 0 if it wasn't.
 */

int dec_to_binary(int num, int* binary) {
    int i;
    int is_negative = 0;
    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    /* Initialize binary array with zeros */
    for (i = 0; i < BITS_IN_DATA; i++) {
        binary[i] = 0;
    }
    
    /* Convert to binary */
    i = BITS_IN_DATA - 1;
    while (num > 0 && i >= 0) {
        binary[i--] = num % 2;
        num /= 2;
    }
    
    /* Check if the number is too large to be represented */
    if (num > 0) {
        return 0;  /* Conversion failed */
    }
    
    /* Apply 2's complement for negative numbers */
    if (is_negative) {
        /* Flip all bits */
        for (i = 0; i < BITS_IN_DATA; i++) {
            binary[i] = binary[i] ? 0 : 1;
        }
        
        /* Add 1 */
        for (i = BITS_IN_DATA - 1; i >= 0; i--) {
            if (binary[i] == 0) {
                binary[i] = 1;
                break;
            }
            binary[i] = 0;
        }
    }
    
    return 1;  /* Conversion successful */
}

char* copy_string(char* s){
    char* new_s = (char*)malloc(strlen(s) + 1);
    if(new_s == NULL){
        return NULL;
    }
    strcpy(new_s, s);
    return new_s;
}

/**
 * @brief This function removes extra white chars from s.
 * 
 * @param s - the string to remove extra white chars from.
 * @return void.
 */
void extra_spaces(char* s){
    char* temp_s = (char*)malloc(strlen(s) + 1);
    int i=0,j=0, space=1;
    if(temp_s == NULL){
        return;
    }
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

/* Helper function to trim whitespace from end of string */
void trim_whitespace(char* str) {
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len-1])) {
        str[--len] = '\0';
    }
}
