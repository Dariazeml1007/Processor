#ifndef READ
#define READ

#include "process.h"

struct Commands
{
    int *code = {};
    size_t size = 0;
    FILE *test_file = NULL;
};


int read_commands (Commands *code_of_command, const char *INPUT_FILE);


#endif
