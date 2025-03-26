/**
 * @file mcro_table.h
 * @brief This file contains the of structure and functions for handling the labels.
 * @brief This file also store the binary code of all the data and code.
 */

 #ifndef _LABEL_DATA_STRUCT_H
 #define _LABEL_DATA_STRUCT_H
 
 /* labels are nodes in a linked list*/
 typedef struct Label{
     char* name;
     char type;
     int is_entry;
     int line_index; /* linbe index is IC or DC*/
     struct Label* next;
 } Label;
 
 /**
  * @brief This function creates a new label node and adds it to the list.
  * 
  * @param name - the name of the label.
  * @param type - the type of the label ('c' = code, 'd' = data)
  * @param line_index - the index of the line in the file.
  * @return int - 1 if the label was created successfully, 0 otherwise.
  */
 int create_label(char* name, char type ,int line_index);
 
 /**
  * @brief This function returns the label with the given name.
  * 
  * @param name - the name of the label.
  * @return label* - the label.
  */
 Label* get_label(char* name);
 
 /**
  * @brief This function frees the memory allocated for the labels list.
  * 
  */

Label* get_label_head();

int add_entry(char* label_name);
 
void free_labels();

typedef struct Data_binary{
    int binary;
    int DC_index;
    struct Data_binary* next;
} Data_binary;
 
/**
 * @brief This function creates a new data node and adds it to the list.
 *  
 * @param binary - the binary code of the data.
 * @param line_index - the index of the line in the file.
 * @return int - 1 if the data was created successfully, 0 otherwise.
 */

int create_binary_data(int binary, int DC_index);

/**
 * @brief This function returns the data with the given line index.
 * 
 * @param line_index - the index of the line in the file.
 * @return Data_binary* - the data.
 */

Data_binary* get_binary_data(int DC_index);

/**
 * @brief This function frees the memory allocated for the data list.
 * 
 */

void free_binary_data();

typedef struct Code_binary{
    int binary;
    int IC_index;
    struct Code_binary* next;
} Code_binary;
 
/**
 * @brief This function creates a new data node and adds it to the list.
 *  
 * @param binary - the binary code of the data.
 * @param IC_index - the index of IC in the file.
 * @return int - 1 if the data was created successfully, 0 otherwise.
 */

int create_binary_code(int binary, int IC_index);

/**
 * @brief This function returns the data with the given IC index.
 * 
 * @param line_index - the index of the line in the file.
 * @return Code_binary* - the data.
 */

Code_binary* get_binary_code(int IC_index);

/**
 * @brief This function frees the memory allocated for the data list.
 * 
 */

void free_binary_code();

/**
 * @brief This function adds the IC to the DC.
 * 
 * @param IC_index - the index of IC in the file.
 * @return int - 1 if the data was created successfully, 0 otherwise.
 */

int add_IC_to_DC(int IC_index);

typedef struct Unknown_label{
    char* name;
    int IC_index;
    int type;
    int line_number;
    int command_IC;
    struct Unknown_label* next;
} Unknown_label;

/**
 * @brief This function creates a new unknown label node and adds it to the list.
 *  
 * @param name - the name of the label.
 * @param IC_index - the index of IC in the file.
 * @param type - the type of the unknown label (direct or relative).
 * @return int - 1 if the data was created successfully, 0 otherwise.
 */

int create_unknown_label(char* name, int IC_index, int type, int line_number, int command_IC);

/**
 * @brief This function returns the unknown label with the given name.
 * 
 * @param name - the name of the label.
 * @return Unknown_label* - the label.
 */

Unknown_label* get_unknown_label(char* name);

/**
 * @brief This function frees the memory allocated for the unknown label list.
 * 
 */

void free_unknown_labels();

/**
 * @brief This function returns the head of Unknown_label.
 * 
 */

Unknown_label* get_unknown_head();


void print_label_table();
void print_data_table();
void print_code_table();
void print_unknown_table();

 #endif