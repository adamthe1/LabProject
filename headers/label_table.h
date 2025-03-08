/**
 * @file mcro_table.h
 * @brief This file contains the  of structure and functions for handling the labels.
 */

 #ifndef _MCRO_TABLE_H
 #define _MCRO_TABLE_H
 
 /* labels are nodes in a linked list*/
 typedef struct Label{
     char* name;
     char type;
     int line_index;
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
  * @brief This function adds code lines to the last label in the list.
  * 
  * @param code - the code line to add.
  * @return int - 1 if the code was added successfully, 0 otherwise.
  */

 char* get_label_code(char* name);
 
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
 
 void free_labels();
 
 #endif // _MCRO_TABLE_H