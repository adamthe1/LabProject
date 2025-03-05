#ifndef _FIRST_PASS_H
#define _FIRST_PASS_H

#include <stdio.h>
#include "label_table.h"

/* Error codes for first pass */
#define ERR_NONE 0
#define ERR_SYNTAX 1
#define ERR_LABEL 2
#define ERR_DIRECTIVE 3
#define ERR_OPERAND 4
#define ERR_FILE 5

/* Maximum values */
#define MAX_LABEL_LEN 31
#define MAX_LINE_LENGTH 80

/* Instruction types */
#define INST_TYPE_OPERATION 1
#define INST_TYPE_DATA 2
#define INST_TYPE_STRING 3
#define INST_TYPE_ENTRY 4
#define INST_TYPE_EXTERN 5

/**
 * @brief Performs the first pass on the assembly file
 * 
 * @param file - The assembly source file
 * @return int - 1 if successful, 0 if errors were found
 */
int first_pass(FILE* file);

/**
 * @brief Parses a line and determines if it has a label
 * 
 * @param line - The line to parse
 * @param label_name - Buffer to store label name if found
 * @return int - 1 if label found, 0 otherwise
 */
int parse_label(char* line, char* label_name);

/**
 * @brief Identifies the type of instruction/directive in the line
 * 
 * @param line - The line to analyze
 * @return int - The instruction type code
 */
int identify_instruction(char* line);

/**
 * @brief Processes a data directive (.data)
 * 
 * @param line - The directive line
 * @param DC - Current data counter (updated by function)
 * @return int - 1 if successful, 0 if error
 */
int process_data_directive(char* line, int* DC);

/**
 * @brief Processes a string directive (.string)
 * 
 * @param line - The directive line
 * @param DC - Current data counter (updated by function)
 * @return int - 1 if successful, 0 if error
 */
int process_string_directive(char* line, int* DC);

/**
 * @brief Processes an entry directive (.entry)
 * 
 * @param line - The directive line
 * @return int - 1 if successful, 0 if error
 */
int process_entry_directive(char* line);

/**
 * @brief Processes an extern directive (.extern)
 * 
 * @param line - The directive line
 * @return int - 1 if successful, 0 if error
 */
int process_extern_directive(char* line);

/**
 * @brief Processes an assembly operation (MOV, CMP, ADD, etc.)
 * 
 * @param line - The operation line
 * @param IC - Current instruction counter (updated by function)
 * @return int - 1 if successful, 0 if error
 */
int process_operation(char* line, int* IC);

/**
 * @brief Skips whitespace in a string
 * 
 * @param str - The string to process
 * @return char* - Pointer to first non-whitespace character
 */
char* skip_whitespace(char* str);

/**
 * @brief Reports an error found during the first pass
 * 
 * @param line_num - Current line number
 * @param err_type - Error type code
 * @param message - Error message
 */
void report_error(int line_num, int err_type, char* message);

#endif /* _FIRST_PASS_H */