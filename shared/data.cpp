#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "data.h"

int get_size_of_file (const char *name_of_file)
{
    assert(name_of_file);
    int info_of_file = 0;
    struct stat buffer;


    if ((info_of_file = stat(name_of_file, &buffer)) != 0)
    {
        printf("stat failure error .%d\n", info_of_file);
        return STAT_FILE_ERROR;
    }

    return buffer.st_size;
}
