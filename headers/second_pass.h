#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H

#include <stdio.h>
#include "label_data_struct.h"

int second_pass(char* file_name, int* IC, int* DC);

/**
 * @brief This function processes the unknown labels in the unknown label table
 * 
 * @param external_file - the name of the external file.
 * @return int - returns 1 if the process has finished successfuly and 0 if it hasn't.
 */
int process_unknown(char* external_file);

#endif /* _SECOND_PASS_H */