#include "../headers/helper.h"
#include "../headers/macro_table.h"
#include "../headers/pre_assemble.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int preprocess(char* file_name){
    FILE* file = fopen(file_name, "r");
    char line[81];
    char* macro_name;
    int line_index = 0;
    while(fgets(line, sizeof(line), file)){
        line_index++;
        if(strstr(line, "#define") != NULL){
            macro_name = strtok(line, " ");
            if(macro_name == NULL){
                return 0;
            }
            if(create_macro(macro_name, line_index) == 0){
                return 0;
            }
        }else{
            if(add_code_to_macro(line) == 0){
                return 0;
            }
        }
    }
    fclose(file);
    return 1;
}