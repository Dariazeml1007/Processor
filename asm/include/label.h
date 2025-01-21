#ifndef LAB
#define LAB


static const size_t size_of_label_array = 128;

struct label
{
    char *name;
    int ip = -1;
    bool is_set = false;
};

struct fix_up
{
    size_t index_label;
    size_t ip_use;
};

struct labels
{
    label label_array[size_of_label_array];
    size_t amount_of_labels = 0;
    fix_up fix_up_array[size_of_label_array];
    size_t amount_of_fix_up = 0;
};


int jmp_at_label(labels *labels, char *label, size_t *ip );
int definition_of_label(labels *labels, char *label, size_t *ip );

#endif
