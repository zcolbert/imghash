#ifndef IMGHASH_H 
#define IMGHASH_H

#include <stdint.h>
#include <vips/vips.h>


#define HASH_PX_PER_ROW 8
#define HASH_NUM_OF_ROWS 8
#define HASH_SIZE HASH_PX_PER_ROW*HASH_NUM_OF_ROWS


float average(int *values, size_t len);
double scale_factor(int dim_new, int dim_current);
void pixel_values(VipsImage* img, int* out_arr, size_t rows, size_t cols);

uint64_t ahash(VipsImage *img);
uint64_t dhash(VipsImage *img);
uint64_t phash(VipsImage *img);


#endif  // IMGHASH_H
