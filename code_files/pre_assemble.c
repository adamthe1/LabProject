#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/macro_table.h"
#include "../headers/pre_assemble.h"
#include "../headers/helper.h"
#include "../headers/define.h"
#include "../headers/errors.h"

/*
algorithem:
1. read the next line of the file, if the file is over go to 9
2. if the next field is a mcro name replace it with the mcro content and go back to step 1
otherwise' continue.
3. if the next field is "mcro" continue to 4, otherwise skip to 6.
4. turn on flag mcro
5. insert to the mcro table the name of the mcro
6. read the next line of the file, if the file is over go to 9.
otherwise, if the mcro flag is on add the line to the mcro table and delete it from the file.
otherwise, (not a mcro) return to 1.
7. if the next field is mcroend delete the label from the file and continue.
otherwise, return to 6.
8. turn off the mcro flag. return to 1 (finished to save the mcro)
9. finish: save the am file.
*/


static int error_found = 0;

int preprocess(char *file_name)
{
    /*fp is the as file pointer, fp2 is the am file pointer*/
    FILE *fp = NULL, *fp2 = NULL;
    char *temp_code, *code;
    char first_thing[MAX_MCRO_LENGTH];
    char mcro_name[MAX_MCRO_LENGTH];
    char line[MAX_LINE_LENGTH + 3];
    char *am_file_name;
    char *pos;
    int i = 0;
    int linecount = 0;
    size_t line_len = 0;
    size_t temp_code_size = 0; /* Current allocated size for temp_code */
    size_t temp_code_used = 0; /* Currently used bytes in temp_code */
    size_t new_size = 0;
    char *new_buffer;
    int too_long_flag = 0; /* Flag to indicate if the line is too long */
    int has_line_too_long = 0; /* Flag to indicate if thefile has a line that is too long */
    int mcro_flag = 0;/* Flag to flag that current line is a macro code */


    error_found = 0;

    am_file_name = change_suffix(file_name, ".am");
    fp = fopen(file_name, "r");
    if (!fp)
    {
        report_error(linecount, Error_1); /*can't open file*/
        return 0;
    }
    fp2 = fopen(am_file_name, "w"); /*add .am to the file name*/
    if (!fp2)
    {
        report_error(linecount, Error_1); /*can't open file*/
        fclose(fp);
        return 0;
    }

    /* Initial allocation for temp_code - will grow as needed */
    temp_code_size = MAX_LINE_LENGTH * 2;
    temp_code = (char *)malloc(temp_code_size);
    if (!temp_code)
    {
        report_error(linecount, Error_4); /*Memory allocation failed*/
        fclose(fp);
        fclose(fp2);
        return 0;
    }
    temp_code[0] = '\0';

    while (fgets(line, sizeof(line), fp)) /*The loop ends when the file ends*/
    {
        if (too_long_flag)
        {
            if (strchr(line, '\n') != NULL)
            {
                too_long_flag = 0;
            }
            continue;
        }
        linecount++;
        /* Remove newline character if present */

        /* If the line len is larger than 80 without \n error */
        if (strlen(line) > 81)
        {
            report_error(linecount, Error_35);
            line[80] = '\n'; 
            line[81] = '\0'; /* Add null terminator */
            too_long_flag = 1;
            has_line_too_long = 1; /* Set the flag to indicate a line is too long */
        }
        

        pos = skip_whitespace(line);
        /* Skip empty lines */
        if (pos[0] == COMMENT_CHAR)
        {
            continue; /* Skip comment lines */
        }

        if (pos[0] == '\0' || pos[0] == '\n')
        {
            if (!mcro_flag)
            {
                fprintf(fp2, "%s", line);
            }
            continue;
        }

        i = 0;
        /*Save the first non white chars in first_thing*/
        while (pos[0] != '\0' && !isspace((unsigned char)pos[0]) && i < MAX_MCRO_LENGTH - 1 && pos[0] != '\n' && pos[0] != ':')
        {
            first_thing[i++] = *pos++;
        }
        first_thing[i] = '\0';

        
        if (get_macro(first_thing))
        { /* If temp is a macro name which was declared before*/
            if (pos[0] == ':')
            {
                report_error(linecount, Error_40);
                error_found = 1;
                continue;
            }
            code = get_macro_code(first_thing);
            fprintf(fp2, "%s", code); /* Prints the macro code instead of its name */
            continue;
        }

        pos = skip_whitespace(line);

        if (strncmp(pos, "mcro ", 5) == 0)
        { /*A start of macro declaration*/

            pos += 5;/*Skips the "mcro "*/
            pos = skip_whitespace(pos);

            i = 0;
            while (pos[0] != '\0' && !isspace((unsigned char)pos[0]) && i < MAX_MCRO_LENGTH - 1 && pos[0] != '\n')
            {
                /*Save the macro name in mcro_name*/
                mcro_name[i++] = *pos++;
            }
            mcro_name[i] = '\0';

            pos = skip_whitespace(pos);
            if (*pos != '\n' && *pos != '\0')
            {
                report_error(linecount, Error_2); /*Extra chars in macro declaration line*/
                error_found = 1;
            }

            if (strlen(mcro_name) == 0)
            {
                report_error(linecount, Error_6); /*There is no macro name*/
                error_found = 1;
            }

            if (!valid_macro_dec(mcro_name))
            {
                report_error(linecount, Error_33); /*Unvalid macro name*/
                error_found = 1;
                continue;
            }

            mcro_flag = 1;                      /*macro declaration*/
            create_macro(mcro_name, linecount); /*create a macro with the name temp*/
            temp_code[0] = '\0';
            temp_code_used = 0;
            continue;
        }

        if (mcro_flag)
        {

            if ((pos = strstr(line, "mcroend")))
            { /*A start of macro declaration*/
                
                
                if (strcmp(first_thing, "mcroend"))
                {
                    report_error(linecount, Error_3); /* Invalid macroend declaration*/
                    error_found = 1;
                }

                pos += 7;/*Skips "mcroend"*/
                pos = skip_whitespace(pos);

                if (*pos != '\n' && *pos != '\0')
                {
                    report_error(linecount, Error_3); /* Invalid macroend declaration*/
                    error_found = 1;
                }
                

                add_code_to_macro(temp_code); /*Adds the whole macro code to the macro*/
                temp_code[0] = '\0';          /*Initilazes the temp_code*/
                mcro_flag = 0;                /*Macro declaration has ended*/
                continue;
            }
            line_len = strlen(line);
            if (temp_code_used + line_len + 1 > temp_code_size)
            {
                /* Double the buffer size */
                new_size = temp_code_size * 2;
                new_buffer = (char *)realloc(temp_code, new_size);

                if (!new_buffer)
                {
                    report_error(linecount, Error_4); /*Memory allocation failed*/
                    error_found = 1;
                    continue;
                }

                temp_code = new_buffer;
                temp_code_size = new_size;
            }

            strcat(temp_code, line); /*Adds the current line to the temp_code*/
            temp_code_used += strlen(line);
            continue;
        }
        fprintf(fp2, "%s", line); /*Prints regular lines only in the am file*/
    }
    if (mcro_flag)
    {
        report_error(linecount, Error_38);
        error_found = 1;
    }

    free(temp_code);
    fclose(fp);

    /* If there is an error delete fp2 and return 0*/
    if (error_found)
    {
        remove(am_file_name);
        free(am_file_name);
        return 0;
    }
    else
    {
        fclose(fp2);
        free(am_file_name);
        if (has_line_too_long)
        {
            return 2;
        }
        return 1;
    }
}


int valid_macro_dec(char *mcro_name)
{
    /*Checks that the mcro name is not a name of operation/instruction/register*/
    if (!cmp_mcro_name(mcro_name))
    {
        /*error - invalid mcro name*/
        return 0;
    }

    /*Checks if the mcro name starts with a letter or _ and contains alphanumeric characters or _
    and that its legnth is not greater than 31*/
    if (!mcro_letters(mcro_name))
    {
        /*error - invalid mcro name*/
        return 0;
    }

    if (get_macro(mcro_name))
    {
        /* Already in the macro table*/
        return 0;
    }
    return 1;
}
