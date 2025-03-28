#ifndef ANALYZE_H
#define ANALYZE_H

/**
 * Checks if a label name is valid according to assembly rules.
 * A valid label must start with a letter and contain only alphanumeric characters.
 *
 * @param label The label string to validate
 * @return 1 if the label is valid, 0 otherwise
 */
int is_valid_label_name(char *label);

/**
 * Checks if an operand is valid according to assembly rules.
 *
 * @param operand The operand string to validate
 * @return 1 if the operand is valid, 0 otherwise
 */
int is_valid_operand(char *operand);

/**
 * Check if address mode is valid according to assembly rules.
 *
 * @param opcode The opcode to check
 * @param addr_mode The address mode to validate
 * @return 1 if the address mode is valid, 0 otherwise
 */
int is_valid_addressing_mode(Op_code *opcode, int addr_mode, int is_source);

#endif