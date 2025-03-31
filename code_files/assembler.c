
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
    FILE *file;
    int preprocess_result;
    /* check if the number of arguments is correct */
    if (argc < TWO)
    {
        report_error(0, Error_0); /*Not enough arguments in command*/
        return 1;
    }
    /* preprocess the files */
    for (; i < argc; i++)
    {
        /* get the file name with a suffix if the arg is empty continue*/
        file_name = get_file_name(argv[i]);
        am_file_name = change_suffix(file_name, ".am");
        if (file_name == NULL)
            continue;

        file = look_for_file(file_name);
        if (file == NULL)
            continue;
            
        fclose(file);

        printf("Starting preprocessing - %s\n", file_name);
        preprocess_result = preprocess(file_name);
        if (preprocess_result == 0)
        {
            printf("Error found in preprocessing, stopping here\n");
            reset_macros();
            continue;
        }

        printf("Preprocessing finished\n");

        printf("Starting first pass on %s\n", am_file_name);
        if (first_pass(am_file_name, preprocess_result) == 0)
        {
            reset_all();
            continue;
        }

        printf("First pass finished\n");
        reset_all();
    }

    free_all();
    free(file_name);
    free(am_file_name);

    return 0;
}