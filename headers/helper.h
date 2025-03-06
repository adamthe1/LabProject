#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum BOOL{
    FALSE = 0,
    TRUE = 1
} BOOL;


/**
 * @brief Extracts the filename from a given file path.
 * 
 * @param file_name - the name of the file without suffix
 * @return char* - the extracted filename.
 */
char* get_file_name(char* file_name);

/**
 * @brief Looks for a file with the given name.
 * 
 * @param file_name_suffix - the name of the file.with a suffix
 * @return FILE - the file if found, NULL otherwise.
 */
FILE* look_for_file(char* file_name_suffix);


/**
 * @brief Adds a suffix to a file name.
 * 
 * @param file_name - the name of the file.
 * @param suffix - the suffix to add.
 * @return char* - the new file name.
 */
char* add_suffix(char* file_name, char* suffix);

/**
 * @brief Changes the suffix of a file name.
 * 
 * @param file_name - the name of the file.
 * @param suffix - the new suffix.
 * @return char* - the new file name.
 */
char* change_suffix(char* file_name, char* suffix);


/**
 * @brief This function chaecks if the macro name is a name of a known word of assembly (operation/instruction/register).
 * 
 * @param name - the name of the macro.
 * @return int - returns 1 if the macro name is valid and 0 if it isn't.
 */
int cmp_mcro_name(char* name);

/**
 * @brief This function checks if the mcro name starts with a letter and contains alphanumeric characters or _
    and that it is no longer than 31 chars.
 * 
 * @param name - the name of the macro.
 * @return int - returns 1 if the macro name is valid and 0 if it isn't.
 */
int mcro_letters(char* name);

/**
 * @brief This function removes extra white chars from s.
 * 
 * @param s - the string to remove extra white chars from.
 * @return void.
 */
void extra_spaces(char* s);

#endif /* HELPER_H */