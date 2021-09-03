/******************************************************************************
* Name:     imghash.c
* Author:   Zachary Colbert <zcolbert1993@gmail.com>
* Purpose:  Definitions for image hashing functions and their helper functions.
*
* Description:
*
*   This file provides implementation for various image hashing methods
*   and their associated helpers. 
******************************************************************************/


#include <assert.h>
#include <math.h>

#include "imghash.h"


int sum(int *values, size_t len)
/* Return the sum of values */
{
    int sum = 0;
    for (size_t i=0; i<len; ++i) {
        sum += values[i];
    }
    return sum;
}

float average(int *values, size_t len)
/* Return the average of values */
{
    return (float)sum(values, len) / (float)len;
}

double scale_factor(int dim_new, int dim_current)
/* 
    Return the multiplication factor required to produce dim_new 
    from dim_current via: dim_new = dim_current * scale_factor 
*/
{
    return (double)dim_new/(double)dim_current;
}

int pixel_values(VipsImage *img, int *out_arr, size_t height, size_t width)
/* 
    Populate out_arr with integers representing the value of each pixel in img.
    The length of out_arr must be equal to height * width.
*/
{
    assert(img != NULL);

    VipsRegion *region = NULL;
    if ( !(region = vips_region_new(img)) ) {
        return EXIT_FAILURE;
    }

    VipsRect r = { left: 0, top: 0, width: width, height: height };
    vips_region_prepare(region, &r);

    int pos = 0;
    VipsPel *pxval;
    for (int y=0; y<height; ++y)
    {
        for (int x=0; x<width; ++x)
        {
            pxval = VIPS_REGION_ADDR(region, x, y); 
            out_arr[pos++] = *pxval;
        }
    }
    g_object_unref(region);
    return EXIT_SUCCESS;
}

VipsImage *resize(VipsImage *in, size_t width, size_t height)
/* 
    Return a pointer to a new VipsImage containing the scaled contents 
    of the original. The dimensions of the new image will be width x height.

    The resulting VipsImage* must be freed using g_object_unref()
*/
{
    assert(in != NULL);

    VipsImage *out = NULL;
    vips_resize(
        in, &out, 
        /*hscale*/ scale_factor(width , vips_image_get_width(in)),
        "vscale",  scale_factor(height, vips_image_get_height(in)),
        NULL
    );

    assert(vips_image_get_width(out) == width);
    assert(vips_image_get_height(out) == height);

    return out;
}

VipsImage *convert_to_grayscale(VipsImage *in)
/* 
    Return a pointer to a new VipsImage containing the data of in,
    where color bands have been converted to grayscale.

    The resulting VipsImage* must be freed using g_object_unref()
*/
{
    assert(in != NULL);

    VipsImage *out= NULL;
    vips_colourspace(in, &out, VIPS_INTERPRETATION_B_W, NULL);
    return out;
}

static uint64_t compute_bit_string_ahash(VipsImage *img, const unsigned int height, const unsigned int width)
{
    assert(img != NULL);

    const unsigned int hash_size = height * width;
    int values[hash_size];
    pixel_values(img, values, height, width); 

    float avg = average(values, hash_size);
    uint64_t hashval = 0;
    uint64_t mask = pow(2.0, (double)(8*sizeof(uint64_t)-1));

    for (int i=0; i<HASH_SIZE; ++i)
    {
        if (values[i] < avg) {
            hashval |= mask;
        }
        mask >>= 1;
    }
    return hashval;
}

static uint64_t compute_bit_string_dhash(VipsImage *img, const unsigned int height, const unsigned int width)
{
    assert(img != NULL);

    const unsigned int hash_size = height * width;
    int values[hash_size];
    pixel_values(img, values, height, width); 

    uint64_t hashval = 0;
    uint64_t mask = pow(2.0, (double)(8*sizeof(uint64_t)-1));

    for (int i=0; i<hash_size; ++i)
    {
        if (values[i] < values[i+1]) {
            hashval |= mask;
        }
        mask >>= 1;
    }
    return hashval;
}

uint64_t ahash(VipsImage *img)
/* Calculate an average hash from the pixels in img. */
{
    if (img == NULL) {
        return 0;
    }

    VipsImage *resized = NULL;
    VipsImage *grayscaled = NULL;

    // Reduce detail by scaling the image down and converting to grayscale
    resized = resize(img, HASH_PX_PER_ROW, HASH_NUM_OF_ROWS);
    grayscaled = convert_to_grayscale(resized);

    uint64_t hashval = compute_bit_string_ahash(
                            grayscaled, 
                            vips_image_get_height(grayscaled), 
                            vips_image_get_width(grayscaled));

    // Unreference image objects to deallocate memory
    g_object_unref(grayscaled);
    g_object_unref(resized);

    return hashval;
}

uint64_t dhash(VipsImage *img)
/* Calculate a difference hash from the pixels in img. */
{
    if (img == NULL) {
        return 0;
    }

    VipsImage *resized = NULL;
    VipsImage *grayscaled = NULL;

    // Increase row width by one since number of comparisons is width - 1
    const int px_per_row = HASH_PX_PER_ROW + 1;  

    // Reduce detail by scaling the image down and converting to grayscale
    resized = resize(img, px_per_row, HASH_NUM_OF_ROWS);
    grayscaled = convert_to_grayscale(resized);

    uint64_t hashval = compute_bit_string_dhash(
                            grayscaled, 
                            vips_image_get_height(grayscaled), 
                            vips_image_get_width(grayscaled));

    // Unreference image objects to deallocate memory
    g_object_unref(grayscaled);
    g_object_unref(resized);

    return hashval;
}

uint64_t ahash_from_file(char *filename)
/* Open the image file located at filename and calculate an average hash. */
{
    VipsImage *img = vips_image_new_from_file(filename, NULL);
    if (img == NULL) {
        return 0;
    }
    uint64_t value = ahash(img);
    g_object_unref(img);
    return value;
}

uint64_t dhash_from_file(char *filename)
/* Open the image file located at filename and calculate a difference hash. */
{
    VipsImage *img = vips_image_new_from_file(filename, NULL);
    if (img == NULL) {
        return 0;
    }
    uint64_t value = dhash(img);
    g_object_unref(img);
    return value;
}

unsigned int distance(uint64_t lhs, uint64_t rhs)
/* Return the Hamming distance between the two hash values, lhs and rhs */
{
    if (lhs == rhs) {
        return 0;
    }
    uint64_t diff = lhs ^ rhs;  // xor to compute differences
    size_t num_bits = 8 * sizeof(uint64_t);

    // Set first bit in mask and zero the rest
    uint64_t mask = 1; 
    mask <<= (num_bits - 1);

    unsigned int diff_count = 0;
    do 
    {
        if (diff & mask) {
            ++diff_count;
        }
    }  while (mask >>= 1);

    return diff_count;
}

