#include "../headers/errors.h"
#include <stdio.h>
#include <stdlib.h>

Error error_list[] = {
    {Error_0, "Error 0: Not enough arguments in command\n"},
    {Error_1, "Error 1: Can't open file\n"},
    {Error_2, "Error 2: Extra chars in macro declaration line\n"},
    {Error_3, "Error 3: Invalid macroend declaration\n"},
    {Error_4, "Error 4: Error: Memory allocation failed\n"},
    {Error_5, "Error 5: Reading text failed\n"},
    {Error_6, "Error 6: There is no macro name\n"},
    {Error_7, "Error 7: Label name already label or command\n"},
    {Error_8, "Error 8: Label name is too long\n"},
    {Error_9, "Error 9: Invalid label name\n"},
    {Error_10, "Error 10: Missing .data directive\n"},
    {Error_11, "Error 11: Missing value in .data directive\n"},
    {Error_12, "Error 12: Invalid value in .data directive\n"},
    {Error_13, "Error 13: Number out of range\n"},
    {Error_14, "Error 14: Syntax error in .string directive\n"},
    {Error_15, "Error 15: Extranous text after command\n"},
    {Error_16, "Error 16: Malformed .entry directive\n"},
    {Error_17, "Error 17: Missing .string directive\n"},
    {Error_18, "Error 18: Missing .extern directive\n"},
    {Error_19, "Error 19: Failed to create extern\n"},
    {Error_20, "Error 20: Failed to find operation\n"},
    {Error_21, "Error 21: Failed to create data label\n"},
    {Error_22, "Error 22: Failed to create string label\n"},
    {Error_23, "Error 23: Failed to create code label\n"},
    {Error_24, "Error 24: Unknown Instruction Type\n"},
    {Error_25, "Error 25: Empty string in .string\n"},
    {Error_26, "Error 26: Incorrect amount of operands\n"},
    {Error_27, "Error 27: Invalid addressing mode\n"},
    {Error_28, "Error 28: Failed to create binary code\n"},
    {Error_29, "Error 29: Syntax error in operand 1\n"},
    {Error_30, "Error 30: Syntax error in operand 2\n"},
    {Error_31, "Error 31: Invalid operand\n"},
    {Error_32, "Error 32: Line after Label cannot be empty\n"},
    {Error_33, "Error 33: Unvalid macro name\n"},
    {Error_34, "Error 34: Label name doesn't exist in the label table\n"},
    {Error_35, "Error 35: Line too long (over 81 char)\n"},
    {Error_36, "Error 36: External label in relative addressing mode\n"},
    {Error_37, "Error 37: No binary code with that IC\n"},
    {Error_38, "Error 38: unclosed macro definition\n"},
    {Error_39, "Error 39: Must have space after label\n"},
    {Error_40, "Error 40: Label cannot be name of macro\n"},
    {Error_41, "Error 41: The IC or DC is too big, there is only 2^21 adresses\n"}  
};

void report_error(int line_num, int error_num)
{
    printf("line %u: %s", line_num, error_list[error_num].error_message);
}