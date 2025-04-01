/**
 * @file mcro_table.h
 * @brief This file contains the  of structure and functions for handling the macros.
 */

#ifndef _MCRO_TABLE_H
#define _MCRO_TABLE_H

/* Macros are nodes in a linked list*/
/**
 * @brief struct of macro and its information
 * 
 * @param name The macro name
 * @param code The macro code
 * @param line_index The line index of the macro
 * @param next The next macro in the list
 */
typedef struct Macro
{
    char *name;
    char *code;
    int line_index;
    struct Macro *next;
} Macro;

/**
 * @brief Creates a new macro node and adds it to the list.
 *
 * @param name - The name of the macro.
 * @param line_index - The index of the line in the file.
 * @return int - 1 if the macro was created successfully, 0 otherwise.
 */
int create_macro(char *name, int line_index);

/**
 * @brief Adds code lines to the last macro in the list.
 *
 * @param code - The code line to add.
 * @return int - 1 if the code was added successfully, 0 otherwise.
 */
int add_code_to_macro(char *code);

/**
 * @brief This function returns the code of the macro with the given name.
 *
 * @param name - The name of the macro.
 * @return char* - The code of the macro.
 */
char *get_macro_code(char *name);

/**
 * @brief This function returns the macro with the given name.
 *
 * @param name - The name of the macro.
 * @return Macro* - The macro.
 */
Macro *get_macro(char *name);

/**
 * @brief Frees the memory allocated for the macros list.
 *
 */
void free_macros();

/**
 * @brief Resets the macro table.
 * 
 */
void reset_macros();

/**
 * @brief Prints all macros in the list.
 *
 */
void print_macros();

#endif