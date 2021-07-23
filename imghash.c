#include "imghash.h"


float average(int *values, size_t len)
{
    int sum = 0;
    for (size_t i=0; i<len; ++i) {
        sum += values[i];
    }
    return (float)sum / (float)len;
}


void resize(VipsImage *img, VipsImage *out, size_t size)
{
    vips_resize(
        img, &out, 
        scale_factor(size, vips_image_get_width(img)),              /* hscale */
        "vscale", scale_factor(size, vips_image_get_height(img)),   /* vscale */
        NULL
    );
}

void pixel_values(VipsImage *img, int *out_arr, size_t rows, size_t cols)
{
    VipsRegion *region = vips_region_new(img);
    VipsRect r = { left:0, top:0, width:cols, height:rows };
    vips_region_prepare(region, &r); // fill region with pixels

    VipsPel *pixval;
    int pos = 0;
    for (int y=0; y < rows; ++y)
    {
        for (int x=0; x < cols; ++x)
        {
            pixval = VIPS_REGION_ADDR(region, x, y);
            out_arr[pos] = *pixval;
            ++pos;
        }
    }
    g_object_unref(region);
}

uint64_t ahash(VipsImage *img)
{
    VipsImage *tmp;

    // Scale the image down and convert to grayscale
    vips_resize(
        img, &tmp, 
        scale_factor(HASH_PX_PER_ROW, vips_image_get_width(img)),              // hscale
        "vscale", scale_factor(HASH_NUM_OF_ROWS, vips_image_get_height(img)),  // vscale
        NULL
    );
    vips_colourspace(img, &tmp, VIPS_INTERPRETATION_B_W, NULL);     // convert to greyscale


    /* TODO factor into a helper function */
    // Compute the bit string
    // Set a 1 for each pixel that is less than the avg value, otherwise set 0
    int values[HASH_SIZE];
    pixel_values(tmp, values, HASH_NUM_OF_ROWS, HASH_PX_PER_ROW); 
    float avg = average(values, HASH_SIZE);
    uint64_t hashval = 0;
    uint64_t mask = pow(2.0, 63.0);

    for (int i=0; i<HASH_SIZE; ++i)
    {
        if (values[i] < avg) {
            hashval |= mask;
        }
        mask >>= 1;
    }
    return hashval;
}
double scale_factor(int dim_new, int dim_current)
{
    return (double)dim_new/(double)dim_current;
}

