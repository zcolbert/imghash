#ifndef PHASH_H
#define PHASH_H

#include <stdint.h>
#include <vips/vips.h>


float average(int *values, size_t len);
double scale_factor(int dim_new, int dim_current);
void convert_to_grayscale(VipsImage *img);
void pixel_values(VipsImage* img, int* out_arr, size_t imgsize);
void resize(VipsImage *img, VipsImage *out, size_t size);
uint64_t avg_hash(int *values, size_t len);
uint64_t phash(VipsImage *img, size_t size);


#endif
