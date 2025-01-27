#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "check.h"
#include "stack.h"
#include "data.h"
#include "operators.h"
#include "process.h"
#include "read_from_file.h"

int main()
{
    const char* DUMP_CODE = "dump_code.log";
    const char* OUTPUT_FILE = "machine_code.bin";


    SPU my_spu = {};

    spu_ctor(&my_spu, DUMP_CODE);


    //my_spu.registers[0] = 8;
    // my_spu.RAM[7] = 200;


    StackElem_t POISON = 66666;
    stack_ctor (&my_spu.stk, 10, POISON);
    //stack_push(&my_spu.stk, 40);
    read_commands (&my_spu.code_of_command, OUTPUT_FILE);

    dump_code (&my_spu);

    process_code (&my_spu);

    memory_report(&my_spu.stk, FREE_);

    spu_dtor(&my_spu);

    return 0;
}

