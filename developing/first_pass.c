#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/define.h"
#include "../headers/first_pass.h"
#include "../headers/label_table.h"
#include "../headers/errors.h"

/* Global variables */
static char line[MAX_LINE_LENGTH + 1]; /* +1 for possible \n */
static int line_number = 0;
static int error_found = 0;
static char error_buffer[256]; /* Buffer for error messages */

/* Operation codes and their allowed operand counts */
typedef struct {
    char* name;
    int operand_count;
} OpCode;

static const OpCode opcodes[] = {
    {"mov", 2}, {"cmp", 2}, {"add", 2}, {"sub", 2},
    {"not", 1}, {"clr", 1}, {"lea", 2}, {"inc", 1}, 
    {"dec", 1}, {"jmp", 1}, {"bne", 1}, {"red", 1},
    {"prn", 1}, {"jsr", 1}, {"rts", 0}, {"stop", 0}
};

#define NUM_OPCODES (sizeof(opcodes) / sizeof(OpCode))

/* Helper functions */


int is_valid_label_name(char* label) {
    int i;
    size_t len;
    
    /* Must start with a letter */
    if (!isalpha((unsigned char)label[0])) {
        return 0;
    }
    
    /* Check length */
    len = strlen(label);
    if (len > MAX_LABEL_LEN) {
        return 0;
    }
    
    /* Only alphanumeric characters allowed */
    for (i = 1; i < len; i++) {
        if (!isalnum((unsigned char)label[i])) {
            return 0;
        }
    }
    
    return 1;
}

int parse_label(char* line, char* label_name) {
    char* colon_pos;
    int label_len;
    
    line = skip_whitespace(line);
    
    /* If line starts with semicolon or is empty, no label */
    if (*line == ';' || *line == '\0' || *line == '\n') {
        return 0;
    }
    
    colon_pos = strchr(line, ':');
    if (colon_pos == NULL) {
        return 0;
    }
    
    /* Extract the label name */
    label_len = colon_pos - line;
    if (label_len >= MAX_LABEL_LEN) {
        report_error(line_number, Error_8);
    }
    
    strncpy(label_name, line, label_len);
    label_name[label_len] = '\0';
    
    /* Validate label name */
    if (!is_valid_label_name(label_name)) {
        report_error(line_number, Error_9);
        return 0;
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
        if (strncmp(line, ".data", 5) == 0) {
            return INST_TYPE_DATA;
        } else if (strncmp(line, ".string", 7) == 0) {
            return INST_TYPE_STRING;
        } else if (strncmp(line, ".entry", 6) == 0) {
            return INST_TYPE_ENTRY;
        } else if (strncmp(line, ".extern", 7) == 0) {
            return INST_TYPE_EXTERN;
        }
    }
    
    /* If not a directive, it's an operation */
    return INST_TYPE_OPERATION;
}

int process_data_directive(char* line, int* DC) {
    char* pos;
    int value;
    char* token;
    char* endptr;
    
    /* Skip .data directive */
    pos = strstr(line, ".data");
    if (pos == NULL) {
        report_error(line_number, Error_10);
        return 0;
    }
    
    pos += 5;  /* Skip ".data" */
    pos = skip_whitespace(pos);
    
    /* Parse comma-separated values */
    token = strtok(pos, ",");
    while (token != NULL) {
        token = skip_whitespace(token);
        
        /* Handle empty token (e.g., "5, , 10") */
        if (*token == '\0' || *token == '\n') {
            report_error(line_number, Error_11);
            return 0;
        }
        
        /* Convert to integer */
        value = (int)strtol(token, &endptr, 10);
        if (*endptr != '\0' && !isspace((unsigned char)*endptr)) {
            report_error(line_number, Error_12);
            return 0;
        }
        
        /* Here you would store the value in your data array */
        /* data_array[*DC] = value; */
        
        (*DC)++;
        token = strtok(NULL, ",");
    }
    
    return 1;
}

int process_string_directive(char* line, int* DC) {
    char* pos;
    char* start_quote;
    char* end_quote;
    
    /* Skip .string directive */
    pos = strstr(line, ".string");
    if (pos == NULL) {
        report_error(line_number, Error_13);
        return 0;
    }
    
    pos += 7;  /* Skip ".string" */
    pos = skip_whitespace(pos);
    
    /* Find start and end quotes */
    start_quote = strchr(pos, '\"');
    if (start_quote == NULL) {
        report_error(line_number, Error_14);
        return 0;
    }
    
    end_quote = strchr(start_quote + 1, '\"');
    if (end_quote == NULL) {
        report_error(line_number, Error_14);
        return 0;
    }
    
    /* Extract and process the string */
    *end_quote = '\0';
    start_quote++;  /* Skip the opening quote */
    
    /* Store each character plus null terminator */
    while (*start_quote != '\0') {
        /* Here you would store the character in your data array */
        /* data_array[*DC] = *start_quote; */
        (*DC)++;
        start_quote++;
    }
    
    /* Add null terminator */
    /* data_array[*DC] = '\0'; */
    (*DC)++;
    
    return 1;
}

