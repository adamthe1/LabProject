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
    char* str, *temp, *temp_code="", *code;
    /*str=the next line, temp=copy of str to check, temp_code=the code of the current mcro, code=the code to replace macro name*/
    int mcro_flag = 0, linecount = 0;
    /*mcro_flag=flag to know if the text is a macro code*/
    fp = fopen(file_name, "r+");
    fp2 = fopen(strcat(strtok(file_name,"."),".am"), "w");
    if (!fp) {
        report_error(linecount,Error_1);/*can't open file*/
        return 0;
    }
    
    while(fp != NULL){
        str = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
        if (!str) {
            report_error(linecount,Error_4);/*Memory allocation failed*/
            return 0;
        }
        
        if (!fgets(str, MAX_LINE_LENGTH, fp)) {
            if (feof(fp)) {/*End of file*/
                ;
            } else {
                report_error(linecount, Error_5); /* Reading text failed */
            }
            free(str);
            break;
        }
        
        temp = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
        if (!temp) {
            report_error(linecount,Error_4);/*Memory allocation failed*/
            free(str);
            return 0;
        }
        
        if (temp_code == NULL || strlen(temp_code) == 0) {
            temp_code = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
            if (!temp_code) {
                report_error(linecount,Error_4);/*Memory allocation failed*/
                free(str);
                free(temp);
                return 0;
            }
            temp_code[0] = '\0';/* Initialize as empty string */
        }
        linecount++;
        strcpy(temp,str);/*copy str into temp to check temp without changing str*/
        extra_spaces(temp);/*remove extra xpaces from temp to make the checks easier*/
        /*step2*/
        temp = strtok(temp, " ");/*temp = the next word*/
        if(temp){/*if temp != null*/
            if(strlen(temp) > 0 && temp[strlen(temp)-1] == '\n'){
                temp[strlen(temp)-1] = '\0';/*remove the enter char*/
            }     
        }
        if(get_macro(temp)){/*if temp is a macro name which was declared before*/
            code = get_macro_code(temp);
            fprintf(fp2,"%s",code);/*prints the macro code instead of its name*/
            continue;
        }
        /*step3*/
        if(!strcmp(temp,"mcro")){
            temp = strtok(NULL, " ");
            if(!temp){
                report_error(linecount,Error_2);/*Unvalid macro declaration*/
                return 0;
            }
            /*checks that there are no extra chars in the line of the macro declaration*/
            if(strlen(temp) > 0 && temp[strlen(temp)-1] == '\n'){
                temp[strlen(temp)-1] = '\0';
            }
            if(!valid_macro_dec(temp)){
                report_error(linecount,Error_2);/*Unvalid macro declaration*/
                return 0;
            }
            mcro_flag = 1;/*macro declaration*/
            create_macro(temp, linecount);/*create a macro with the name temp*/
            continue;
        }
        
        if (mcro_flag==1)
        {
            if(!strcmp(temp,"mcroend")){/*if the macro code has ended*/
                if(strtok(NULL, " ")){/*checks that there are no extra chars in the line of the macroend declaration*/
                    report_error(linecount,Error_3);/*Unvalid macroend declaration*/
                    return 0;
                }
                add_code_to_macro(temp_code);/*aads the whole macro code to the macro*/
                temp_code = "";/*initilazes the temp_code*/
                mcro_flag = 0;/*macro declaration has ended*/
                continue;
            }
            strcat(temp_code,str);/*adds the current line to the temp_code*/
            continue;
            }
            fprintf(fp2,"%s",str);/*prints regular lines only in the am file*/
    }
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