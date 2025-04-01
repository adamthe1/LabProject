#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/macro_table.h"
#include "../headers/helper.h"

static Macro *head = NULL;

int create_macro(char *name, int line_index)
{
    Macro *new_macro = (Macro *)malloc(sizeof(Macro));

    if (new_macro == NULL)
    {
        /*Memory allocation failed*/
        return 0;
    }
    /*Initilizes the new macro values*/
    new_macro->name = copy_string(name);
    new_macro->line_index = line_index;
    new_macro->code = NULL;
    new_macro->next = NULL;

    /*Adds the new macro to the end of the list*/
    if (head == NULL)
    {
        head = new_macro;
    }
    else
    {
        Macro *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_macro;
    }
    return 1;
}


int add_code_to_macro(char *code)
{
    Macro *current = head;
    if (head == NULL)
    {
        /*No macros in the list*/
        return 0;
    }
    while (current->next != NULL)
    {
        /*Go to the end of the list*/
        current = current->next;
    }
    current->code = (char *)malloc(strlen(code) + 1);
    if (!current->code)
    {
        printf("Memory allocation failed for macro code\n");
        return 0;
    }
    strcpy(current->code, code);
    return 1;
}


char *get_macro_code(char *name)
{
    Macro *current = get_macro(name);
    if (current != NULL)
    {
        return current->code;
    }
    return NULL;
}


Macro *get_macro(char *name)
{
    Macro *current = head;
    while (current != NULL)
    {
        /*Looks for the macro*/
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    /*No macro with this name is in the table*/
    return NULL;
}


void free_macros()
{
    Macro *current = head;
    Macro *next;
    while (current != NULL)
    {
        next = current->next;
        free(current->name);
        free(current->code);
        free(current);
        current = next;
    }
}

void reset_macros()
{
    free_macros();
    head = NULL;
}


void print_macros()
{
    Macro *current = head;

    if (head == NULL)
    {
        printf("No macros defined.\n");
        return;
    }

    printf("Macro Table:\n");
    printf("------------\n");

    while (current != NULL)
    {
        printf("Name: %s\n", current->name);
        printf("Line: %d\n", current->line_index);
        printf("Code: %s\n", current->code ? current->code : "No code defined");
        printf("------------\n");
        current = current->next;
    }
}