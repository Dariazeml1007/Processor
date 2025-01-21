#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "asm.h"
#include "data.h"

typedef enum
{
    SUCCESS = 0,
    FAILED = 1

} SystemResult;

int main(const int argc, const char* argv[])
{
    asm_struct my_asm = {};
    labels p_labels = {};
    my_asm.p_labels = &p_labels;

    const char *output_file = "../machine_code.bin";

    if (argc == 1)
    {
        printf ("Source not specified\n");
        return FAILED;
    }

    printf ("%d %s\n", argc, argv[1]);
    if (argc == 2)
    {
        strcpy(my_asm.input_file, argv[1]);
        if (!my_asm.input_file)
        {
            printf ("NULL pointer");
            assert(0);
        }
        strcpy(my_asm.output_file, output_file);

    }

    if (argc == 3)
    {
        strcpy(my_asm.input_file, argv[1]);
        strcpy(my_asm.output_file, argv[2]);
    }

    asm_ctor(&my_asm);

    assembler(&my_asm);

    asm_dtor (&my_asm);

    return 0;
}

