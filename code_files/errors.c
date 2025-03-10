#include "../headers/errors.h"
#include <stdio.h>
#include <stdlib.h>

Error error_list[] = {
    {Error_0, "Error 0: Not enough arguments in command\n"},
    {Error_1, "Error 1: Can't open file\n"},
    {Error_2, "Error 2: Unvalid macro declaration\n"},
    {Error_3, "Error 3: Unvalid macroend declaration\n"},
    {Error_4, "Error 4: Error: Memory allocation failed\n"},
    {Error_5, "Error 5: Reading text failed\n"},




    {Error_8, "Error 8: Label name is too long\n"},
    {Error_9, "Error 9: Invalid label name\n"},
    {Error_10, "Error 10: Missing .data directive\n"},
    {Error_11, "Error 11: Missing value in .data directive\n"},
    {Error_12, "Error 12: Invalid value in .data directive\n"},
    {Error_13, "Error 13: Malformed .string directive\n"},
    {Error_14, "Error 14: Syntax error in .string directive\n"},
    
};

void report_error(int line_num, int error_num){
    printf("line %u: %s", line_num, error_list[error_num].error_message);
}