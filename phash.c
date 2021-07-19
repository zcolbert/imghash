#include "phash.h"


float average(int *values, size_t len)
{
    int sum = 0;
    for (size_t i=0; i<len; ++i)
    {
        sum += values[i];
    }
    return (float)sum / (float)len;
}

uint64_t avg_hash(int *values, size_t len)
{
    float avg = average(values, len);
    uint64_t hashval = 0;
    uint64_t mask = pow(2.0, 63.0);

    for (int i=0; i<len; ++i)
    {
        if (values[i] < avg) 
        {
            hashval |= mask;
        }
        mask >>= 1;
    }
    return hashval;
}

void convert_to_grayscale(VipsImage *img)
{
    switch ( vips_image_get_interpretation(img) )
    {
        case VIPS_INTERPRETATION_sRGB:
            vips_sRGB2scRGB(img, &img, NULL);
            vips_scRGB2BW(img, &img, NULL);
            break;
        case VIPS_INTERPRETATION_scRGB:
            vips_scRGB2BW(img, &img, NULL);
            break;
        default: 
            printf("Invalid color channel\n");
            break;
    }
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

void pixel_values(VipsImage *img, int *out_arr, size_t imgsize)
{
    VipsRegion *region = vips_region_new(img);
    VipsRect r = { left:0, top:0, width:imgsize, height:imgsize };
    vips_region_prepare(region, &r); /* fill region with pixels */

    VipsPel *pixval;
    int x = 0;
    int y = 0;
    int pos = 0;
    while (++y < imgsize)
    {
        x = 0;
        while (++x < imgsize)
        {
            pixval = VIPS_REGION_ADDR(region, x, y);
            out_arr[pos] = *pixval;
            ++pos;
        }
    }
    g_object_unref(region);
}

uint64_t phash(VipsImage *img, size_t size)
{
    VipsImage *tmp;

    //resize(img, tmp, size);
    vips_resize(
        img, &tmp, 
        scale_factor(size, vips_image_get_width(img)),              /* hscale */
        "vscale", scale_factor(size, vips_image_get_height(img)),   /* vscale */
        NULL
    );
    convert_to_grayscale(tmp);

    int values[size*size];
    pixel_values(tmp, values, size*size);
    return avg_hash(values, size*size);
}

double scale_factor(int dim_new, int dim_current)
{
    return (double)dim_new/(double)dim_current;
}


