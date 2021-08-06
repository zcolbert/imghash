#include <assert.h>
#include <math.h>

#include "imghash.h"


float average(int *values, size_t len)
{
    int sum = 0;
    for (size_t i=0; i<len; ++i) {
        sum += values[i];
    }
    return (float)sum / (float)len;
}

double scale_factor(int dim_new, int dim_current)
{
    return (double)dim_new/(double)dim_current;
}

int pixel_values(VipsImage *img, int *out_arr, size_t rows, size_t cols)
{
    assert(img != NULL);

    VipsRegion *region = NULL;
    if ( !(region = vips_region_new(img)) ) {
        return EXIT_FAILURE;
    }

    VipsRect r = { left: 0, top: 0, width: cols, height: rows };
    vips_region_prepare(region, &r);

    int pos = 0;
    VipsPel *pxval;
    for (int y=0; y<rows; ++y)
    {
        for (int x=0; x<cols; ++x)
        {
            pxval = VIPS_REGION_ADDR(region, x, y); 
            out_arr[pos++] = *pxval;
        }
    }
    g_object_unref(region);
    return EXIT_SUCCESS;
}

uint64_t ahash(VipsImage *img)
{
    assert(img != NULL);
    VipsImage *resized = NULL;
    VipsImage *grayscaled = NULL;

    vips_resize(
        img, &resized, 
        /*hscale*/ scale_factor(HASH_PX_PER_ROW, vips_image_get_width(img)),    // hscale
        "vscale",  scale_factor(HASH_NUM_OF_ROWS, vips_image_get_height(img)),  // vscale
        NULL
    );
    assert(vips_image_get_width(resized) == HASH_PX_PER_ROW);
    assert(vips_image_get_height(resized) == HASH_NUM_OF_ROWS);

    vips_colourspace(resized, &grayscaled, VIPS_INTERPRETATION_B_W, NULL);     // convert to greyscale


    int values[HASH_SIZE];
    pixel_values(grayscaled, values, HASH_NUM_OF_ROWS, HASH_PX_PER_ROW); 

    float avg = average(values, HASH_SIZE);
    uint64_t hashval = 0;
    uint64_t mask = pow(2.0, (double)(8*sizeof(uint64_t)-1));

    for (int i=0; i<HASH_SIZE; ++i)
    {
        if (values[i] < avg) {
            hashval |= mask;
        }
        mask >>= 1;
    }

    g_object_unref(grayscaled);
    g_object_unref(resized);

    return hashval;
}

uint64_t dhash(VipsImage *img)
{
    assert(img != NULL);
    VipsImage *resized = NULL;
    VipsImage *grayscaled = NULL;
    // Increase row width by one since number of comparisons is width - 1
    const int px_per_row = HASH_PX_PER_ROW + 1;  
    // Update total pixel count to reflect increased row width
    const int num_px = HASH_NUM_OF_ROWS * px_per_row;
    // Scale the image down and convert to grayscale
    vips_resize(
        img, &resized, 
        scale_factor(px_per_row, vips_image_get_width(img)),                    // hscale
        "vscale", scale_factor(HASH_NUM_OF_ROWS, vips_image_get_height(img)),   // vscale
        NULL
    );
    vips_colourspace(resized, &grayscaled, VIPS_INTERPRETATION_B_W, NULL);     // convert to greyscale

    // Compute the bit string
    // Set a 1 for each pixel that is darker than the preceding pixel, otherwise set 0
    int values[num_px];
    pixel_values(grayscaled, values, HASH_NUM_OF_ROWS, px_per_row); 

    uint64_t hashval = 0;
    uint64_t mask = pow(2.0, (double)(8*sizeof(uint64_t)-1));

    for (int i=0; i<num_px; ++i)
    {
        if (values[i] < values[i+1]) {
            hashval |= mask;
        }
        mask >>= 1;
    }

    g_object_unref(grayscaled);
    g_object_unref(resized);

    return hashval;
}

unsigned int distance(uint64_t lhs, uint64_t rhs)
{
    uint64_t diff = lhs ^ rhs;  // xor to compute differences
    size_t num_bits = 8 * sizeof(uint64_t);

    // set first bit in mask and zero the rest
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




