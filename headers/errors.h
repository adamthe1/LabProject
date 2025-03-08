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
} ERROR_NUMBERS;

typedef struct Error{
    int error_num;
    char* error_message;
} Error;

extern Error error_list[];

void report_error(int line_num, int error_num);

#endif /* ERRORS_H */