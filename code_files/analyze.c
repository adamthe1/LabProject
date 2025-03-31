#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/helper.h"
#include "../headers/define.h"
#include "../headers/label_data_struct.h"

int is_valid_label_name(char *label)
{
    int i;
    int len;
    len = strlen(label);

    /* Must start with a letter */
    if (!isalpha((unsigned char)label[0]))
    {
        return 0;
    }

    /* Only alphanumeric characters allowed */
    for (i = 1; i < len; i++)
    {
        if (!isalnum((unsigned char)label[i]))
        {
            return 0;
        }
    }

    return 1;
}

int is_valid_operand(char *operand)
{
    int i;
    int len;
    len = strlen(operand);
    if (len == 0)
    {
        return 0;
    }
    /* if starts with '#' must be a number can start with +, - */
    if (operand[0] == '#')
    {
        if (len == 1)
        {
            return 0;
        }

        if (len == 2 && !isdigit((unsigned char)operand[1]))
        {
            return 0;
        }

        if (len > 2 && (operand[1] == '+' || operand[1] == '-'))
        {
            for (i = 2; i < len; i++)
            {
                if (!isdigit((unsigned char)operand[i]))
                {
                    return 0;
                }
            }
            return 1;
        }

        for (i = 1; i < len; i++)
        {
            if (!isdigit((unsigned char)operand[i]))
            {
                return 0;
            }
        }
        return 1;
    }

    /* Must start with a letter or '&' */
    if (!isalpha((unsigned char)operand[0]) && operand[0] != '&')
    {
        return 0;
    }

    if (operand[0] == '&')
    {
        if (len == 1)
        {
            return 0;
        }

        if (len == 2 && !isalpha((unsigned char)operand[1]))
        {
            return 0;
        }
    }

    /* Only alphanumeric characters allowed */
    for (i = 1; i < len; i++)
    {
        if (!isalnum((unsigned char)operand[i]))
        {
            return 0;
        }
    }

    return 1;
}

/* Check if an addressing mode is valid for the given opcode */
int is_valid_addressing_mode(Op_code *opcode, int addr_mode, int is_source)
{
    int mode_bit = 1 << addr_mode;

    if (is_source)
    {
        return (opcode->addressing_type_src & mode_bit) != 0;
    }
    else
    {
        return (opcode->addressing_type_end & mode_bit) != 0;
    }
}