#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * @brief Skips whitespace characters in a string.
 * 
 * @param str - the string to skip whitespace in.
 * @return char* - the pointer to the first non-whitespace character.
 */
char* skip_whitespace(char* str);

#endif /* HELPER_H */