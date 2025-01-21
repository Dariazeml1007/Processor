#ifndef ASM
#define ASM

#include "label.h"

typedef enum
{
    ASM_STAT_FILE_ERROR = 1,
    ASM_ERROR_WITH_READ = -1,
    ASM_UNDEFINED_LABEL = 2,
    ASM_SUCCESS = 0,
    ASM_POSSIBLE_LABEL = -2,
    ASM_SUCCESS_GET_SIZE = 0
}Asm_status_of_read;

struct asm_struct
{
    FILE *file_in  = NULL;
    FILE *file_out  = NULL;
    labels *p_labels;
    char input_file[50];
    char output_file[50];
    char *buffer;
    int *code;
    int size = 0;
    int index = 0;
};

int asm_ctor (asm_struct *my_asm);
int asm_dtor (asm_struct *my_asm);
int assembler (asm_struct *my_asm);
int fill_gaps_in_code (asm_struct *my_asm);

#endif
