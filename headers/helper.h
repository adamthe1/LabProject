#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Op_code
{
    char *name;
    int opcode_number;
    int funct;
    int operand_count;
    int addressing_type_src;
    int addressing_type_end;
} Op_code;

extern Op_code op_codes[16];

extern char *instructions[4];

typedef enum BOOL
{
    FALSE = 0,
    TRUE = 1
} BOOL;

/**
 * @brief Gets the opcode structure for a given opcode name.
 *
 * @param name - the name of the opcode.
 * @return Op_code* - pointer to the opcode structure if found, NULL otherwise.
 */
Op_code *get_opcode(char *name);

/**
 * @brief Gets the register number from a register name.
 *
 * @param reg_name - the name of the register (e.g., "$3", "$t0").
 * @return int - the register number if valid, -1 otherwise.
 */
int get_reg(char *reg_name);

/**
 * @brief Gets the instruction name in intstructions.
 * 
 * @param name - the name of the instruction.
 * @return int - the index of the instruction if found, -1 otherwise.
 */

int get_instruction(char *name);

/**
 * @brief Extracts the filename from a given file path.
 *
 * @param file_name - the name of the file without suffix
 * @return char* - the extracted filename.
 */
char *get_file_name(char *file_name);

/**
 * @brief Looks for a file with the given name.
 *
 * @param file_name_suffix - the name of the file.with a suffix
 * @return FILE - the file if found, NULL otherwise.
 */
FILE *look_for_file(char *file_name_suffix);

/**
 * @brief Adds a suffix to a file name.
 *
 * @param file_name - the name of the file.
 * @param suffix - the suffix to add.
 * @return char* - the new file name.
 */
char *add_suffix(char *file_name, char *suffix);

/**
 * @brief Changes the suffix of a file name.
 *
 * @param file_name - the name of the file.
 * @param suffix - the new suffix.
 * @return char* - the new file name.
 */
char *change_suffix(char *file_name, char *suffix);

/**
 * @brief This function chaecks if the macro name is a name of a known word of assembly (operation/instruction/register).
 *
 * @param name - the name of the macro.
 * @return int - returns 1 if the macro name is valid and 0 if it isn't.
 */
int cmp_mcro_name(char *name);

/**
 * @brief This function checks if the mcro name starts with a letter and contains alphanumeric characters or _
    and that it is no longer than 31 chars.
 *
 * @param name - the name of the macro.
 * @return int - returns 1 if the macro name is valid and 0 if it isn't.
 */
int mcro_letters(char *name);

/**
 * @brief This function removes extra white chars from s.
 *
 * @param s - the string to remove extra white chars from.
 * @return void.
 */
void extra_spaces(char *s);

/**
 * @brief Skips whitespace characters in a string.
 *
 * @param str - the string to skip whitespace in.
 * @return char* - the pointer to the first non-whitespace character.
 */
char *skip_whitespace(char *str);

/**
 * @brief This function converts a number to binary with 2s complement.
 * @brief The number of bits is given in DEFINE.h.
 * @param binary - the array to store the binary number.
 * @return int - returns 1 if the conversion was successful and 0 if it wasn't.
 */
int dec_to_binary(int num, int *binary);

/**
 * @brief This function copys the string and returns a pointer to the new string.
 *
 * @param s - the string to copy.
 *
 */

char *copy_string(char *s);

/**
 * @brief  function to trim whitespace from end of string
 *
 * @param s - the string to trim.
 */
void trim_whitespace(char *s);

/**
 * @brief free every table in the project
 */

void free_all();

/**
 * @brief Resets all the tables for a new file.
 */

 void reset_all();

#endif /* HELPER_H */