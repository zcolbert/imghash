#include <stdio.h>

#include "dev.h"


void listdir(char *path)
{
    DIR *dir = opendir("./images");
    struct dirent *files;

    while ( (files = readdir(dir)) != NULL )
    {
        printf("%s\n", files->d_name);
    }
    closedir(dir);
}

void listdir_by_type(char *path, char *type)
{
}

void print_array(int *arr_vals, size_t len)
{
    printf("[");
    for (size_t i=0; i < len-1; ++i) {
        printf("%d, ", arr_vals[i]);
    }
    printf("%d]\n", arr_vals[len-1]);
}

void print_array_named(int *arr_vals, size_t len,  char *name)
{
    printf("%s = ", name);
    print_array(arr_vals, len);
}

void print_bits(uint64_t value)
{
    int num_bits = 8*sizeof(uint64_t);
    uint64_t mask = (uint64_t)pow(2.0, (double)(num_bits-1));

    for (int i=num_bits-1; i>=0; --i)
    {
        printf("%d", value & mask ? 1 : 0);
        if (i % 4 == 0) {
            printf(" ");
        }
        mask >>= 1;
    }
    printf("\n");
}

