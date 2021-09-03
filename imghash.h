/******************************************************************************
* Name:     imghash.h
* Author:   Zachary Colbert <zcolbert1993@gmail.com>
* Purpose:  Declarations for image hashing functions.
*   
* Description:  
*
*   Contains declarations for various image hashing functions,
*   and their associated helper functions.
******************************************************************************/


#ifndef IMGHASH_H 
#define IMGHASH_H

#include <stdint.h>
#include <vips/vips.h>


#define HASH_PX_PER_ROW 8
#define HASH_NUM_OF_ROWS 8
#define HASH_SIZE HASH_PX_PER_ROW*HASH_NUM_OF_ROWS

#define IMGHASH_EXIT_SUCCESS 0
#define IMGHASH_EXIT_FAILURE 1


// Functions for calculating hash from a VipsImage
uint64_t ahash(VipsImage *img);
uint64_t dhash(VipsImage *img);

// Functions for calculating hash from an image file on disk
uint64_t ahash_from_file(char *filename);
uint64_t dhash_from_file(char *filename);

// Calculate the distance between two hashes
unsigned int distance(uint64_t lhs, uint64_t rhs);

// Image transformation functions used during hashing operations
VipsImage *convert_to_grayscale(VipsImage *orig);
VipsImage *resize(VipsImage *orig, size_t width_new, size_t height_new);

// Helper functions used during hashing operations
float average(int *values, size_t len);
int pixel_values(VipsImage *img, int *out_arr, size_t rows, size_t cols);
double scale_factor(int dim_new, int dim_current);


#endif  // IMGHASH_H

