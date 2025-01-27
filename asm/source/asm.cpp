#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "asm.h"
#include "data.h"
#include "label.h"

// 1 push, 2 add, 3 sub, 4 mul, 5 div, 6 out, 7 in, 0 hlt

struct Type_of_command
{
    const char *name_command;
    int code_of_command ;
};

Type_of_command massive_of_commands[] =
{
    {"push",  1},
    {"pop" ,  2},
    {"add" ,  3},
    {"sub" ,  4},
    {"mul" ,  5},
    {"div" ,  6},
    {"out" ,  7},
    {"in"  ,  8},
    {"jmp" ,  9},
    {"ja"  , 10},
    {"jb"  , 11},
    {"jae" , 12},
    {"jbe" , 13},
    {"je"  , 14},
    {"jne" , 15},
    {"hlt" ,  0}
};

Type_of_command massive_of_regs[] =
{
    {"ax", 1},
    {"bx", 2},
    {"cx", 3},
    {"dx", 4}
};

int type_of_reg(char *command);
int type_of_command(asm_struct *my_asm);
int arg_push_pop (asm_struct *my_asm, size_t *ip);
int arg_jmp (asm_struct *my_asm, size_t *ip);

int scan_ram_value (asm_struct *my_asm, size_t *ip, int value);
int scan_value     (asm_struct *my_asm, size_t *ip, int value);
int scan_sum_ram   (asm_struct *my_asm, size_t *ip, int value, char *command);
int scan_str_ram   (asm_struct *my_asm, size_t *ip, char *command);
int scan_sum_reg   (asm_struct *my_asm, size_t *ip, int value, char *command);
int scan_str_reg   (asm_struct *my_asm, size_t *ip, char *command);

int skip_spaces    (asm_struct *my_asm);
int skip_until_new_command (asm_struct *my_asm);

bool is_label (asm_struct *my_asm, size_t *ip, char *label);

int skip_until_new_command (asm_struct *my_asm)
{
    while (my_asm->buffer[my_asm->index] != '\n')
            my_asm->index++;
    my_asm->index++;

    return 0;
}
int skip_spaces (asm_struct *my_asm)
{
    assert(my_asm);
    while (my_asm->buffer[my_asm->index] == ' ')
        my_asm->index++;
    return 0;
}

int assembler (asm_struct *my_asm)
{
    assert(my_asm);

    fread(my_asm->buffer, sizeof(char), my_asm->size, my_asm->file_in);
    fseek(my_asm->file_in, 0, SEEK_SET);


    unsigned int amount_of_command = 0;

    while (my_asm->buffer[my_asm->index] != '\0')
    {
        int type = type_of_command(my_asm);
        if (type == ASM_POSSIBLE_LABEL)
        {

            char *label= (char *) calloc(50, sizeof(char));
            if (is_label(my_asm, &amount_of_command, label))
            {
                definition_of_label(my_asm->p_labels, label, &amount_of_command);
                skip_until_new_command(my_asm);
            }
            else
            {
                printf ("Wrong command");
                assert(0);
            }

        }
        else
        {

            my_asm->code[amount_of_command++] = type;

            if (type == PUSH || type == POP)
            {
                if (arg_push_pop(my_asm, &amount_of_command) == ASM_ERROR_WITH_READ)
                    printf("ERROR\n");
            }
            if (type >= JMP && type <= JNE)
            {
                if (arg_jmp(my_asm, &amount_of_command) == ASM_ERROR_WITH_READ)
                    printf("ERROR\n");

            }
            if (type == HLT)
                break;

            skip_until_new_command(my_asm);
        }
    }

    if (fwrite(my_asm->code, sizeof(int), amount_of_command, my_asm->file_out) != amount_of_command)
        printf("Problem with writing\n");

    fill_gaps_in_code(my_asm);

    return 0;
}

