/**
 * this file is the part where we preprocess the files before assembling them.
 * we need to add all macros to the macros linked list and replace them in the code.
 */

#ifndef _PRE_ASSEMBLE_H
#define _PRE_ASSEMBLE_H


/**
 * main function of the file, opens the file and calls the macros functions.
 */
int preprocess(char* file_name);

int save_macros(char* file_name);


#endif 

