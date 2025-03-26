
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/errors.h"
#include "../headers/helper.h"
#include "../headers/define.h"
#include "../headers/pre_assemble.h"
#include "../headers/first_pass.h"
#include "../headers/label_data_struct.h"
#include "../headers/macro_table.h"

/**
 * @brief The main function of the assembler program.
 * 
 * @param argc - the number of arguments.
 * @param argv - the arguments.
 * 
 */
int main(int argc, char *argv[])
{
    int i = 1;
    char *file_name;
    char *am_file_name;
    FILE* file;
    /* check if the number of arguments is correct */
    if (argc < TWO)
    {
        report_error(0, Error_0);/*Not enough arguments in command*/
        return 1;
    }
    /* preprocess the files */
    for (; i < argc; i++)
    {
        /* get the file name with a suffix if the arg is empty continue*/
        file_name = get_file_name(argv[i]);
        if (file_name == NULL)
            continue;
        

        file = look_for_file(file_name);
        if (file == NULL)
            continue;
        fclose(file);
        
        printf("Starting preprocessing - %s\n",file_name);
        if (preprocess(file_name) == 0){
            free_macros();
            continue;
        }

        printf("Preprocessing finished\n");
        
        am_file_name = change_suffix(file_name, ".am");
        
        printf("Starting first pass on %s\n", am_file_name);
        if (first_pass(am_file_name) == 0){
            free_all();
            continue;
        }

        printf("First pass finished\n");
        free_all();
        free(am_file_name);
        free(file_name); 
        
    }
    
    return 0;
}