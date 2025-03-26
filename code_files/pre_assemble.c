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

/**
 * @brief This function is the pre_assembler.
 * 
 * @param file_name - the name of the file to check.
 * @return int - returns 1 if the pre_assembler has finished successfuly and 0 if it hasn't.
 */

 static int error_found = 0;


int preprocess(char* file_name){
    
    FILE *fp = NULL, *fp2 = NULL;
    /*fp=as file pointer, fp2=am file pointer*/
    char *temp_code, *code;
    char first_thing[MAX_MCRO_LENGTH];
    char mcro_name[MAX_MCRO_LENGTH];
    char line[MAX_LINE_LENGTH + 1];
    char* am_file_name;
    char* pos;
    int i = 0;
    int linecount = 0;
    size_t line_len = 0;
    size_t temp_code_size = 0;    /* Current allocated size for temp_code */
    size_t temp_code_used = 0;    /* Currently used bytes in temp_code */
    size_t new_size = 0;
    char *new_buffer;
    int mcro_flag = 0;
    /*mcro_flag=flag to know if the text is a macro code*/

    am_file_name = change_suffix(file_name, ".am");
    fp = fopen(file_name, "r");
    if (!fp) {
        report_error(linecount,Error_1);/*can't open file*/
        return 0;
    }
    fp2 = fopen(am_file_name, "w");/*add .am to the file name*/
    if (!fp2) {
        report_error(linecount,Error_1);/*can't open file*/
        fclose(fp);
        return 0;
    }

    /* Initial allocation for temp_code - will grow as needed */
    temp_code_size = MAX_LINE_LENGTH * 2;
    temp_code = (char*) malloc(temp_code_size);
    if (!temp_code) {
        report_error(linecount, Error_4); /*Memory allocation failed*/
        fclose(fp);
        fclose(fp2);
        return 0;
    }
    temp_code[0] = '\0';
        
    /*the loop ends when the file ends*/
    while (fgets(line, sizeof(line), fp)) {
        linecount++;
        /* Remove newline character if present */

        /* If the line len is larger than 80 without \n error */
        if (strlen(line) > 81) {
            report_error(linecount, Error_35);
        }
        
        pos = skip_whitespace(line);
        /* Skip empty lines */
        if (pos[0] == '\0' || pos[0] == '\n') {
            if (!mcro_flag){
                fprintf(fp2, "%s", line);
            }
            continue;
        }

        i = 0;
        while (pos[0] != '\0' && !isspace((unsigned char)pos[0]) && i < MAX_MCRO_LENGTH-1 && pos[0] != '\n') {
            first_thing[i++] = *pos++;
        }
        first_thing[i] = '\0'; 
        if(get_macro(first_thing)){/*if temp is a macro name which was declared before*/
            code = get_macro_code(first_thing);
            fprintf(fp2, "%s", code);/*prints the macro code instead of its name*/
            continue;
        }
        pos = skip_whitespace(line);

        /*step3*/
        if ((pos = strstr(line,"mcro"))){/*A start of macro declaration*/
        
            if ((pos>line && !isspace(*(pos-1))) || (*(pos+4) != '\0' && !isspace(*(pos+4)))){
                    ;/*not a macro declaration - regular line*/
            }
            else{
                if(strcmp(first_thing,"mcro")){
                    report_error(linecount, Error_2);/*Extra chars in macro declaration line*/
                    error_found = 1;
                    continue;
                }
    
                pos += 5;
                pos = skip_whitespace(pos);
                
                i = 0;
                while (pos[0] != '\0' && !isspace((unsigned char)pos[0]) && i < MAX_MCRO_LENGTH-1 && pos[0] != '\n') {
                    mcro_name[i++] = *pos++;
                }
                mcro_name[i] = '\0'; 
    
                pos = skip_whitespace(pos);
                if (*pos != '\n' && *pos != '\0'){
                    report_error(linecount, Error_2);/*Extra chars in macro declaration line*/
                    error_found = 1;
                }
    
                if(strlen(mcro_name) == 0){
                    report_error(linecount,Error_6);/*There is no macro name*/
                    error_found = 1;
                }
    
                if(!valid_macro_dec(mcro_name)){
                    report_error(linecount,Error_33);/*Unvalid macro name*/
                    error_found = 1;
                    continue;
                }
    
                mcro_flag = 1;/*macro declaration*/
                create_macro(mcro_name, linecount);/*create a macro with the name temp*/
                temp_code[0] = '\0';
                temp_code_used = 0;
                continue;
            }
            
        }
        
        if (mcro_flag){

            if ((pos = strstr(line,"mcroend"))){/*A start of macro declaration*/
                if((pos>line && !isspace(*(pos-1))) || (*(pos+7) != '\0' && !isspace(*(pos+7)))){
                    ;/*not a macroend declaration - part of the macro*/
                }
                else{
                    if(strcmp(first_thing,"mcroend")){
                        report_error(linecount, Error_3);/* Invalid macroend declaration*/
                        error_found = 1;
                    }
        
                    pos += 7;
                    pos = skip_whitespace(pos);
                    
                    if (*pos != '\n' && *pos != '\0'){
                        report_error(linecount,Error_3); /* Invalid macroend declaration*/
                        error_found = 1;
                    }

                    mcro_flag = 0;/*macro declaration has ended*/
                    add_code_to_macro(temp_code);/*adds the whole macro code to the macro*/
                    temp_code[0] = '\0';/*initilazes the temp_code*/
                    continue;
                }
                
            }
            line_len = strlen(line);
            if (temp_code_used + line_len + 1 > temp_code_size) {
                /* Double the buffer size */
                new_size = temp_code_size * 2;
                new_buffer = (char*) realloc(temp_code, new_size);
                
                if (!new_buffer) {
                    report_error(linecount, Error_4); /*Memory allocation failed*/
                    error_found = 1;
                    continue;
                }
                
                temp_code = new_buffer;
                temp_code_size = new_size;
            }

            strcat(temp_code, line); /*adds the current line to the temp_code*/
            temp_code_used += strlen(line);
            continue;
        }
        fprintf(fp2,"%s",line);/*prints regular lines only in the am file*/
    }
    if (mcro_flag) {
        report_error(linecount, Error_38);
        error_found = 1;
    }

    free(temp_code);
    fclose(fp);

    /* if there is an error delete fp2 and return 0*/
    if (error_found){
        remove(am_file_name);
        free(am_file_name);
        return 0;
    }
    fclose(fp2);
    free(am_file_name);
    return 1;   
}


/**
 * @brief This function chaecks the validation of the macro name.
 * 
 * @param mcro_name - the name of the macro.
 * @return int - returns 1 if the macro name is valid and 0 if it isn't.
 */
int valid_macro_dec(char *mcro_name)
{
    /*checks that the mcro name is not a name of operation/instruction/register*/
    if(!cmp_mcro_name(mcro_name)){
        /*error - invalid mcro name*/
        return 0;
    }

    /*checks if the mcro name starts with a letter or _ and contains alphanumeric characters or _
    and that its legnth is not greater than 31*/
    if(!mcro_letters(mcro_name)){
       /*error - invalid mcro name*/
        return 0;
    }   

    if(get_macro(mcro_name)){
        /* already defined*/
        return 0;
    }
    return 1;
}
