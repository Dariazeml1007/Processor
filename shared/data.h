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
    CALL = 9,
    RET  = 10,
    JMP  = 11,
    JA   = 12,
    JB   = 13,
    JAE  = 14,
    JBE  = 15,
    JE   = 16,
    JNE  = 17,
    HLT  = 0

}Command;

static const int size_reg = 4;
static const int size_ram = 25;

#endif
