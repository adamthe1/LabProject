/**
 * This file is the part where we preprocess the files before assembling them.
 * We need to add all macros to the macros linked list and replace them in the code.
 */

#ifndef _PRE_ASSEMBLE_H
#define _PRE_ASSEMBLE_H

/**
 * @brief This function is the pre_assembler.
 *
 * @param file_name - the name of the file to check.
 * @return int - returns 1 if the pre_assembler has finished successfuly,
 *  2 if there was a too long line and 0 if there was at least 1 error.
 */
int preprocess(char *file_name);

/**
 * @brief Chaecks the validation of the macro name.
 *
 * @param mcro_name - The name of the macro.
 * @return int - Returns 1 if the macro name is valid and 0 if it isn't.
 */
int valid_macro_dec(char *mcro_name);

#endif
