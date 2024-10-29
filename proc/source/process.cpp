#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>

#include "stack.h"
#include "check.h"
#include "operators.h"
#include "process.h"
#include "data.h"


struct functions_struct
{
    int (*func) (SPU *my_spu);
    Command command;
};


functions_struct functions_array[] =
{
    {&get_arg, PUSH},
    {&add,     ADD },
    {&sub,     SUB },
    {&mul,     MUL },
    {&divide,  DIV },
    {&out,     OUT },
    {&in,      IN  },
    {&jmp,     JMP },
    {&ja,      JA  },
    {&jb,      JB  },
    {&jae,     JAE },
    {&jbe,     JBE },
    {&je,      JE  },
    {&jne,     JNE },
};

int spu_ctor(SPU *my_spu, const char *dump)
{
    assert (my_spu);

    my_spu->dump = fopen(dump, "w");

    if (!my_spu->dump)
    {
        printf("file has't opened");
        assert(0);
    }
    return 0;
}

int spu_dtor (SPU *my_spu)
{
    assert (my_spu);

    fclose(my_spu->code_of_command.test_file);
    fclose(my_spu->dump);
    free(my_spu->code_of_command.code);
    dtor(&my_spu->stk);

    return 0;
}


int process_code (SPU *my_spu)
{
    assert(my_spu);

    my_spu->ip = -1;
    int flag = 1;

    while (1)
    {
        my_spu->ip += 1;
        STACK_DUMP(&my_spu->stk);
        dump_code(my_spu);
        int elem = my_spu->code_of_command.code[my_spu->ip];

        if (elem == HLT)
        {
            break;
        }

        flag = 0;

        int amount_of_commands = sizeof(functions_array)/sizeof(functions_array[0]);
        for (int i = 0; i < amount_of_commands; i++)
        {
            if (elem == functions_array[i].command)
            {
                functions_array[i].func(my_spu);
                flag = 1;
                break;
            }

        }

        if (flag == 0)
            printf ("problem %d\n", elem);

    }
    return 0;
}


void dump_code (SPU *my_spu)
{
    fprintf (my_spu->dump, "Code status\n----------------------------------------\n");
    for (size_t i = 0; i < my_spu->code_of_command.size; i++)
    {
        if (i == my_spu->ip)
            fprintf (my_spu->dump, "ip -> code[%d] = %d\n", i, my_spu->code_of_command.code[i]);
        else
            fprintf (my_spu->dump, "code[%d] = %d\n",i, my_spu->code_of_command.code[i]);
    }

    fprintf (my_spu->dump, "\n----------------------------------------\n");
}