int fill_gaps_in_code (asm_struct *my_asm)
{
    assert(my_asm);
    for (size_t i = 0; i < my_asm->p_labels->amount_of_labels; i++)
    {
        if (my_asm->p_labels->label_array[i].is_set)
        {
            for (size_t j = 0; j < my_asm->p_labels->amount_of_fix_up; j++)
                if (i == my_asm->p_labels->fix_up_array[j].index_label)
                {
                    my_asm->code[my_asm->p_labels->fix_up_array[j].ip_use] = my_asm->p_labels->label_array[i].ip;
                }
        }
        else
        {
            printf ("Not recognised label");
            assert(0);
        }

    }
    return 0;

}


int type_of_reg(char *command)
{
    size_t size_of_array = size_reg;

    for (size_t i = 0; i < size_of_array; i++)
    {
        if (strcmp(massive_of_regs[i].name_command, command) == 0)
        {

            return massive_of_regs[i].code_of_command;
        }
    }
    printf("No such command %s\n", command);
    return ASM_ERROR_WITH_READ;
}

int type_of_command (asm_struct *my_asm)
{
    int size_of_array = sizeof(massive_of_commands) / sizeof(massive_of_commands[0]);
    for (int i = 0; i < size_of_array; i++)
    {
        int len = strlen(massive_of_commands[i].name_command);
        if ((strncmp(my_asm->buffer + my_asm->index, massive_of_commands[i].name_command, len)) == 0)
        {
            my_asm->index += len;
            return massive_of_commands[i].code_of_command;
        }
    }

    return ASM_POSSIBLE_LABEL;
}

int arg_jmp (asm_struct *my_asm, size_t *ip)
{
    assert(my_asm);
    assert (ip);

    int value = 0;
    char *label = (char*)calloc(50, sizeof(char));
    if (sscanf (my_asm->buffer + my_asm->index, "%d", &value) == 1)
        my_asm->code[(*ip)++] = value;

    else if (is_label(my_asm, ip, label))
    {
        my_asm->code[(*ip)++] = jmp_at_label(my_asm->p_labels, label, ip);
    }

    else
    {
        printf("Error with command jmp, word after is not a label\n");
        return ASM_UNDEFINED_LABEL;
    }

    return 0;

}

bool is_label (asm_struct *my_asm, size_t *ip, char *label)
{
    assert(my_asm);
    assert(ip);

    if (sscanf (my_asm->buffer + my_asm->index, "%s", label) == 1)
    {
        int i = 0;
        while (label[i] != '\0')
        {
            i++;
        }
        i--;
        return (label[i] == ':');
    }
    return false;
}

int scan_ram_value (asm_struct *my_asm, size_t *ip, int value)
{
    assert(my_asm);
    assert (ip);

    my_asm->code[(*ip)++] = 5;

    my_asm->code[(*ip)++] = value;

    return 0;
}

int scan_value (asm_struct *my_asm, size_t *ip, int value)
{
    assert(my_asm);
    assert (ip);

    my_asm->code[(*ip)++] = 1;
    my_asm->code[(*ip)++] = value;

    return 0;
}

int scan_sum_ram (asm_struct *my_asm, size_t *ip, int value, char *command)
{
    assert(my_asm);
    assert (ip);
    assert(command);

    my_asm->code[(*ip)++] = 7;
    int type = type_of_reg(command);
    if (type < 0)
        return ASM_ERROR_WITH_READ;
    my_asm->code[(*ip)++] = type;
    my_asm->code[(*ip)++] = value;

    return 0;

}

int scan_str_ram (asm_struct *my_asm, size_t *ip, char *command)
{
    assert(my_asm);
    assert (ip);
    assert(command);

    my_asm->code[(*ip)++] = 6;

    int type = type_of_reg(command);

    if (type < 0)
        return ASM_ERROR_WITH_READ;
    my_asm->code[(*ip)++] = type;

    return 0;
}

