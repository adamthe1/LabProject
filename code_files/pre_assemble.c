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

int preprocess(char* file_name){
    
    FILE *fp, *fp2;
    /*fp=as file pointer, fp2=am file pointer*/
    char* str,*str2, *temp, *temp_code, *code, *rest;
    /*str=the next line,str2=copy of str to use in strtok, temp=token of str2-the next word in the line,
      temp_code=the code of the current mcro, code=the code to replace macro name, rest=extra chars in the declaration lines*/
    int mcro_flag = 0, linecount = 0;
    /*mcro_flag=flag to know if the text is a macro code*/

    fp = fopen(file_name, "r+");
    if (!fp) {
        report_error(linecount,Error_1);/*can't open file*/
        return 0;
    }
    fp2 = fopen(strcat(strtok(file_name,"."),".am"), "w");/*add .am to the file name*/
    if (!fp2) {
        report_error(linecount,Error_1);/*can't open file*/
        return 0;
    }
    
    /*the loop ends when the file ends*/
    while (!feof(fp)) {
        str = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
        if (!str) {
            report_error(linecount,Error_4);/*Memory allocation failed*/
            return 0;
        }
        /*reads the next line into str*/
        if (!fgets(str, MAX_LINE_LENGTH, fp)){
            if (feof(fp)) {/*End of file*/
                ;
            } else {
                report_error(linecount, Error_5); /* Reading text failed */
            }
            free(str);
            break;
        }
        
        temp = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
        if (!temp){
            report_error(linecount,Error_4);/*Memory allocation failed*/
            free(str);
            return 0;
        }

        str2 = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
        if (!str2){
            report_error(linecount,Error_4);/*Memory allocation failed*/
            free(str);
            free(temp);
            return 0;
        }

        if (temp_code == NULL || strlen(temp_code) == 0){
            temp_code = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
            if (!temp_code) {
                report_error(linecount,Error_4);/*Memory allocation failed*/
                free(str);
                free(temp);
                free(str2);
                return 0;
            }
            temp_code[0] = '\0';/* Initialize as empty string */
        }

        linecount++;
        strcpy(str2,str);/*copy str into str2 to use strtok without changing str*/
        extra_spaces(str2);/*remove extra spaces from str2 to make the checks easier*/
        /*step2*/
        temp = strtok(str2, " ");/*temp = the first word*/

        if(!temp){/*if the line is empty*/
            continue;
        }

        if(get_macro(temp)){/*if temp is a macro name which was declared before*/
            code = get_macro_code(temp);
            printf("%s",code);
            fprintf(fp2,"%s",code);/*prints the macro code instead of its name*/
            continue;
        }
        /*step3*/
        if(!strcmp(temp,"mcro")){/*A start of macro declaration*/
            temp = strtok(NULL, " ");/*macro name*/
            rest = strtok(NULL," ");/*the rest of the line*/
            if(rest){
                report_error(linecount,Error_2);/*Extra chars in macro declaration line*/
                return 0;
            }
            if(!temp){
                report_error(linecount,Error_6);/*There is no macro name*/
                return 0;
            }
            if(!valid_macro_dec(temp)){
                report_error(linecount,Error_33);/*Unvalid macro name*/
                return 0;
            }
            mcro_flag = 1;/*macro declaration*/
            create_macro(temp, linecount);/*create a macro with the name temp*/
            continue;
        }
        
        if (mcro_flag==1){
            if(!strcmp(temp,"mcroend")){/*if the macro code has ended*/
                rest = strtok(NULL," ");
                if(rest){/*checks that there are no extra chars in the line of the macroend declaration*/
                    report_error(linecount,Error_3);/*Unvalid macroend declaration*/
                    return 0;
                }
                add_code_to_macro(temp_code);/*adds the whole macro code to the macro*/
                temp_code[0] = '\0';/*initilazes the temp_code*/
                mcro_flag = 0;/*macro declaration has ended*/
                continue;
            }
            strcat(temp_code,str);/*adds the current line to the temp_code*/
            continue;
        }
            fprintf(fp2,"%s",str);/*prints regular lines only in the am file*/
    }
    free(str);
    free(temp);
    free(temp_code);
    fclose(fp);
    fclose(fp2);
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
    return 1;
}