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
    phash(p_img, HASH_SIZE_PIXELS);
    printf("it worked?\n");
    return 0;
}
