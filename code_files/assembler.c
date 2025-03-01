
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/errors.h"
#include "../headers/helper.h"
#include "../headers/define.h"
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
        print_error(Error_0);
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

        if (preprocess(file_name) == 0)
            continue;
    }
}