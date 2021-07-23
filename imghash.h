#ifndef PHASH_H
#define PHASH_H

#include <stdint.h>
#include <vips/vips.h>


float average(int *values, size_t len);
double scale_factor(int dim_new, int dim_current);
void pixel_values(VipsImage* img, int* out_arr, size_t rows, size_t cols);
void resize(VipsImage *img, VipsImage *out, size_t size);

uint64_t avg_hash(int *values, size_t len);
uint64_t phash(VipsImage *img, size_t size);

void avg_hash_array(int *values, int *out, size_t len);
void phash_array(VipsImage *img, int *out, size_t size);



#endif
