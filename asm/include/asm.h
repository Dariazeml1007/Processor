#ifndef ASM
#define ASM


struct asm_struct
{
    FILE *file_in  = NULL;
    FILE *file_out  = NULL;
    char input_file[50];
    char output_file[50];
    int *code ;
    int size = 0;
};

int asm_ctor (asm_struct *my_asm);
int asm_dtor (asm_struct *my_asm);
int assembler (asm_struct *my_asm);

#endif
