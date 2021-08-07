#ifndef DEV_H
#define DEV_H


#include <dirent.h>
#include <stdbool.h>
#include <stdint.h>


#define BLACK   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"


void listdir(char *path);
void listdir_by_type(char *path, char *type);
void print_array(int *arr_vals, size_t len);
void print_array_named(int *arr_vals, size_t len, char *name);
void print_bits(uint64_t value);


#endif  // DEV_H
