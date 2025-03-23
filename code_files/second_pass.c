#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/define.h"
#include "../headers/first_pass.h"
#include "../headers/label_data_struct.h"
#include "../headers/errors.h"
#include "../headers/helper.h"
#include "../headers/analyze.h"
#include "../headers/second_pass.h"

/*
Algorithem steps:
1. Read the next line from the source file. If the end of the source file is reached, proceed to step 7.
2. If the first field in the line is a symbol (label), skip it.
2. Is this a data, string, or extern directive? If so, go back to step 1.
3. Is this an entry directive? If not, proceed to step 6.
4. Add the entry attribute to the symbol’s properties in the symbol table,
   corresponding to the symbol appearing as the operand of the directive.
5. Complete the binary encoding of the operands' data words according to the addressing methods
   (if the symbol is not found in the symbol table, report an error). Return to step 1.
6. For each operand in the source code that contains a symbol,
   find the symbol’s value in the symbol table. If the symbol is defined as external,
   add the address of the relevant data word to the list of data words referring to the external symbol.
   If the symbol is not found in the table, report an error.
7. The source file has been fully read. If errors were found in the second pass, stop here.
8. Build the output files (more details to follow).
*/


/* Global variables */
static char line[MAX_LINE_LENGTH + 1]; /* +1 for possible \n */
static int line_number = 0;
static int error_found = 0;

int second_pass(char* file_name ,int* IC, int* DC){
    int symbol_flag = 0;
    char *token;
    char *temp_line;
    char label_name[MAX_LABEL_LEN];
    int inst_type, i;
    char* pos;
    FILE* file, *fp_ob,*fp_ext,*fp_ent;
    
    temp_line = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
    if (!temp_line){
        report_error(line_number,Error_4);/*Memory allocation failed*/
        return 0;
    }

    token = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
    if (!token){
        report_error(line_number,Error_4);/*Memory allocation failed*/
        free(temp_line);
        return 0;
    }

    file = fopen(file_name, "r");
    line_number = 0;
    error_found = 0;
    
    /* Reset file pointer to beginning */
    rewind(file);

    /*step 1*/
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        /* Remove newline character if present */
        pos = strchr(line, '\n');
        if (pos) *pos = '\0';
        
        /* Skip empty lines and comments */
        pos = skip_whitespace(line);
        if (*pos == '\0' || *pos == ';') {
            continue;
        }

        /*step 2*/
        /*If(the first field in the line is a symbol (label)) - skip it*/
        strcpy(temp_line,line);/*temp_line is copy of str for the strtok function*/
        token = strtok(temp_line, " \t");
        if(get_label(token)){
            memmove(line, line + strlen(token), strlen(line));
        }

        /*step 3*/
        if(identify_instruction(line) == INST_TYPE_DATA
            || identify_instruction(line) == INST_TYPE_STRING
           || identify_instruction(line) == INST_TYPE_EXTERN){
            continue;
           }

        /*step 4*/
        if(identify_instruction(line) == INST_TYPE_ENTRY){
            /*steps 5*/
            token = strtok(NULL, " \t");
            if(!get_label(token)){
                report_error(line_number,Error_34);/*Label name doesn't existe in the label table*/
            }
            add_entry(token);
            continue;
        }
            
        /*step 6*/
        if(identify_instruction(line) != INST_TYPE_OPERATION){
            /*error - not instruction or operation*/
        }
        if(!process_operation2(line,IC)){
            /*error*/
        }   
    }
    /*step 7*/
    fclose(file);
    if(error_found){/*if there is at least 1 error in the file found in the second pass*/
        printf("Errors were found during the second pass in the file, assembler  process can't be completed\n");
        return 0;
    }
    /*step 8 - bulid the output files*/

    fp_ob = fopen(change_suffix(file_name,".ob"), "w");/*add .ob to the file name*/
    if (!fp_ob) {
        report_error(line_number,Error_1);/*can't open file*/
        return 0;
    }
    fprintf("%d %d\n",IC-100,DC);/*print the file headline*/
    for(i = 100;i<*IC+*DC;i++){
        fprintf(fp_ob,"%07d 0x%06X\n",i,get_binary_code(i));

    }

    fp_ent = fopen(change_suffix(file_name,".ent"), "w");/*add .ent to the file name*/
    if (!fp_ent) {
        report_error(line_number,Error_1);/*can't open file*/
        return 0;
    }
    Label* current = get_label_head();
    while(current != NULL){
        if(current->is_entry){
            fprintf("%s %07d\n",current->name,current->line_index);
        }
        current = current->next;
    }

    
    fp_ext = fopen(change_suffix(file_name,".ext"), "w");/*add .ext to the file name*/
    if (!fp_ext) {
        report_error(line_number,Error_1);/*can't open file*/
        return 0;
    }
    /*

    current = get_label_head();
    while(current != NULL){
        if(current->is_entry){
            fprintf("%s %07d\n",current->name,current->line_index);
        }
        current = current->next;
    }
    */
    
    free(temp_line);
    fclose(fp_ob);
    fclose(fp_ent);
    fclose(fp_ext);
    
    return 1;
}


