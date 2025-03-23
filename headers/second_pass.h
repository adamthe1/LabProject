#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H

#include <stdio.h>
#include "label_data_struct.h"

int second_pass(char* file_name, int* IC, int* DC);

/**
 * @brief Processes an assembly operation (MOV, CMP, ADD, etc.)
 * 
 * @param line - The operation line
 * @param IC - Current instruction counter (updated by function)
 * @return int - 1 if successful, 0 if error
 */
int process_operation2(char* line, int* IC);

/**
 * @brief Encodes an operand into binary form
 * 
 * @param operand - The operand to encode
 * @param addr_mode - The addressing mode of the operand
 * @param IC - Current instruction counter (updated by function)
 * @return int - 1 if successful, 0 if error
 */
int encode_operand2(char* operand, int addr_mode);

#endif /* _SECOND_PASS_H */