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

/*
Algorithem steps:
1. Read the next line from the source file. If the end of the source file is reached, proceed to step 7.
2. If the first field in the line is a symbol (label), skip it.
2. Is this a data, string, or extern directive? If so, go back to step 1.
3. Is this an entry directive? If not, proceed to step 6.
4. Add the entry attribute to the symbol’s properties in the symbol table,
   corresponding to the symbol appearing as the operand of the directive.
5. Complete the binary encoding of the operands' data words according to the addressing methods
   (if the symbol is not found in the symbol table, report an error). Return to step 1.
6. For each operand in the source code that contains a symbol,
   find the symbol’s value in the symbol table. If the symbol is defined as external,
   add the address of the relevant data word to the list of data words referring to the external symbol.
   If the symbol is not found in the table, report an error.
7. The source file has been fully read. If errors were found in the second pass, stop here.
8. Build the output files (more details to follow).
*/

/* Global variables */
static char line[MAX_LINE_LENGTH + 1]; /* +1 for possible \n */
static int line_number = 0;
static int error_found = 0;

int second_pass(char *file_name, int *IC, int *DC, int error_found_first)
{
    char label_name[MAX_LABEL_LEN];
    int inst_type, i;
    char *pos;
    char *colon_pos;
    char *label_end;
    char *ob_name, *ent_name, *ext_name;
    FILE *file, *fp_ob, *fp_ent;
    Label *current;
    ob_name = change_suffix(file_name, ".ob");
    ent_name = change_suffix(file_name, ".ent");
    ext_name = change_suffix(file_name, ".ext");

    fp_ob = fopen(ob_name, "w"); /*add .ob to the file name*/
    if (!fp_ob)
    {
        report_error(line_number, Error_1); /*can't open file*/
        return 0;
    }
    fp_ent = fopen(ent_name, "w"); /*add .ent to the file name*/
    if (!fp_ent)
    {
        report_error(line_number, Error_1); /*can't open file*/
        return 0;
    }

    file = fopen(file_name, "r");
    line_number = 0;
    error_found = error_found_first;

    /* Reset file pointer to beginning */

    rewind(file);
    /*Step 1*/
    while (fgets(line, sizeof(line), file))
    {
        line_number++;
        /* Remove newline character if present */
        pos = strchr(line, '\n');
        if (pos)
            *pos = '\0';

        /* Skip empty lines and comments */
        pos = skip_whitespace(line);

        if (*pos == '\0' || *pos == ';')
        {
            continue;
        }

        /*Step 2 - If(the first field in the line is a symbol (label)) - skip it*/
        
        colon_pos = strchr(pos, ':');
        if (colon_pos != NULL)
        {
            label_end = colon_pos + 1;
            memmove(pos, label_end, strlen(label_end) + 1);
        }
        pos = skip_whitespace(pos);

        /*Step 3*/

        inst_type = identify_instruction(pos);
        if (inst_type == INST_TYPE_DATA || inst_type == INST_TYPE_STRING || inst_type == INST_TYPE_EXTERN)
            continue;

        /*Step 4*/

        if (inst_type == INST_TYPE_ENTRY)
        {
            /* make label_name new again*/
            memset(label_name, 0, MAX_LABEL_LEN);

            /*Steps 5*/
            if (strncmp(pos, ".entry", 6) == 0)
            {
                pos += 6;
            }
            pos = skip_whitespace(pos);

            i = 0;
            while (*pos && !isspace((unsigned char)*pos))
            {
                label_name[i++] = *pos++;
            }
            label_name[i] = '\0';

            if (!is_valid_label_name(label_name))
            {
                report_error(line_number, Error_9); /*Invalid label name*/
                error_found = 1;
                continue;
            }

            current = get_label(label_name);
            if (current == NULL)
            {
                report_error(line_number, Error_34); /*Label name doesn't exist in the label table*/
                error_found = 1;
                continue;
            }
            if (current->type == EXTERN_TYPE)
            {
                report_error(line_number, Error_43); /*Label name doesn't exist in the label table*/
                error_found = 1;
                continue;
            }
            current = NULL;

            add_entry(label_name);/*Marks the label as entry*/

            continue;
        }
    }

    if (!process_unknown(ext_name))
    {
        error_found = 1;
    }

    /*Step 7*/
    fclose(file);

    if (error_found)
    { /*if there is at least 1 error in the file found in the second pass*/
        printf("Errors were found during the pass in the file, assembler  process can't be completed\n");
        /* delete all files*/
        remove(ob_name);
        remove(ext_name);
        remove(ent_name);
        free(ob_name);
        free(ext_name);
        free(ent_name);
        fclose(fp_ob);
        fclose(fp_ent);
        return 0;
    }
    /*Step 8 - Bulid the output files*/

    /*Prints the ob file*/
    fprintf(fp_ob, "%d %d\n", *IC - 100, *DC - *IC); /*Prints the file headline*/

    for (i = 100; i < *IC; i++)
    {
        fprintf(fp_ob, "%07d %06x\n", i, get_binary_code(i)->binary & 0xFFFFFF);
    }
    for (i = *IC; i < *DC; i++)
    {
        fprintf(fp_ob, "%07d %06x\n", i, get_binary_data(i)->binary & 0xFFFFFF);
    }

    /*Prints the ent file*/
    current = get_label_head();
    while (current != NULL)
    {
        if (current->is_entry)
        {
            fprintf(fp_ent, "%s %07d\n", current->name, current->line_index);
        }
        current = current->next;
    }

    fclose(fp_ob);
    fclose(fp_ent);
    free(ent_name);
    free(ext_name);
    free(ob_name);
    return 1;
}

