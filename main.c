#include <stdio.h>
#include <time.h>

#include <vips/vips.h>

#include "phash.h"


#define HASH_SIZE_PIXELS 8


uint64_t hash_img(char *filename)
{
    VipsImage *p_img = vips_image_new_from_file(filename, NULL);
    if (p_img == NULL)
    {
        return 1;
    }
    uint64_t hashval = 0;
    hashval = phash(p_img, 8);
    g_object_unref(p_img);
    return hashval;
}

void array_hash(char *filename)
{
    VipsImage *p_img = vips_image_new_from_file(filename, NULL);
    int values[64];
    phash_array(p_img, values, 8);
    g_object_unref(p_img);
}

void measure_hash_iterations(size_t iterations, void(*hash_func)(char *))
{
    clock_t start = clock();
    clock_t diff;

    printf("Testing %lu iterations\n", iterations);
    for (int i=0; i<iterations; ++i)
    {
        printf("\rProcessing %5d of %lu", i+1, iterations);
        hash_func("vanagon.jpg\0");
        hash_func("vanagon.jpg\0");
        hash_func("resized.jpg\0");
        hash_func("half.jpg\0");
        hash_func("quarter.jpg\0");
        hash_func("inverted.jpg\0");
        hash_func("inverted.jpg\0");
        hash_func("inverted.jpg\0");
    }
    printf("\n");
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken: %ds %dms\n", msec/1000, msec%1000);

}

int main(int argc, char** argv)
{
    char *filename = "vanagon.jpg\0";
    VipsImage *p_img = vips_image_new_from_file(filename, NULL);
    if (p_img == NULL)
    {
        printf("Failed to load %s\n", filename);
        return 1;
    }

    size_t iterations = 100000;

    void (*array_hash_func_ptr)(char *);
    array_hash_func_ptr = &array_hash;

    void (*phash_func_ptr)(char *);
    phash_func_ptr = &hash_img;

    measure_hash_iterations(iterations, array_hash_func_ptr);
    measure_hash_iterations(iterations, phash_func_ptr);

    return 0;
}
