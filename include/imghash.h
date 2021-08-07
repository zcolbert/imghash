#ifndef IMGHASH_H 
#define IMGHASH_H

#include <stdint.h>
#include <vips/vips.h>


#define HASH_PX_PER_ROW 8
#define HASH_NUM_OF_ROWS 8
#define HASH_SIZE HASH_PX_PER_ROW*HASH_NUM_OF_ROWS

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


float average(int *values, size_t len);

uint64_t ahash(VipsImage *img);
uint64_t dhash(VipsImage *img);

uint64_t ahash_from_file(char *filename);
uint64_t dhash_from_file(char *filename);

double scale_factor(int dim_new, int dim_current);
int pixel_values(VipsImage *img, int *out_arr, size_t rows, size_t cols);

unsigned int distance(uint64_t lhs, uint64_t rhs);


#endif  // IMGHASH_H
