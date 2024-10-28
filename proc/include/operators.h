#ifndef OPER
#define OPER

#include "process.h"

int add (SPU *my_spu);
int sub (SPU *my_spu);
int divide (SPU *my_spu);
int mul (SPU *my_spu);
int out (SPU *my_spu);
int in  (SPU *my_spu);
int get_arg (SPU *my_spu);

#endif
