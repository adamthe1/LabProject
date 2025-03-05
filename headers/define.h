#ifndef _DEFINE_H
#define _DEFINE_H


#define MAX_LINE_LENGTH 80
#define COMMENT_CHAR ';'
#define LABEL_CHAR ':'
#define STRING_CHAR '"'
#define TWO 2
#define CODE_TYPE 'c'
#define DATA_TYPE 'd'

/* Memory size limits */
#define MEMORY_SIZE 4096
#define FIRST_ADDRESS 100

/* Maximum sizes */
#define MAX_LINE_LENGTH 80
#define MAX_LABEL_LEN 31
#define MAX_OPERATION_LEN 5
#define MAX_OPERANDS 2
#define MAX_OPERAND_LEN 50

/* Addressing modes */
#define ADDR_MODE_IMMEDIATE 0
#define ADDR_MODE_DIRECT 1
#define ADDR_MODE_RELATIVE 2
#define ADDR_MODE_REGISTER 3

/* Error codes for first pass */
#define ERR_NONE 0
#define ERR_SYNTAX 1
#define ERR_LABEL 2
#define ERR_DIRECTIVE 3
#define ERR_OPERAND 4
#define ERR_FILE 5

/* Maximum values */
#define MAX_LABEL_LEN 31
#define MAX_LINE_LENGTH 80

/* Instruction types */
#define INST_TYPE_OPERATION 1
#define INST_TYPE_DATA 2
#define INST_TYPE_STRING 3
#define INST_TYPE_ENTRY 4
#define INST_TYPE_EXTERN 5

#endif /* _DEFINE_H */
