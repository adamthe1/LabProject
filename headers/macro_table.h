/**
 * @file mcro_table.h
 * @brief This file contains the  of structure and functions for handling the macros.
 */

#ifndef _MCRO_TABLE_H
#define _MCRO_TABLE_H

/* Macros are nodes in a linked list*/
typedef struct Macro{
    char* name;
    char* code;
    int line_index;
    struct Macro* next;
} Macro;

/**
 * @brief This function creates a new macro node and adds it to the list.
 * 
 * @param name - the name of the macro.
 * @param line_index - the index of the line in the file.
 * @return int - 1 if the macro was created successfully, 0 otherwise.
 */
int create_macro(char* name, int line_index);

/**
 * @brief This function adds code lines to the last macro in the list.
 * 
 * @param code - the code line to add.
 * @return int - 1 if the code was added successfully, 0 otherwise.
 */
int add_code_to_macro(char* code);

/**
 * @brief This function returns the code of the macro with the given name.
 * 
 * @param name - the name of the macro.
 * @return char* - the code of the macro.
 */
char* get_macro_code(char* name);

/**
 * @brief This function returns the macro with the given name.
 * 
 * @param name - the name of the macro.
 * @return Macro* - the macro.
 */
Macro* get_macro(char* name);

/**
 * @brief This function frees the memory allocated for the macros list.
 * 
 */

void free_macros();

#endif