#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <sys\stat.h>

#include "label.h"
#include "asm.h"

int jmp_at_label(labels *labels, char *label, size_t *ip )
{
    assert(labels);
    assert(label);
    assert(ip);

    bool flag = false;
    for (size_t i = 0; i < labels->amount_of_labels; i++)
    {
        if (strcmp(labels->label_array[i].name, label) == 0)
        {
            if (labels->label_array[i].is_set)
            {
                printf("def label found(jmp) %s\n", labels->label_array[i].name);
                return labels->label_array[i].ip;
            }
            else
            {
                printf ("find def - %s\n", label);
                labels->fix_up_array[labels->amount_of_fix_up].index_label = i;
                labels->fix_up_array[labels->amount_of_fix_up++].ip_use = *ip - 1;
            }
            flag = true;
        }
    }
    if (!flag)
    {
        labels->label_array[labels->amount_of_labels].name = label;

        printf ("name of new label(jmp) = %s  amount = %d\n", labels->label_array[labels->amount_of_labels].name, labels->amount_of_labels);
        labels->fix_up_array[labels->amount_of_fix_up].index_label = labels->amount_of_labels;
        labels->amount_of_labels++;
        labels->fix_up_array[labels->amount_of_fix_up++].ip_use = *ip - 1;
    }
    return -13;
}

int definition_of_label(labels *labels, char *label, size_t *ip )
{
    assert(labels);
    assert(label);
    assert(ip);

    bool flag = false;
    for (size_t i = 0; i < labels->amount_of_labels; i++)
    {
        printf ("name = %s, ip = %d\n", labels->label_array[i].name, labels->label_array[i].ip);
    }
    for (size_t i = 0; i < labels->amount_of_labels; i++)
    {
        if (strcmp(labels->label_array[i].name, label) == 0)
        {
            printf ("label found (jmp) %s\n", label);
            if (labels->label_array[i].is_set)
            {
                printf ("Repeating label");
                assert(0);
            }
            else
            {
                labels->label_array[i].is_set = true;
                labels->label_array[i].ip = *ip;
                flag = true;
                break;
            }
        }
    }
    for (size_t i = 0; i < labels->amount_of_labels; i++)
    {
        printf ("name = %s, ip = %d\n", labels->label_array[i].name, labels->label_array[i].ip);
    }
    if (!flag)
    {
        printf("not fount label - only definition %s\n", label);
        labels->label_array[labels->amount_of_labels].name = label;
        labels->label_array[labels->amount_of_labels].is_set = true;
        labels->label_array[labels->amount_of_labels++].ip = *ip;
    }
}

