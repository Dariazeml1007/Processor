#ifndef PROC
#define PROC

typedef int type_for_spu;

#include "read_from_file.h"
#include "data.h"
#include "stack.h"

typedef enum
{
    VALUE = 1,
    ADDR_RAM = 4,
    ADDR_REG = 2
}Arg;

    // void (*funcions)( &ip)
    //void (*message) (void)
// struct functions_struct
// {
//     int (*func) (SPU *my_spu);
//     Command command;
// };

struct SPU
{
    struct Stack_t stk ;
    struct Commands code_of_command ;
    type_for_spu registers[size_reg] ;
    type_for_spu RAM[size_ram] ;
    FILE *dump = NULL;
    int ip = -1;
};

int spu_ctor(SPU *my_spu, const char *dump);
int process_code (SPU *my_spu);
void dump_code (SPU *my_spu);
int spu_dtor (SPU *my_spu);

#endif
