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

/* Global variables */
static char line[MAX_LINE_LENGTH + 1]; /* +1 for possible \n */
static int line_number = 0;
static int error_found = 0;


int first_pass(char* file_name) {
    int DC = 0;
    int IC = 100;
    int symbol_flag = 0;
    char label_name[MAX_LABEL_LEN];
    int inst_type;
    char* pos;
    FILE* file;
    
    file = fopen(file_name, "r");
    line_number = 0;
    error_found = 0;
    
    /* Reset file pointer to beginning */
    rewind(file);
    
    while (fgets(line, sizeof(line), file)) {

        line_number++;

        /* Remove newline character if present */
        pos = strchr(line, '\n');
        if (pos) *pos = '\0';

        /* If the line len is larger than 80 without \n error */
        if (strlen(line) > 80) {
            report_error(line_number, Error_35);
        }
        
        
        /* Skip empty lines and comments */
        pos = skip_whitespace(line);
        if (*pos == '\0' || *pos == ';') {
            continue;
        }
        
        /* Check for label */
        symbol_flag = parse_label(line, label_name);
        
        /* Identify instruction type */
        inst_type = identify_instruction(line);
        
        /* Process based on instruction type */
        switch (inst_type) {
            case INST_TYPE_DATA:
                if (symbol_flag) {
                    /* Add label to symbol table as data */
                    if (get_label(label_name)){
                        report_error(line_number, Error_7);/*Label name already in label table*/
                        error_found = 1;
                    }

                    if (!create_label(label_name, DATA_TYPE, DC)) {
                        report_error(line_number, Error_21);/*Failed to create data label*/
                        error_found = 1;
                    }
                }
                if (!process_data_directive(line, &DC)) {
                    error_found = 1;
                }
                break;
                
            case INST_TYPE_STRING:
                if (symbol_flag) {
                    /* Add label to symbol table as data */
                    /* Must be unique - check if that name is already in label */
                    if (get_label(label_name)){
                        report_error(line_number, Error_7);/*Label name already in label table*/
                        error_found = 1;
                    }
                    if (!create_label(label_name, DATA_TYPE, DC)) {
                        report_error(line_number, Error_22);/*Failed to create string label*/
                        error_found = 1;
                    }
                }
                if (!process_string_directive(line, &DC)) {
                    error_found = 1;
                }
                break;
                
            case INST_TYPE_ENTRY:
                /* Entry directives are processed in second pass */
                break;
                
            case INST_TYPE_EXTERN:
                if (!process_extern_directive(line)) {
                    error_found = 1;
                }
                break;
                
            case INST_TYPE_OPERATION:
                if (symbol_flag) {
                    /* Add label to symbol table as code */
                    if (get_label(label_name)){
                        report_error(line_number, Error_7);/*Label name already in label table*/
                        error_found = 1;
                    }
                    if (!create_label(label_name, CODE_TYPE, IC)) {
                        report_error(line_number, Error_23);/*Failed to create code label*/
                        error_found = 1;
                    }
                }
                
                if (!process_operation(line, &IC)) {
                    error_found = 1;
                }
                break;
                
            default:
                report_error(line_number, Error_24);/*Unknown Instruction Type*/
                error_found = 1;
        }
    }
    
    /* Update addresses for data labels to come after code */
    /* This would involve traversing your label table and updating addresses */
    if(!add_IC_to_DC(IC)){
        report_error(line_number, Error_28); /*Failed to create binary code*/
        error_found = 1;
    }
    DC = DC + IC;
    
    fclose(file);
    
    if(!error_found){
        return second_pass(file_name,&IC,&DC);
    }
    printf("Errors were found during the first pass in the file, assembler  process can't be completed");
    return 0;
}

int parse_label(char* line, char* label_name) {
    char* colon_pos;
    int label_len;
    
    line = skip_whitespace(line);
    
    
    colon_pos = strchr(line, ':');
    if (colon_pos == NULL) {
        return 0;
    }

    /* Extract the label name */
    label_len = colon_pos - line;
    if (label_len >= MAX_LABEL_LEN) {
        report_error(line_number, Error_8);/*Label name is too long*/
        error_found = 1;
    }
    
    strncpy(label_name, line, label_len);
    label_name[label_len] = '\0';
    
    /* Validate label name */
    if (!is_valid_label_name(label_name)) {
        report_error(line_number, Error_9);/*Invalid label name*/
        error_found = 1;
    }
    
    return 1;
}

