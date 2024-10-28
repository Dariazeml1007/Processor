#ifndef DATA
#define DATA

int get_size_of_file (const char *name_of_file);

typedef enum
{
    STAT_FILE_ERROR = 1,
    ERROR_WITH_READ = -1,//
    OK = 0,
    SUCCESS_GET_SIZE = 0
}Status_of_read;

typedef enum
{
    PUSH = 1,
    ADD  = 2,
    SUB  = 3,
    MUL  = 4,
    DIV  = 5,
    OUT  = 6,
    IN   = 7,
    HLT  = 0

}Command;

static const int size_reg = 4;
static const int size_ram = 25;

#endif
