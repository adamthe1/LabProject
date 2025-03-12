
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/macro_table.h"
#include "../headers/pre_assemble.h"
#include "../headers/helper.h"
#include "../headers/define.h"

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
int preprocess2(char* file_name){
    
    FILE *fp, *fp2;
    char* str, *temp, *temp_code="", *code;
    int mcro_flag = 0, linecount = 0;
    fp = fopen(file_name, "r+");
    fp2 = fopen(strcat(strtok(file_name,"."),".am"), "w");
    if (fp == NULL) {
        /*error - opening file failed*/
        return 0;
    }
    
    
    while(fp != NULL){
        str = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
        if (!str) {
            printf("Error: Memory allocation failed\n");
            return 0;
        }
        
        if (!fgets(str, MAX_LINE_LENGTH, fp)) {
            free(str);
            break;
        }
        
        temp = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
        if (!temp) {
            printf("Error: Memory allocation failed\n");
            free(str);
            return 0;
        }
        
        if (temp_code == NULL || strlen(temp_code) == 0) {
            temp_code = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
            if (!temp_code) {
            printf("Error: Memory allocation failed\n");
            free(str);
            free(temp);
            return 0;
            }
            temp_code[0] = '\0';  /* Initialize as empty string */
        }
        
        linecount++;
        strcpy(temp,str);
        extra_spaces(temp);
        /*step2*/
        temp = strtok(temp, " ");
        if(get_macro(temp)){
            code = get_macro_code(temp);
            fprintf(fp2,code);
            continue;
        }
        
        
        /*step3*/
        if(strcmp(temp,"mcro")){
            temp = strtok(NULL, " ");
            if(!temp){
                /*error*/ printf("error1");
                return 0;
            }
            /*checks that there are no extra chars in the line of the macro declaration*/
            if(temp[strlen(temp)-1] == '\n')
                temp[strlen(temp)-1] = '\0';
            if(strtok(NULL, " ") || !valid_macro_dec(temp)){
                /*error*/ printf("error2");
                return 0;
            }
            mcro_flag = 1;
            /* remove /n from the end of temp if its there */
            
            create_macro(temp, linecount);
            continue;
        }
        
        
        if (mcro_flag==1)
        {
            if(strcmp(strtok(temp, " "),"mcroend")){
                /*checks that there are no extra chars in the line of the macroend declaration*/
                if(strtok(NULL, " ")){
                    /*error*/ printf("error3");
                    return 0;
                }
                add_code_to_macro(temp_code);
                mcro_flag = 0;
                continue;
            }
            strcat(temp_code,str);
        }
        fprintf(fp2,str);
    }
        
}

/**
 * @brief This function chaecks the validation of the macro name.
 * 
 * @param mcro_name - the name of the macro.
 * @return int - returns 1 if the macro name is valid and 0 if it isn't.
 */
int valid_macro_dec(char *mcro_name)
{
    /*checks that the mcro name is not null*/
    char* temp;
    
    /* No need to use strtok here, mcro_name is already the token we want to check */
    
    temp = mcro_name;
    if(!temp)
    {
        /*error*/ printf("error4");
        return 0;
    }

    /*checks that the mcro name is not a name of operation/instruction/register*/
    if(!cmp_mcro_name(temp)){
        /*error - invalid mcro name*/
         /*error*/ printf("error5");
        return 0;
    }

    /*checks if the mcro name starts with a letter or _ and contains alphanumeric characters or _
    and that its legnth is not greater than 31*/
    if(!mcro_letters(temp)){
       /*error - invalid mcro name*/
        /*error*/ printf("error6");
       return 0; 
    }
        
    return 1;
}