int identify_instruction(char* line) {
    char* pos;
    
    /* Skip label if present */
    pos = strchr(line, ':');
    if (pos != NULL) {
        line = pos + 1;
    }
    
    line = skip_whitespace(line);
    
    /* Check for directives */
    if (*line == '.') {
        if (strncmp(line, ".data ", 5) == 0) {
            return INST_TYPE_DATA;
        } else if (strncmp(line, ".string ", 7) == 0) {
            return INST_TYPE_STRING;
        } else if (strncmp(line, ".entry ", 6) == 0) {
            return INST_TYPE_ENTRY;
        } else if (strncmp(line, ".extern ", 7) == 0) {
            return INST_TYPE_EXTERN;
        }
    }
    
    /* If not a directive, it's an operation */
    return INST_TYPE_OPERATION;
}

int process_data_directive(char* line, int* DC) {
    char* pos;
    int value;
    long int temp_value;
    char temp_char;
    int has_found_value = 0;
    char* num_start;
    
    /* Skip .data directive */
    pos = strstr(line, ".data");
    if (pos == NULL) {
        report_error(line_number, Error_10);/*Missing .data directive*/
        return 0;
    }
    
    pos += 5;  /* Skip ".data" */
    pos = skip_whitespace(pos);
    
    /* Check for empty data directive */
    if (*pos == '\0') {
        report_error(line_number, Error_11);/*Missing value in .data directive*/
        return 0;
    }
    
    /* Check for leading comma */
    if (*pos == ',') {
        report_error(line_number, Error_11);/*Missing value in .data directive*/
        return 0;
    }
    
    /* Parse comma-separated values manually */
    while (*pos != '\0') {
        /* Find start of the number */
        pos = skip_whitespace(pos);
        
        /* Check for unexpected end */
        if (*pos == '\0') {
            /* If we found a trailing comma before */
            if (!has_found_value) {
                report_error(line_number, Error_11);/*Missing value in .data directive*/
                return 0;
            }
            break;
        }
        
        /* Check for consecutive commas */
        if (*pos == ',') {
            report_error(line_number, Error_11);/*Missing value in .data directive*/
            return 0;
        }
        
        /* Mark start of number for conversion */
        num_start = pos;
        
        /* Check if it's a negative number */
        if (*pos == '-' || *pos == '+') {
            pos++;
        }
        
        /* Verify it's a digit */
        if (!isdigit((unsigned char)*pos)) {
            report_error(line_number, Error_12);/*Invalid value in .data directive*/
            return 0;
        }
        
        /* Find the end of the number */
        while (isdigit((unsigned char)*pos)) {
            pos++;
        }
        
        /* Check if the number is followed by invalid characters */
        if (*pos != ',' && *pos != '\0' && *pos != '\n' && *pos != ';' && !isspace((unsigned char)*pos)) {
            report_error(line_number, Error_12); /*Invalid value in .data directive (Invalid character in number) */
            return 0;
        }
        
        /* Convert the number */
        temp_char = *pos;
        *pos = '\0'; 
        temp_value = strtol(num_start, NULL, 10);
        *pos = temp_char; 
        
        /* Check for 12-bit range (-2048 to 2047) */
        if (temp_value < -2048 || temp_value > 2047) {
            report_error(line_number, Error_13);/*Number out of range*/
            return 0;
        }
        
        value = (int)temp_value;
        
        /* Store the value and increment DC */
        create_binary_data(value, *DC);
        (*DC)++;
        has_found_value = 1;
        
        /* Check for end of string or move past the comma */
        pos = skip_whitespace(pos);
        if (*pos == '\0') {
            break;
        }
        
        if (*pos == ',') {
            pos++; /* Skip the comma */
            has_found_value = 0; /* Reset for next number */
        } else {
            report_error(line_number, Error_12); /*Invalid value in .data directive (Expected comma) */
            return 0;
        }
    }
    
    return 1;
}

