#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum BOOL{
    FALSE = 0,
    TRUE = 1
} BOOL;

/* 
Function that recieves the file name and adds the as suffix
*/
/**
 * @brief Extracts the filename from a given file path.
 * 
 * @param file_path - the path to the file.
 * @return char* - the extracted filename.
 */
/**
 * @brief Extracts the filename from a given file path
 * 
 * @param file_path A string containing the full path to a file
 * @return char* The extracted filename (caller is responsible for memory)
 */
char* get_file_name(char* file_path);

/* Opens the file name with read, if it opens returns the file else NULL*/
FILE look_for_file(char* file_name);


/* 
Function that adds a suffix to a file name
*/
char* add_suffix(char* file_name, char* suffix);

char* change_suffix(char* file_name, char* suffix);
#endif /* HELPER_H */