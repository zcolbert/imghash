#ifndef DEV_H
#define DEV_H


#include <dirent.h>
#include <stdbool.h>
#include <stdint.h>


void listdir(char *path);
void listdir_by_type(char *path, char *type);
void print_array(int *arr_vals, size_t len);
void print_array_named(int *arr_vals, size_t len, char *name);
void print_bits(uint64_t value);


#endif  // DEV_H
