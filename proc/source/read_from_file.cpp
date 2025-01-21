#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "stack.h"
#include "check.h"
#include "operators.h"
#include "process.h"
#include "read_from_file.h"
#include "data.h"


int read_commands (Commands *code_of_command, const char *input_file)
{
    assert(code_of_command);
    assert(input_file);

    code_of_command->test_file = fopen (input_file, "rb");
    if (!code_of_command->test_file)
        assert(0 && "File hasn't opened");


    code_of_command->size = get_size_of_file(input_file)/sizeof(int);

    code_of_command->code = (int *) calloc(code_of_command->size, sizeof(int));

    if (!code_of_command->code)
    {
        printf ("Allocation memory\n");
        assert(fclose (code_of_command->test_file) == 0);
        return PROC_ERROR_WITH_READ;
    }

    if (fread(code_of_command->code, sizeof(int), code_of_command->size, code_of_command->test_file) != code_of_command->size)
    {
        printf ("Haven't read!");
        assert(fclose (code_of_command->test_file) != 0);
        return PROC_ERROR_WITH_READ;
    }

    return PROC_SUCCESS;

}
