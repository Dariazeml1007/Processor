#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "asm.h"
#include "data.h"

// 1 push, 2 add, 3 sub, 4 mul, 5 div, 6 out,7 in, 0 hlt

struct Type_of_command
{
    const char *name_command;
    int code_of_command ;
};

typedef enum
{
    COMMAND = 1,
    REG_CELL = 2
}Str_command_t;

Type_of_command massive_of_commands[] =
{
    {"push",  1},
    {"add" ,  2},
    {"sub" ,  3},
    {"mul" ,  4},
    {"div" ,  5},
    {"out" ,  6},
    {"in"  ,  7},
    {"jmp" ,  8},
    {"ja"  ,  9},
    {"jb"  , 10},
    {"jae" , 11},
    {"jbe" , 12},
    {"je"  , 13},
    {"jne" , 14},
    {"hlt" ,  0}
};

Type_of_command massive_of_regs[] =
{
    {"ax", 1},
    {"bx", 2},
    {"cx", 3},
    {"dx", 4}
};

int type_of_command (const char* command, Str_command_t type);
int arg_push (int *code, size_t *ip, FILE *file);
int arg_jmp (int *code, size_t *ip, FILE *file);

int scan_ram_value (int *code, size_t *ip, int value);
int scan_value (int *code, size_t *ip, int value);
int scan_sum_ram (int *code, size_t *ip, int value, char *command);
int scan_str_ram (int *code, size_t *ip, char *command);
int scan_sum_reg (int *code, size_t *ip, int value, char *command);
int scan_str_reg (int *code, size_t *ip, char *command);

int assembler (asm_struct *my_asm)
{
    assert(my_asm);

    char c = '0';
    int count = 0;

    for (int i = 0; i < my_asm->size; i++)
    {
        fscanf(my_asm->file_in, "%c", &c);
        if (c == '\n')
            count++;
    }

    fseek(my_asm->file_in, 0, SEEK_SET);
    //

    char command[50] = {};


    unsigned int amount_of_command = 0;

    for (int i = 0; i < count; i++)
    {
        fscanf(my_asm->file_in, "%s", command);
        int type = type_of_command(command, COMMAND);
        if (type < 0)
        {
            printf ("error with %s\n", command);
            return ERROR_WITH_READ;
        }
        my_asm->code[amount_of_command++] = type;

        if (type == PUSH)
        {
            if (arg_push(my_asm->code, &amount_of_command, my_asm->file_in) == ERROR_WITH_READ)
                printf("ERROR\n");
        }
        if (type >= JMP && type <= JNE)
        {
            if (arg_jmp(my_asm->code, &amount_of_command, my_asm->file_in) == ERROR_WITH_READ)
                printf("ERROR\n");
        }
        if (type == HLT)
            break;
    }

    if (fwrite(my_asm->code, sizeof(int), amount_of_command, my_asm->file_out) != amount_of_command)
        printf("Problem with writing\n");

    for (size_t i = 0; i < amount_of_command; i++)
        printf ("%d ",my_asm->code[i]);
    return 0;
}

int type_of_command (const char* command, Str_command_t type)
{
    if (type == COMMAND)
    {
        int size_of_array = sizeof(massive_of_commands) / sizeof(massive_of_commands[0]);

        for (int i = 0; i < size_of_array; i++)
        {
            if (strcmp(command, massive_of_commands[i].name_command) == 0)
                return massive_of_commands[i].code_of_command;
        }

    }

    if (type == REG_CELL)
    {
        size_t size_of_array = size_reg;

        for (size_t i = 0; i < size_of_array; i++)
        {
        if (strcmp(command, massive_of_regs[i].name_command) == 0)
            return massive_of_regs[i].code_of_command;
        }

    }
    printf("No such command %s\n", command);
    return ERROR_WITH_READ;
}

int scan_ram_value (int *code, size_t *ip, int value)
{
    assert(code);
    assert (ip);

    code[(*ip)++] = 5;

    code[(*ip)++] = value;

    return 0;
}

int scan_value (int *code, size_t *ip, int value)
{
    assert(code);
    assert (ip);

    code[(*ip)++] = 1;
    code[(*ip)++] = value;

    return 0;
}

int scan_sum_ram (int *code, size_t *ip, int value, char *command)
{
    assert(code);
    assert (ip);
    assert(command);

    code[(*ip)++] = 7;
    int type = type_of_command(command, REG_CELL);
    if (type < 0)
        return ERROR_WITH_READ;
    code[(*ip)++] = value;

    return 0;

}

int scan_str_ram (int *code, size_t *ip, char *command)
{
    assert(code);
    assert (ip);
    assert(command);

    code[(*ip)++] = 6;

    command[2] ='\0';
    int type = type_of_command(command, REG_CELL);

    if (type < 0)
        return ERROR_WITH_READ;
    code[(*ip)++] = type;

    return 0;
}

int scan_sum_reg (int *code, size_t *ip, int value, char *command)
{
    assert(code);
    assert (ip);
    assert(command);

    code[(*ip)++] = 3;
    int type = type_of_command(command, REG_CELL);
    if (type < 0)
        return ERROR_WITH_READ;

    code[(*ip)++] = type;
    code[(*ip)++] = value;

    return 0;

}

int scan_str_reg(int *code, size_t *ip, char *command)
{
    assert(code);
    assert (ip);
    assert(command);

    code[(*ip)++] = 2;

    code[(*ip)++] = type_of_command(command, REG_CELL);

    return 0;
}

int arg_push (int *code, size_t *ip, FILE *file)
{
    int value = 0;
    char command[50] = {};

    if (fscanf(file, "[%d]", &value) == 1)
    {
        scan_ram_value(code, ip, value);
        return OK;
    }

    if (fscanf(file, "%d", &value) == 1)
    {
        scan_value(code, ip, value);
        return OK;
    }

    int amount_of_read = fscanf(file, "[%s + %d]", command, &value);

    if (amount_of_read == 2)
    {

        scan_sum_ram(code, ip, value, command);
        return OK;
    }

    if (amount_of_read == 1)
    {
        scan_str_ram(code, ip, command);
        return OK;
    }

    amount_of_read = fscanf(file, "%s + %d", command, &value);

    if (amount_of_read == 2)
    {
        scan_sum_reg(code, ip, value, command);
        return OK;
    }

    if (amount_of_read == 1)
    {
        scan_str_reg(code, ip, command);
        return OK;
    }

    return ERROR_WITH_READ;
}

int arg_jmp (int *code, size_t *ip, FILE *file)
{
    int value = 0;
    if (fscanf(file, "%d", &value) == 1)
    {
        code[(*ip)++] = value;
        return OK;
    }
    else
        return ERROR_WITH_READ;
}
int asm_ctor (asm_struct *my_asm)
{
    assert(my_asm);


    my_asm->file_in = fopen(my_asm->input_file, "r");
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

    free(my_asm->code);

    return 0;
}
