#ifndef OPER
#define OPER

#include "process.h"

int add         (SPU *my_spu);
int sub         (SPU *my_spu);
int divide      (SPU *my_spu);
int mul         (SPU *my_spu);
int out         (SPU *my_spu);
int in          (SPU *my_spu);
int get_arg_push(SPU *my_spu);
int get_arg_pop (SPU *my_spu);
int call        (SPU *my_spu);
int ret         (SPU *my_spu);

int jmp     (SPU *my_spu);
int ja      (SPU *my_spu);
int jb      (SPU *my_spu);
int jae     (SPU *my_spu);
int jbe     (SPU *my_spu);
int jne     (SPU *my_spu);
int je      (SPU *my_spu);


#endif
