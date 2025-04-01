/**
 * @file mcro_table.h
 * @brief This file contains the of structure and functions for handling the labels.
 * @brief This file also store the binary code of all the data and code.
 */

#ifndef _LABEL_DATA_STRUCT_H
#define _LABEL_DATA_STRUCT_H

/* labels are nodes in a linked list*/
/**
 * @brief struct of label and its information
 * 
 * @param name The name of the label
 * @param type Type of label (data, string etc.)
 * @param is_entry Flag to know if the kabel is entry
 * @param line_index Line index is IC or DC
 * @param next The next label in the list (or null)
 */
typedef struct Label
{
    char *name;
    char type;
    int is_entry;
    int line_index; /* line index is IC or DC*/
    struct Label *next;
} Label;

/**
 * @brief This function creates a new label node and adds it to the list.
 *
 * @param name - the name of the label.
 * @param type - the type of the label ('c' = code, 'd' = data)
 * @param line_index - the index of the line in the file.
 * @return int - 1 if the label was created successfully, 0 otherwise.
 */
int create_label(char *name, char type, int line_index);

/**
 * @brief Returns the label with the given name.
 *
 * @param name - The name of the label.
 * @return label* - The label.
 */
Label *get_label(char *name);

/**
 * @brief Returns the pointer to head of label list.
 *
 * @return label* - The head of label list.
 */
Label *get_label_head();

/**
 * @brief Marks the label as an entry label.
 *
 * @param name - The name of the label to mark.
 * @return int - 1 if the function finished successfully, 0 otherwise.
 */
int add_entry(char *label_name);

/**
 * @brief Frees the memory allocated for the labels list.
 *
 */
void free_labels();

/**
 * @brief struct of data_binary
 * 
 * @param binary The data binary
 * @param Dc_index The DC index of the data binary
 * @param next The next data binary in the list
 */
typedef struct Data_binary
{
    int binary;
    int DC_index;
    struct Data_binary *next;
} Data_binary;

/**
 * @brief Creates a new data node and adds it to the list.
 *
 * @param binary - The binary code of the data.
 * @param line_index - The index of the line in the file.
 * @return int - 1 if the data was created successfully, 0 otherwise.
 */
int create_binary_data(int binary, int DC_index);

/**
 * @brief Returns the data with the given line index.
 *
 * @param line_index - The index of the line in the file.
 * @return Data_binary* - The data.
 */
Data_binary *get_binary_data(int DC_index);

/**
 * @brief This function frees the memory allocated for the data list.
 *
 */
void free_binary_data();

/**
 * @brief struct of code_binary
 * 
 * @param binary The code binary
 * @param IC_index The IC index of the code binary
 * @param next The next code binary in the list
 */
typedef struct Code_binary
{
    int binary;
    int IC_index;
    struct Code_binary *next;
} Code_binary;

/**
 * @brief Creates a new data node and adds it to the list.
 *
 * @param binary - The binary code of the data.
 * @param IC_index - The index of IC in the file.
 * @return int - 1 if the data was created successfully, 0 otherwise.
 */
int create_binary_code(int binary, int IC_index);

/**
 * @brief Returns the data with the given IC index.
 *
 * @param line_index - The index of the line in the file.
 * @return Code_binary* - The data.
 */
Code_binary *get_binary_code(int IC_index);

/**
 * @brief Frees the memory allocated for the data list.
 *
 */
void free_binary_code();

/**
 * @brief Adds the IC to the DC.
 *
 * @param IC_index - The index of IC in the file.
 * @return int - 1 if the data was created successfully, 0 otherwise.
 */
int add_IC_to_DC(int IC_index);

/* labels are nodes in a linked list*/
/**
 * @brief struct of unknown label and its information
 * 
 * @param name The name of the label
 * @param IC_index The IC index of the label
 * @param type Type of label (data, string etc.)
 * @param line_number Line number is IC or DC
 * @param command_IC The command IC of the label
 * @param next The next unknwon label in the list (or null)
 */
typedef struct Unknown_label
{
    char *name;
    int IC_index;
    int type;
    int line_number;
    int command_IC;
    struct Unknown_label *next;
} Unknown_label;

/**
 * @brief Creates a new unknown label node and adds it to the list.
 *
 * @param name - The name of the label.
 * @param IC_index - The index of IC in the file.
 * @param type - The type of the unknown label (direct or relative).
 * @return int - 1 if the data was created successfully, 0 otherwise.
 */
int create_unknown_label(char *name, int IC_index, int type, int line_number, int command_IC);

/**
 * @brief Returns the unknown label with the given name.
 *
 * @param name - The name of the label.
 * @return Unknown_label* - The label.
 */
Unknown_label *get_unknown_label(char *name);

/**
 * @brief Frees the memory allocated for the unknown label list.
 *
 */
void free_unknown_labels();

/**
 * @brief Returns the head of Unknown_label.
 *
 */
Unknown_label *get_unknown_head();

/**
 * @brief Resets all the tables for a new file process.
 * 
 */
void reset_all_tables();

/**
 * @brief Prints all labels in the label table.
 * 
 */
void print_label_table();
void print_data_table();
void print_code_table();
void print_unknown_table();

#endif