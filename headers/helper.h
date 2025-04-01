#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
* @brief struct that includes all information that needs to encode the operation.
*
* @param name - The operation name
* @param opcode_number - The number of the operation
* @param funct - The funct number of the operation
* @param operand_count - The number of operands for the operation
* @param addressing_type_src - The addressing type of source operand
* @param addressing_type_end - The addressing type of destination operand
*/
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
 * @param name - The name of the opcode.
 * @return Op_code* - pointer to the opcode structure if found, NULL otherwise.
 */
Op_code *get_opcode(char *name);

/**
 * @brief Gets the register number from a register name.
 *
 * @param reg_name - The name of the register (e.g., "$3", "$t0").
 * @return int - 1 if valid, 0 otherwise.
 */
int get_reg(char *reg_name);

/**
 * @brief Gets the instruction name in intstructions.
 * 
 * @param name - The name of the instruction.
 * @return int - 1 if instruction name is valid, 0 otherwise.
 */
int get_instruction(char *name);

/**
 * @brief Extracts the filename from a given file path.
 *
 * @param file_name - The name of the file without suffix
 * @return char* - the extracted filename.
 */
char *get_file_name(char *file_name);

/**
 * @brief Looks for a file with the given name.
 *
 * @param file_name_suffix - The name of the file.with a suffix
 * @return FILE - the file if found, NULL otherwise.
 */
FILE *look_for_file(char *file_name_suffix);

/**
 * @brief Adds a suffix to a file name.
 *
 * @param file_name - The name of the file.
 * @param suffix - The suffix to add.
 * @return char* - The new file name.
 */
char *add_suffix(char *file_name, char *suffix);

/**
 * @brief Changes the suffix of a file name.
 *
 * @param file_name - The name of the file.
 * @param suffix - The new suffix.
 * @return char* - The new file name.
 */
char *change_suffix(char *file_name, char *suffix);

/**
 * @brief Chaecks if the macro name is a name of a known word of assembly (operation/instruction/register).
 *
 * @param name - The name of the macro.
 * @return int - 1 if the macro name is valid and 0 if it isn't.
 */
int cmp_mcro_name(char *name);

/**
 * @brief Checks if the mcro name starts with a letter, contains alphanumeric characters or _
    and that it is no longer than 31 chars.
 *
 * @param name - The name of the macro.
 * @return int - 1 if the macro name is valid and 0 if it isn't.
 */
int mcro_letters(char *name);


/**
 * @brief Skips whitespace characters in a string.
 *
 * @param str - The string to skip whitespace in.
 * @return char* - The pointer to the first non-whitespace character.
 */
char *skip_whitespace(char *str);

/**
 * @brief This function converts a number to binary with 2s complement.
 * @brief The number of bits is given in DEFINE.h.
 * @param binary - The array to store the binary number.
 * @return int - 1 if the conversion was successful and 0 if it wasn't.
 */
int dec_to_binary(int num, int *binary);

/**
 * @brief Copys the string and returns a pointer to the new string.
 *
 * @param s - The string to copy.
 *
 */
char *copy_string(char *s);

/**
 * @brief  Trim whitespace from end of string
 *
 * @param s - The string to trim.
 */
void trim_whitespace(char *s);

/**
 * @brief Free every table in the project
 */
void free_all();

/**
 * @brief Resets all the tables for a new file.
 */
 void reset_all();

#endif /* HELPER_H */