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
    char* str,*str2, *temp, *temp_code = NULL, *code;
    /*str=the next line,str2=copy of str to use in strtok, temp=token of str2-the next word in the line,
      temp_code=the code of the current mcro, code=the code to replace macro name, rest=extra chars in the declaration lines*/
    int mcro_flag = 0, linecount = 0;
    /*mcro_flag=flag to know if the text is a macro code*/
    int word_counter = 0;
    int char_to_advance = 0;
    char* pos;
    
    fp = fopen(file_name, "r+");
    if (!fp) {
        report_error(linecount,Error_1);/*can't open file*/
        return 0;
    }
    fp2 = fopen((change_suffix(file_name,".am")), "w");/*add .am to the file name*/
    if (!fp2) {
        report_error(linecount,Error_1);/*can't open file*/
        return 0;
    }
    str = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
    if (!str) {
        report_error(linecount,Error_4);/*Memory allocation failed*/
        return 0;
    }

    temp = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
    if (!temp){
        report_error(linecount,Error_4);/*Memory allocation failed*/
        free(str);
        return 0;
    }

    temp_code = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
    if (!temp_code) {
        report_error(linecount,Error_4);/*Memory allocation failed*/
        free(str);
        free(temp);
        return 0;
    }

    temp_code[0] = '\0';/* Initialize as empty string */
    /*the loop ends when the file ends*/
    while (fgets(str, MAX_LINE_LENGTH, fp)) {/*reads the next line into str*/
        str2 = str;
        printf("line = %s\n", str);
        linecount++;
        pos = skip_whitespace(str);
        if (*pos == '\0') {/*Skip empty lines*/
            continue;
        }
        printf("skip line okay\n");
        /*step2*/
        temp = next_word(str2, &char_to_advance);
        if(!temp){/*if the line is empty*/
            continue;
        }
        if(get_macro(temp)){/*if temp is a macro name which was declared before*/
            code = get_macro_code(temp);
            printf("macro = %s\nmacro code = %s",temp,code);
            fprintf(fp2,"%s",code);/*prints the macro code instead of its name*/
            continue;
        }
        /*step3*/
        if(look_for_word(str2,"mcro",&word_counter)){/*A start of macro declaration*/
            if(word_counter == -1 || word_counter > 2){
                report_error(linecount,Error_2);/*Extra chars in macro declaration line*/
                return 0;
            }
            if(word_counter == 1){
                report_error(linecount,Error_6);/*There is no macro name*/
                return 0;
            }
            str2 += char_to_advance;
            temp = next_word(str2,&char_to_advance);
            printf("macro name = %s\n", temp);
            if(!valid_macro_dec(temp)){
                report_error(linecount,Error_33);/*Unvalid macro name*/
                return 0;
            }
            mcro_flag = 1;/*macro declaration*/
            create_macro(temp, linecount);/*create a macro with the name temp*/
            continue;
        }
        if (mcro_flag==1){
            if(look_for_word(str,"mcroend",&word_counter)){/*if the macro code has ended*/
                if(word_counter == -1 || word_counter > 1){/*checks that there are no extra chars in the line of the macroend declaration*/
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

    printf("end of function");
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

char* next_word(char* line, int* char_counter){
    char* word;

    word = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
    if (!word) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    *char_counter = 0;

    line = skip_whitespace(line);
    if((*line) == '\0'){
        free(word);
        return NULL;
    }
    while((line)[*char_counter] !='\0' && !isspace((line)[*char_counter])){
        (*char_counter)++;
    }
    
    strncpy(word,line,*char_counter);
    word[*char_counter] = '\0';
    return word; 
    
}

int look_for_word(char* line, char* word, int* word_counter){
    /*assums that the line is not empty*/
    int found_word = 0;
    int char_to_advance = 0;
    char* copy_line;
    char* current_word;
    copy_line = line;
    current_word = next_word(copy_line, &char_to_advance);
    copy_line += char_to_advance+1;
    (*word_counter) = 0;

    while (current_word && strlen(current_word) > 0)
    {
        if(!strcmp(word, current_word)){
            if(*word_counter>0){
                *word_counter = -1;
                return 1;
             }
            found_word = 1;
        }
        (*word_counter)++;
        free(current_word);
        current_word = next_word(copy_line, &char_to_advance);
        copy_line += char_to_advance+1;
    }
    
    return found_word;
    
}