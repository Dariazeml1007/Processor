#include <stdio.h>

void add(int x, int y){
    printf("x+y=%d \n", x+y);
}

void subtract(int x, int y){
    printf("x-y=%d \n", x-y);
}

void multiply(int x, int y){
    printf("x*y=%d \n", x*y);
}

struct functions_struct
{
    void (*func) (int a, int b);
    int a;
};

functions_struct functions_array[] =
{
    {&add, 1},
    {&subtract, 2},
    {&multiply, 3}
};

int main(void)
{
    const int LEN = 3;
    int a = 10;
    int b = 5;
    int k = 3;

    for (int i = 0; i < 3; i++)
    {
        if (functions_array[i].a == k)
            functions_array[i].func(a,b);
    }


    return 0;
}
