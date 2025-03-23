
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/errors.h"
#include "../headers/helper.h"
#include "../headers/define.h"
#include "../headers/pre_assemble.h"
#include "../headers/first_pass.h"
#include "../headers/label_data_struct.h"

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

        printf("Starting preprocessing\n");
        if (preprocess(file_name) == 0)
            continue;

        printf("Preprocessing finished\n");

        file_name = change_suffix(file_name, ".am");
        
        printf("Starting first pass on %s\n", file_name);
        if (first_pass(file_name) == 0)
            continue;
        printf("First pass finished\n");
        print_label_table();
    }
    return 0;
}