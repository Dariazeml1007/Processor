#ifndef DATA
#define DATA

int get_size_of_file (const char *name_of_file);



typedef enum
{
    PUSH = 1,
    POP  = 2,
    ADD  = 3,
    SUB  = 4,
    MUL  = 5,
    DIV  = 6,
    OUT  = 7,
    IN   = 8,
    JMP  = 9,
    JA  =  10,
    JB  =  11,
    JAE  = 12,
    JBE  = 13,
    JE  =  14,
    JNE  = 15,
    HLT  = 0

}Command;

static const int size_reg = 4;
static const int size_ram = 25;

#endif