int process_operation2(char* line, int* IC) {
    char* pos;
    char operation[MAX_OPERATION_LEN + 1];
    char operand1[MAX_OPERAND_LEN + 1] = {0};
    char operand2[MAX_OPERAND_LEN + 1] = {0};
    Op_code* opcode;
    int operand_count = 0;
    int source_addr_mode = 0;
    int source_register = 0;
    int target_addr_mode = 0;
    int target_register = 0;
    int i;
    int word_count = 1; /* Always at least one word for the operation */
    int instruction_word = 0;
    
    /* Skip label if present - maybe fix case of no label but incorrect : */
    pos = strchr(line, ':');
    if (pos != NULL) {
        line = pos + 1;
    }
    
    line = skip_whitespace(line);
    
    /* Extract operation name */
    i = 0;
    while (*line && !isspace((unsigned char)*line) && i < MAX_OPERATION_LEN) {
        operation[i++] = *line++;
    }
    operation[i] = '\0';

    opcode = get_opcode(operation);

    /* Check if it's a known operation*/
    if (opcode == NULL) {
        report_error(line_number, Error_20);/*Failed to find operation*/
        return 0;
    }
    
    /* Skip whitespace after operation */
    line = skip_whitespace(line);
    
    /* Parse operands if any */
    if (*line != '\0') {
        i = 0;
        while (*line && *line != ',' && i < MAX_OPERAND_LEN) {
            operand1[i++] = *line++;
        }
        operand1[i] = '\0';
        trim_whitespace(operand1); 
        operand_count++;

        if(!is_valid_operand(operand1)){
            report_error(line_number, Error_29);/*Syntax error in operand 1*/
            return 0;
        }
        
        /* Check if there's a second operand */
        if (*line == ',') {
            line++; 
            line = skip_whitespace(line);
            
            i = 0;
            while (*line && i < MAX_OPERAND_LEN) {
                operand2[i++] = *line++;
            }
            operand2[i] = '\0';
            trim_whitespace(operand2); 
            operand_count++;

            if(!is_valid_operand(operand2)){
                report_error(line_number, Error_30);/*Syntax error in operand 2*/
                return 0;
            }
        }
    }
    
    /* check expected operand count */
    if (operand_count != opcode->operand_count) {
        report_error(line_number, Error_26);/*Incorrect amount of operands*/
        return 0;
    }
    
    /* Determine addressing modes and registers for operands */
    if (operand_count > 0) {
        /* First operand (source) */
        if (!determine_addressing_mode(operand1, &source_addr_mode, &source_register)) {
            report_error(line_number, Error_31);/*Invalid operand*/
            return 0;
        }

        if (!is_valid_addressing_mode(opcode, source_addr_mode, operand_count == 2)) {
            report_error(line_number, Error_27);/*Failed to create binary code*/
            return 0;

        }
        /* If not register addressing, we need another word */
        if (source_addr_mode != ADDR_MODE_REGISTER) {
            word_count++;
        }
        
        /* Second operand (target) if present */
        if (operand_count > 1) {
            if (!determine_addressing_mode(operand2, &target_addr_mode, &target_register)) {
                report_error(line_number, Error_31);/*Invalid operand*/
                return 0;
            }

            if (!is_valid_addressing_mode(opcode, target_addr_mode, 0)) {
                report_error(line_number, Error_27);/*Invalid addressing mode*/
                return 0;
            }

            /* If not register addressing, we need another word */
            if (target_addr_mode != ADDR_MODE_REGISTER) {
                word_count++;
            }
        }
    }
    
    /* Process source operand if not a register and add 1 to IC */
    if (operand_count > 0 && source_addr_mode != ADDR_MODE_REGISTER) {
        if (!encode_operand2(operand1, source_addr_mode)) {
            return 0;
        }
    }
    
    /* Process target operand if present and not a register and add 1 to IC */
    if (operand_count > 1 && target_addr_mode != ADDR_MODE_REGISTER) {
        if (!encode_operand2(operand2, target_addr_mode)) {
            return 0;
        }
    }
    
    return 1;
}


int encode_operand2(char* operand, int addr_mode) {
    int operand_word = 0;
    long value;
    char* label;
    Label* label_entry;
    int IC;

    label_entry = get_label(operand);
    if(!label_entry){
        report_error(line_number,Error_34);/*Label name doesn't existe in the label table*/
        return 0;
    }
    
    IC = label_entry->line_index;
    
    switch(addr_mode) {
        case ADDR_MODE_IMMEDIATE:
            /* Immediate value: 21 bits for value, ARE = 001 (absolute) */
            value = strtol(operand + 1, NULL, 10); /* Skip the # */

            if (value < -1048576 || value > 1048575) {
                report_error(line_number, Error_13);/*Number out of range*/
                return 0;
            }
            operand_word = ((int)value & 0x1FFFFF) << 3; /* 21-bit value */
            operand_word |= 1; /* ARE = 001 (absolute) */
            break;
            
        case ADDR_MODE_DIRECT:
            /* Direct addressing: 21 bits for address, ARE = 010 (relocatable) or 100 (external) */
            label = operand;
            label_entry = get_label(label);
            
            if (label_entry == NULL) {
                /* Label not found - will be resolved in second pass */
                operand_word = UKNOWN_LABEL_DIRECT; 
            } else if (label_entry->type == EXTERN_TYPE) {
                operand_word = 0; 
                operand_word |= 4; /* ARE = 100 (external) */
            } else {
                operand_word = (label_entry->line_index & 0x1FFFFF) << 3; /* 21-bit address */
                operand_word |= 2; /* ARE = 010 (relocatable) */
            }
            break;
            
        case ADDR_MODE_RELATIVE:
            /* Relative addressing: 21 bits for offset, ARE = 001 (absolute) */
            label = operand + 1; /* Skip the & */
            label_entry = get_label(label);
            
            if (label_entry == NULL) {
                /* Label not found - will be resolved in second pass */
                operand_word = UKNOWN_LABEL_RELATIVE; 
            } else {
                /* Calculate distance (will be done in second pass) */
                operand_word = UKNOWN_LABEL_RELATIVE; 
            }
            break;
            
        default:
            return 0;
    }
    
    /* Store operand word */
    if (!create_binary_code(operand_word, IC)) {
        report_error(line_number, Error_28);/*Error 28: Failed to create binary code*/
        return 0;
    }
    
    (IC)++;
    return 1;
}