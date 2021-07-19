#include <stdio.h>
#include <vips/vips.h>
#include "phash.h"


#define HASH_SIZE_PIXELS 8


int main(int argc, char** argv)
{
    char *filename = "vanagon.jpg\0";
    VipsImage *p_img = vips_image_new_from_file(filename, NULL);
    if (p_img == NULL)
    {
        printf("Failed to load %s\n", filename);
        return 1;
    }
    uint64_t hashval = 0;

    for (int i=0; i<100; ++i) 
    {
        hashval = phash(p_img, HASH_SIZE_PIXELS);
    }
    printf("it worked?\n");
    printf("Hash value: %u\n", hashval);
    return 0;
}
