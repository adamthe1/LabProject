#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>

typedef enum ERROR_NUMBERS {
    Error_0 = 0, Error_1, Error_2, Error_3,
    Error_4, Error_5, Error_6,
    Error_7, Error_8, Error_9,
    Error_10, Error_11, Error_12,
    Error_13, Error_14, Error_15,
    Error_16, Error_17, Error_18,
    Error_19, Error_20, Error_21,
    Error_22, Error_23, Error_24,
    Error_25, Error_26, Error_27,
    Error_28, Error_29, Error_30,
    Error_31, Error_32, Error_33,
    Error_34, Error_35, Error_36,
    Error_37, Error_38, Error_39,
    Error_40, Error_41, Error_42,
    Error_43, Error_44, Error_45,
    Error_46, Error_47, Error_48,
    Error_49, Error_50
} ERROR_NUMBERS;

/**
 * @brief struct of error
 * 
 * @param error_num The error number
 * @param error_message The error message to print
 */
typedef struct Error{
    int error_num;
    char* error_message;
} Error;

extern Error error_list[];

/**
 * @brief Report the error - prints to stdout
 * Prints the error and the line number.
 * @param line_num The error line number
 * @param line_num The error number
 */
void report_error(int line_num, int error_num);

#endif /* ERRORS_H */