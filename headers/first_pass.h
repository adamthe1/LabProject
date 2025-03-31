#ifndef _FIRST_PASS_H
#define _FIRST_PASS_H

#include <stdio.h>
#include "label_data_struct.h"

/**
 * @brief Performs the first pass on the assembly file
 *
 * @param file - The assembly source file
 * @param preprocess_result - Result of the preprocessing step (1 if successful, 2 if there is a line that is too long)
 * @return int - 1 if successful, 0 if errors were found
 */
int first_pass(char *file_name, int preprocess_result);

/**
 * @brief Parses a line and determines if it has a label
 *
 * @param line - The line to parse
 * @param label_name - Buffer to store label name if found
 * @return int - 1 if label found, 0 otherwise
 */
int parse_label(char *line, char *label_name);

/**
 * @brief Identifies the type of instruction/directive in the line
 *
 * @param line - The line to analyze
 * @return int - The instruction type code
 */
int identify_instruction(char *line);

/**
 * @brief Processes a data directive (.data)
 *
 * @param line - The directive line
 * @param DC - Current data counter (updated by function)
 * @return int - 1 if successful, 0 if error
 */
int process_data_directive(char *line, int *DC);

/**
 * @brief Processes a string directive (.string)
 *
 * @param line - The directive line
 * @param DC - Current data counter (updated by function)
 * @return int - 1 if successful, 0 if error
 */
int process_string_directive(char *line, int *DC);

/**
 * @brief Processes an entry directive (.entry)
 *
 * @param line - The directive line
 * @return int - 1 if successful, 0 if error
 */
int process_entry_directive(char *line);

/**
 * @brief Processes an extern directive (.extern)
 *
 * @param line - The directive line
 * @return int - 1 if successful, 0 if error
 */
int process_extern_directive(char *line);

/**
 * @brief Processes an assembly operation (MOV, CMP, ADD, etc.)
 *
 * @param line - The operation line
 * @param IC - Current instruction counter (updated by function)
 * @return int - 1 if successful, 0 if error
 */
int process_operation(char *line, int *IC);

/**
 * @brief Encodes an operand into binary form
 *
 * @param operand - The operand to encode
 * @param addr_mode - The addressing mode of the operand
 * @param IC - Current instruction counter (updated by function)
 * @return int - 1 if successful, 0 if error
 */
int encode_operand(char *operand, int addr_mode, int *IC, int command_IC);

/**
 * @brief Determines the addressing mode and register number of an operand
 *
 * @param operand - The operand to analyze
 * @param addr_mode - Buffer to store the addressing mode
 * @param reg_num - Buffer to store the register number
 * @return int - 1 if successful, 0 if error
 */

int determine_addressing_mode(char *operand, int *addr_mode, int *reg_num);

#endif /* _FIRST_PASS_H */