int scan_sum_reg (asm_struct *my_asm, size_t *ip, int value, char *command)
{
    assert(my_asm);
    assert (ip);
    assert(command);

    my_asm->code[(*ip)++] = 3;
    int type = type_of_reg(command);
    if (type < 0)
        return ASM_ERROR_WITH_READ;

    my_asm->code[(*ip)++] = type;
    my_asm->code[(*ip)++] = value;

    return 0;

}

int scan_str_reg(asm_struct *my_asm, size_t *ip, char *command)
{
    assert(my_asm);
    assert (ip);
    assert(command);

    my_asm->code[(*ip)++] = 2;

    my_asm->code[(*ip)++] = type_of_reg(command);

    return 0;
}

int arg_push_pop (asm_struct *my_asm, size_t *ip)
{
    int value = 0;
    int len = 50;
    char *command= (char *)calloc(len, sizeof(char));
    skip_spaces(my_asm);

    if (sscanf(my_asm->buffer + my_asm->index, "[%d]", &value) == 1)
    {
        scan_ram_value(my_asm, ip, value);
        free(command);
        return ASM_SUCCESS;
    }

    if (sscanf(my_asm->buffer + my_asm->index, "%d", &value) == 1 && my_asm->code[*ip - 1] == 1)
    {
        scan_value(my_asm, ip, value);
        free(command);
        return ASM_SUCCESS;
    }

    int amount_of_read = sscanf(my_asm->buffer + my_asm->index, "[%s + %d]", command, &value);

    if (amount_of_read == 2)
    {

        scan_sum_ram(my_asm, ip, value, command);
        free(command);
        return ASM_SUCCESS;
    }

    if (amount_of_read == 1)
    {
        scan_str_ram(my_asm, ip, command);
        free(command);
        return ASM_SUCCESS;
    }

    amount_of_read = sscanf(my_asm->buffer + my_asm->index, "%s + %d", command, &value);
   if (amount_of_read == 2)
    {

        scan_sum_reg(my_asm, ip, value, command);
        free(command);
        return ASM_SUCCESS;
    }

    if (amount_of_read == 1)
    {
        scan_str_reg(my_asm, ip, command);
        free(command);
        return ASM_SUCCESS;
    }

    return ASM_ERROR_WITH_READ;
}

int asm_ctor (asm_struct *my_asm)
{
    assert(my_asm);


    my_asm->file_in = fopen(my_asm->input_file, "r");  // check
    my_asm->file_out = fopen(my_asm->output_file, "wb");


    if (my_asm->file_in == NULL)
    {
        printf("The input file has't opened");
        return -1;
    }

    if (my_asm->file_out == NULL)
    {
        printf("The output file has't opened");
        assert(fclose (my_asm->file_in) == 0);
        return -1;
    }

    my_asm->size = get_size_of_file(my_asm->input_file);
    my_asm->code = (int *)calloc(my_asm->size, sizeof(int));
    if (!my_asm->code)
    {
        assert (0 && "Memory Allocation");
    }

    my_asm->buffer = (char*)calloc (my_asm->size, sizeof(char));

    if (!my_asm->buffer)
    {
        assert (0 && "Memory Allocation");
    }

    return 0;
}

int asm_dtor (asm_struct *my_asm)
{
    assert(my_asm);

    if (fclose (my_asm->file_in))
    {
        printf ("File_in files hasn't closed\n");
        free(my_asm->code);
        assert(0);
    }

    if (fclose (my_asm->file_out))
    {
        printf ("File_out files hasn't closed\n");
        free(my_asm->code);
        assert(0);
    }

    free(my_asm->buffer);
    free(my_asm->code);

    return 0;
}

// int skip_brackets (char **command)
// {
//     int i = 0, j = 0;
//     printf ("command %s\n", *command);
//     while (*command[i] != '\0')
//     {
//         if (*command[i] != '[' && *command[i] != ']')
//             *command[j++] = *command[i];
//         i++;
//     }
//     *command[j] = '\0';
//     printf ("command %s\n", *command);
//     return 0;
// }
