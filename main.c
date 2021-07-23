#include <stdio.h>
#include <time.h>

#include <vips/vips.h>

#include "imghash.h"


#define HASH_SIZE_PIXELS 8


int main(int argc, char** argv)
{
    char *filename = "images/vanagon.jpg\0";
    VipsImage *p_img = vips_image_new_from_file(filename, NULL);
    if (p_img == NULL)
    {
        printf("Failed to load %s\n", filename);
        return 1;
    }

    uint64_t hashval = 0;
    size_t iterations = 100000;
    for (int i=0; i<100; ++i)
    {
        hashval = ahash(p_img);
        printf("%lu\n", hashval);
    }

    //measure_hash_iterations(iterations, array_hash_func_ptr);
    //measure_hash_iterations(iterations, phash_func_ptr);

    return 0;
}
