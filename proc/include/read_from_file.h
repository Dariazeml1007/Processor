#ifndef READ
#define READ

#include "process.h"

typedef enum
{
    PROC_STAT_FILE_ERROR = 1,
    PROC_ERROR_WITH_READ = -1,
    PROC_SUCCESS = 0,
    PROC_SUCCESS_GET_SIZE = 0
}Status_of_read;

struct Commands
{
    int *code = {};
    size_t size = 0;
    FILE *test_file = NULL;
};

int read_commands (Commands *code_of_command, const char *INPUT_FILE);


#endif