int process_string_directive(char* line, int* DC) {
    char* pos;
    char* start_quote;
    char* end_quote;
    char current_char;
    
    /* Skip .string directive */
    pos = strstr(line, ".string");
    if (pos == NULL) {
        report_error(line_number, Error_13);/*Number out of range*/
        return 0;
    }
    pos += 7;  
    pos = skip_whitespace(pos);
    
    /* Check for empty string directive */
    if (*pos == '\0') {
        report_error(line_number, Error_25);/*Empty string in .string*/
        return 0;
    }
    
    /* Check for opening quote*/
    if (*pos != '\"') {
        report_error(line_number, Error_14);/*Syntax error in .string directive*/
        return 0;
    }
    
    start_quote = pos;
    
    /* Find the closing quote */
    end_quote = strchr(start_quote + 1, '\"');
    if (end_quote == NULL) {
        report_error(line_number, Error_14); /*Syntax error in .string directive (Missing closing quote)*/
        return 0;
    }
    
    /* Check for extra content after closing quote */
    pos = end_quote + 1;
    pos = skip_whitespace(pos);
    if (*pos != '\0') {
        report_error(line_number, Error_15);/*Extranous text after command*/
        return 0;
    }
    
    /* Process the string contents */
    start_quote++; 
    current_char = *end_quote;
    *end_quote = '\0';
    
    /* Store each character */
    while (*start_quote != '\0') {
        /* Store character as integer representation */
        create_binary_data((int)*start_quote, *DC);
        (*DC)++;
        start_quote++;
    }
    
    /* Add null terminator in binary */
    create_binary_data(0, *DC);
    (*DC)++;
    
    *end_quote = current_char;
    
    return 1;
}

int process_extern_directive(char* line) {
    char* pos;
    char* label_start;
    char* label_end;
    char label_name[MAX_LABEL_LEN];
    char temp_char;
    Label* existing;
    int flag;
    
    /* Skip .extern directive */
    pos = strstr(line, ".extern");
    if (pos == NULL) {
        report_error(line_number, Error_18);/*Malformed .extern directive*/
        return 0;
    }
    pos += 7;  
    pos = skip_whitespace(pos);
    
    /* Check for empty extern directive */
    if (*pos == '\0') {
        report_error(line_number, Error_18);/*Missing .extern directive*/ 
        return 0;
    }
    
    /* Mark the start of the label */
    label_start = pos;
    
    /* Verify first character is a letter */
    if (!isalpha((unsigned char)*pos)) {
        report_error(line_number, Error_9);/*Label name is too long*/
        return 0;
    }
    
    /* Find the end of the label */
    while (isalnum((unsigned char)*pos)) {
        pos++;
    }
    
    label_end = pos;
    
    /* Check for unexpected characters after the label */
    pos = skip_whitespace(pos);
    if (*pos != '\0') {
        report_error(line_number, Error_15);/*Extranous text after command*/
        return 0;
    }
    
    /* Check label length */
    if (label_end - label_start >= MAX_LABEL_LEN) {
        report_error(line_number, Error_8);/*Label name is too long*/
        return 0;
    }
    
    /* Extract the label name */
    temp_char = *label_end;
    *label_end = '\0';
    strcpy(label_name, label_start);
    *label_end = temp_char;
    
    /* Check if label name is valid */
    if (!is_valid_label_name(label_name)) {
        report_error(line_number, Error_9);/*Invalid label name*/
        return 0;
    }
    
     
    /* Check if label already exists */
    existing = get_label(label_name); 
    if (existing != NULL) {
        if (existing->type != EXTERN_TYPE) {
            report_error(line_number, Error_7);/*Label name already in label table*/
            return 0;
        }
        
        /* Label already exists as external, no need to create it again */
        return 1;
    }

    
    flag = create_label(label_name, EXTERN_TYPE, 0);
    if (!flag) {
        report_error(line_number, Error_19);/*Failed to create extern*/
        return 0;
    }
    

    return 1;
}

