#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#include "stack.h"
#include "check.h"
#include "operators.h"
#include "process.h"
#include "data.h"

bool if_above(int * x1, int* x2);
bool if_above_or_equal(int * x1, int* x2);
bool if_beneath(int *x1, int* x2);
bool if_beneath_or_equal(int *x1, int* x2);
bool if_not_equal (int *x1, int* x2);
bool if_equal (int *x1, int* x2);


int jmp_command (SPU *my_spu, bool (* Compare_func)(int*, int*));

int sub (SPU *my_spu)
{
    assert(my_spu);

    int first_number = 0;
    int second_number = 0;

    stack_pop(&my_spu->stk, &first_number);
    stack_pop(&my_spu->stk, &second_number);

    stack_push(&my_spu->stk, second_number - first_number);
    printf ("%d %d\n", second_number, first_number);
    return 0;
}

int add (SPU *my_spu)
{
    assert(my_spu);

    int first_number = 0;
    int second_number = 0;

    stack_pop(&my_spu->stk, &first_number);
    stack_pop(&my_spu->stk, &second_number);
    stack_push(&my_spu->stk, first_number + second_number);

    STACK_DUMP(&my_spu->stk);

    return 0;
}

int mul (SPU *my_spu)
{
    assert(my_spu);

    int first_number = 0;
    int second_number = 0;

    stack_pop(&my_spu->stk, &first_number);
    stack_pop(&my_spu->stk, &second_number);

    stack_push(&my_spu->stk, second_number * first_number);

    STACK_DUMP(&my_spu->stk);
    return 0;
}

int divide (SPU *my_spu)
{
    assert(my_spu);

    int first_number = 0;
    int second_number = 0;

    stack_pop(&my_spu->stk, &first_number);
    stack_pop(&my_spu->stk, &second_number);

    if (first_number != 0)
        stack_push(&my_spu->stk, second_number / first_number);

    else
        {
            printf ("division on 0 is forbidden\n");
            dtor(&my_spu->stk);
            assert(0);
        }

    return 0;

}

int out (SPU *my_spu)
{
    assert(my_spu);

    int result = 0;
    stack_pop(&my_spu->stk, &result);
    printf ("%d\n", result);
    STACK_DUMP(&my_spu->stk);

    return 0;
}

int in (SPU *my_spu)
{
    assert(my_spu);

    int new_elem = 0;
    scanf ("%d", &new_elem);
    stack_push(&my_spu->stk, new_elem);

    return 0;
}

int get_arg (SPU *my_spu)
{
    int arg_t = my_spu->code_of_command.code[my_spu->ip + 1];
    int result = 0;

    if (arg_t & ADDR_REG)
    {
        result += my_spu->registers[my_spu->code_of_command.code[my_spu->ip + 2] - 1];
        my_spu->ip++;
    }

    if (arg_t & VALUE)
    {
        result += my_spu->code_of_command.code[my_spu->ip + 2];
        my_spu->ip++;
    }

    if (arg_t & ADDR_RAM)
    {
        int add_r = result;
        result = my_spu->RAM[add_r];
    }

    if (!(arg_t & ADDR_RAM) && !(arg_t & ADDR_REG) && !(arg_t & VALUE))
    {
        printf("Error\n");
        return -1;
    }

    stack_push(&my_spu->stk, result);

    my_spu->ip += 1;
    return 0;

}

int jmp_command (SPU *my_spu, bool (* Compare_func)(int*, int*))
{
    assert(my_spu);

    int first_number = 0;
    int second_number = 0;

    stack_pop(&my_spu->stk, &first_number);
    stack_pop(&my_spu->stk, &second_number);

    if (Compare_func(&first_number, &second_number))
        my_spu->ip = my_spu->code_of_command.code[my_spu->ip + 1] - 1;
    else
        my_spu->ip += 1;

    STACK_DUMP(&my_spu->stk);
    return 0;
}

int jmp (SPU *my_spu)
{
    assert(my_spu);

    my_spu->ip = my_spu->code_of_command.code[my_spu->ip + 1] - 1;

    return 0;
}

int ja (SPU *my_spu)
{
    assert(my_spu);

    jmp_command(my_spu, if_above);

    return 0;
}

int jb (SPU *my_spu)
{
    assert(my_spu);

    jmp_command(my_spu, if_beneath);

    return 0;
}

int jae (SPU *my_spu)
{
    assert(my_spu);

    jmp_command(my_spu, if_above_or_equal);

    return 0;
}

int jbe (SPU *my_spu)
{
    assert(my_spu);

    jmp_command(my_spu, if_beneath_or_equal);

    return 0;
}

int je (SPU *my_spu)
{
    assert(my_spu);

    jmp_command(my_spu, if_equal);

    return 0;
}

int jne (SPU *my_spu)
{
    assert(my_spu);

    jmp_command(my_spu, if_not_equal);

    return 0;
}

bool if_above(int *x1, int* x2)
{
    if ((*x1 - *x2) > 0)
        return true;
    return false;
}

bool if_above_or_equal(int *x1, int* x2)
{
    if ((*x1 - *x2) >= 0)
        return true;
    return false;
}

bool if_beneath(int *x1, int* x2)
{
    if ((*x1 - *x2) < 0)
        return true;
    return false;
}

bool if_beneath_or_equal(int *x1, int* x2)
{
    if ((*x1 - *x2) <= 0)
        return true;
    return false;
}

bool if_equal (int *x1, int* x2)
{
    if ((*x1 - *x2) == 0)
        return true;
    return false;
}

bool if_not_equal (int *x1, int* x2)
{
    if ((*x1 - *x2) != 0)
        return true;
    return false;
}