int process_entry_directive(char* line) {
    char* pos;
    char label_name[MAX_LABEL_LEN];
    int i = 0;
    
    /* Skip .entry directive */
    pos = strstr(line, ".entry");
    if (pos == NULL) {
        report_error(line_number, ERR_SYNTAX, "Malformed .entry directive");
        return 0;
    }
    
    pos += 6;  /* Skip ".entry" */
    pos = skip_whitespace(pos);
    
    /* Extract label name */
    while (*pos && !isspace((unsigned char)*pos) && i < MAX_LABEL_LEN - 1) {
        label_name[i++] = *pos++;
    }
    label_name[i] = '\0';
    
    /* Validation will be done in second pass */
    /* For now, just store that this label should be an entry */
    
    return 1;
}

int process_extern_directive(char* line) {
    char* pos;
    char label_name[MAX_LABEL_LEN];
    int i = 0;
    
    /* Skip .extern directive */
    pos = strstr(line, ".extern");
    if (pos == NULL) {
        report_error(line_number, ERR_SYNTAX, "Malformed .extern directive");
        return 0;
    }
    
    pos += 7;  /* Skip ".extern" */
    pos = skip_whitespace(pos);
    
    /* Extract label name */
    while (*pos && !isspace((unsigned char)*pos) && i < MAX_LABEL_LEN - 1) {
        label_name[i++] = *pos++;
    }
    label_name[i] = '\0';
    
    /* Check if label name is valid */
    if (!is_valid_label_name(label_name)) {
        report_error(line_number, ERR_LABEL, "Invalid external label name");
        return 0;
    }
    
    /* Add to label table as external */
    if (!create_label(strdup(label_name), 'e', 0)) {
        report_error(line_number, ERR_LABEL, "Failed to create external label");
        return 0;
    }
    
    return 1;
}

int process_operation(char* line, int* IC) {
    char* pos;
    char operation[MAX_LABEL_LEN];
    int i, found = 0;
    int op_size = 0;
    
    /* Skip label if present */
    pos = strchr(line, ':');
    if (pos != NULL) {
        line = pos + 1;
    }
    
    line = skip_whitespace(line);
    
    /* Extract operation name */
    i = 0;
    while (*line && !isspace((unsigned char)*line) && i < MAX_LABEL_LEN - 1) {
        operation[i++] = *line++;
    }
    operation[i] = '\0';
    
    /* Check if it's a valid operation */
    for (i = 0; i < NUM_OPCODES; i++) {
        if (strcmp(operation, opcodes[i].name) == 0) {
            found = 1;
            op_size = 1;  /* Base size for the operation */
            
            /* Additional words for operands would be calculated here */
            /* This depends on addressing modes and operand types */
            op_size += opcodes[i].operand_count;
            
            break;
        }
    }
    
    if (!found) {
        report_error(line_number, ERR_SYNTAX, "Unknown operation");
        return 0;
    }
    
    /* Here you would encode the operation and operands */
    
    /* Update instruction counter */
    (*IC) += op_size;
    
    return 1;
}

int first_pass(FILE* file) {
    int DC = 0;
    int IC = 100;
    int symbol_flag = 0;
    char label_name[MAX_LABEL_LEN];
    int inst_type;
    char* pos;
    
    line_number = 0;
    error_found = 0;
    
    /* Reset file pointer to beginning */
    rewind(file);
    
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
        
        /* Check for label */
        symbol_flag = parse_label(line, label_name);
        
        /* Identify instruction type */
        inst_type = identify_instruction(line);
        
        /* Process based on instruction type */
        switch (inst_type) {
            case INST_TYPE_DATA:
                if (symbol_flag) {
                    /* Add label to symbol table as data */
                    if (!create_label(strdup(label_name), 'd', DC)) {
                        report_error(line_number, ERR_LABEL, "Failed to create data label");
                        return 0;
                    }
                }
                if (!process_data_directive(line, &DC)) {
                    return 0;
                }
                break;
                
            case INST_TYPE_STRING:
                if (symbol_flag) {
                    /* Add label to symbol table as data */
                    if (!create_label(strdup(label_name), 'd', DC)) {
                        report_error(line_number, ERR_LABEL, "Failed to create string label");
                        return 0;
                    }
                }
                if (!process_string_directive(line, &DC)) {
                    return 0;
                }
                break;
                
            case INST_TYPE_ENTRY:
                if (!process_entry_directive(line)) {
                    return 0;
                }
                break;
                
            case INST_TYPE_EXTERN:
                if (!process_extern_directive(line)) {
                    return 0;
                }
                break;
                
            case INST_TYPE_OPERATION:
                if (symbol_flag) {
                    /* Add label to symbol table as code */
                    if (!create_label(strdup(label_name), 'c', IC)) {
                        report_error(line_number, ERR_LABEL, "Failed to create code label");
                        return 0;
                    }
                }
                if (!process_operation(line, &IC)) {
                    return 0;
                }
                break;
                
            default:
                report_error(line_number, ERR_SYNTAX, "Unknown instruction type");
                return 0;
        }
    }
    
    /* Update addresses for data labels to come after code */
    /* This would involve traversing your label table and updating addresses */
    
    return !error_found;
}