int process_operation(char* line, int* IC) {
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
            report_error(line_number, Error_27);/*Invalid addressing mode*/
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
    
    /* Encode first word (instruction word) */
    instruction_word = (opcode->opcode_number << 18);             /* Opcode: bits 23-18 */
    instruction_word |= (source_addr_mode << 16);  /* Source addressing mode: bits 17-16 */
    instruction_word |= (source_register << 13);   /* Source register: bits 15-13 */
    instruction_word |= (target_addr_mode << 11);  /* Target addressing mode: bits 12-11 */
    instruction_word |= (target_register << 8);    /* Target register: bits 10-8 */
    instruction_word |= (opcode->funct << 3);                 /* Function code: bits 7-3 (0 for now) */
    instruction_word |= 1;                        /* ARE: bits 2-0 (001 = absolute) */
    
    /* Store the instruction word */
    if (!create_binary_code(instruction_word, *IC)) {
        report_error(line_number, Error_28);/*Failed to create binary code*/
        return 0;
    }
    
    /* Process operands and create additional words if needed */
    (*IC)++;
    
    /* Process source operand if not a register and add 1 to IC */
    if (operand_count > 0 && source_addr_mode != ADDR_MODE_REGISTER) {
        if (!encode_operand(operand1, source_addr_mode, IC)) {
            return 0;
        }
    }
    
    /* Process target operand if present and not a register and add 1 to IC */
    if (operand_count > 1 && target_addr_mode != ADDR_MODE_REGISTER) {
        if (!encode_operand(operand2, target_addr_mode, IC)) {
            return 0;
        }
    }
    
    return 1;
}

/* Helper function to determine addressing mode and register number */
int determine_addressing_mode(char* operand, int* addr_mode, int* reg_num) {
    int reg;
    
    /* Check for empty operand */
    if (!operand || !*operand) {
        return 0;
    }
    
    /* Check for immediate addressing (#number) */
    if (operand[0] == '#') {
        *addr_mode = ADDR_MODE_IMMEDIATE;
        *reg_num = 0; 
        return 1;
    }
    
    /* Check for relative addressing (&label) */
    if (operand[0] == '&') {
        *addr_mode = ADDR_MODE_RELATIVE;
        *reg_num = 0;
        return 1;
    }
    
    /* Check for register addressing by checking if its an r followed by a number */
    if (operand[0] == 'r') {
        if (isdigit((unsigned char)operand[1]) && (operand[2] == '\0')) {
            reg = operand[1] - '0';
            if (reg >= 0 && reg <= 7) {
                *addr_mode = ADDR_MODE_REGISTER;
                *reg_num = reg;
                return 1;
            }
            return 0; /* Invalid register */
        }
    }
    
    /* If not any of the above, it's direct addressing */
    *addr_mode = ADDR_MODE_DIRECT;
    *reg_num = 0;
    return 1;
}

/* Helper function to encode operand words */
int encode_operand(char* operand, int addr_mode, int* IC) {
    int operand_word = 0;
    long value;
    char* label;
    Label* label_entry;
    
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
                if (!create_unknown_label(label, *IC, UKNOWN_LABEL_DIRECT, line_number)) {
                    report_error(line_number, Error_28);/*Failed to create binary code*/
                    return 0;
                }
            } else if (label_entry->type == EXTERN_TYPE) {
                if (!create_unknown_label(label, *IC, UKNOWN_LABEL_DIRECT, line_number)) {
                    report_error(line_number, Error_28);/*Failed to create binary code*/
                    return 0;
                }
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
            }
            else if (label_entry->type == EXTERN_TYPE) {
                report_error(line_number, Error_36); /*external label in relative addressing mode*/
                return 0;
            } else {
                /* Calculate distance (will be done in second pass) */
                operand_word = UKNOWN_LABEL_RELATIVE; 
            }
            if (!create_unknown_label(label, *IC, UKNOWN_LABEL_RELATIVE, line_number)) {
                report_error(line_number, Error_28);/*Failed to create binary code*/
                return 0;
            }

            break;
            
        default:
            return 0;
    }
    
    /* Store operand word */
    if (!create_binary_code(operand_word, *IC)) {
        report_error(line_number, Error_28);/*Failed to create binary code*/
        return 0;
    }
    
    (*IC)++;
    return 1;
}

