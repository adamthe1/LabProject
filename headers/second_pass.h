#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H

#include <stdio.h>
#include "label_data_struct.h"

/**
 * @brief Performs the second pass on the assembly file.
 *
 * @param file - The assembly source file
 * @param *IC - The address of the instruction counter.
 * @param *DC - The address of the data counter.
 * @param error_found_first - 1 if thers is error in the first pass, 0 otherwise.
 * @return int - 1 if successful, 0 if errors were found.
 */
int second_pass(char *file_name, int *IC, int *DC, int error_found_first);

/**
 * @brief Processes the unknown labels in the unknown label table
 *
 * @param external_file - The name of the external file.
 * @return int - returns 1 if the process has finished successfuly and 0 if it hasn't.
 */
int process_unknown(char *external_file);

#endif /* _SECOND_PASS_H */