int process_unknown(char *external_file)
{
    Unknown_label *current = get_unknown_head();
    Code_binary *code;
    Label *label;
    FILE *fp_ext = fopen(external_file, "w");
    while (current != NULL)
    {
        if (current->type == UKNOWN_LABEL_DIRECT)
        {
            label = get_label(current->name);
            if (label == NULL)
            {
                report_error(current->line_number, Error_34); /*Label name doesn't exist in the label table*/
                error_found = 1;
                current = current->next;
                continue;
            }
            if (label->type == EXTERN_TYPE)
            {
                code = get_binary_code(current->IC_index);
                if (code == NULL)
                {
                    report_error(current->line_number, Error_37); /*Failed to create binary code*/
                    error_found = 1;
                    current = current->next;
                    continue;
                }
                code->binary = 0;  /* External address */
                code->binary |= 1; /* ARE = 001 (external) */
                fprintf(fp_ext, "%s %07d\n", current->name, current->IC_index);
            }
            else
            {
                code = get_binary_code(current->IC_index);
                if (code == NULL)
                {
                    report_error(current->line_number, Error_37); /*Failed to create binary code*/
                    error_found = 1;
                    current = current->next;
                    continue;
                }
                code->binary = (label->line_index & 0x1FFFFF) << 3; /* 21-bit address */
                code->binary |= 2;                                  /* ARE = 010 (relocatable) */
            }
        }
        else if (current->type == UKNOWN_LABEL_RELATIVE)
        {
            label = get_label(current->name);
            if (label == NULL)
            {
                report_error(current->line_number, Error_34); /*Label name doesn't exist in the label table*/
                error_found = 1;
                current = current->next;
                continue;
            }
            code = get_binary_code(current->IC_index);
            if (code == NULL)
            {
                report_error(current->line_number, Error_37); /*Failed to create binary code*/
                error_found = 1;
                current = current->next;
                continue;
            }
            if (label->type == EXTERN_TYPE)
            {
                report_error(current->line_number, Error_36); /*external label in relative addressing mode*/
                error_found = 1;
                current = current->next;
                continue;
            }
            if (label->type == DATA_TYPE)
            {
                report_error(current->line_number, Error_42); /*external label in relative addressing mode*/
                error_found = 1;
                current = current->next;
                continue;
            }
            else
            {
                code->binary = label->line_index - current->command_IC;
                code->binary <<= 3; /* 21-bit offset */
                code->binary |= 4;  /* ARE = 100 (absolute) */
            }
        }
        current = current->next;
    }
    fclose(fp_ext);
    return 1;
}