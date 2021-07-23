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

    uint64_t old = dhash(p_img);
    uint64_t hashval = 0;

    int errors = 0;
    for (int i=0; i < 10000; ++i)
    {
        hashval = dhash(p_img);
        printf("%lu\n", hashval);
        if (hashval != old)
            ++errors;
        old = hashval;
    }
    printf("Errors: %d\n", errors);
    printf("Hash size: %d\n", HASH_SIZE);

    //measure_hash_iterations(iterations, array_hash_func_ptr);
    //measure_hash_iterations(iterations, phash_func_ptr);

    return 0;
}
