#include <stdio.h>
#include <time.h>

#include <vips/vips.h>

#include "imghash.h"


#define HASH_SIZE_PIXELS 8

void compare(char *path1, char *path2)
{
    VipsImage *p_img1 = vips_image_new_from_file(path1, NULL);
    VipsImage *p_img2 = vips_image_new_from_file(path2, NULL);

    if (p_img1 == NULL || p_img2 == NULL) {
        printf("Failed to load image\n");
        return;
    }

    printf("Comparing \"%s\" vs \"%s\":\n", path1, path2);
    printf("ahash difference: %u\n", distance(ahash(p_img1), ahash(p_img2)));
    printf("dhash difference: %u\n", distance(dhash(p_img1), dhash(p_img2)));
    printf("\n");
}


int main(int argc, char** argv)
{
    char *filename = "images/vanagon.jpg\0";
    VipsImage *p_img = vips_image_new_from_file(filename, NULL);
    if (p_img == NULL)
    {
        printf("Failed to load %s\n", filename);
        return 1;
    }
    //VipsImage *tmp;
    //vips_resize(p_img, &tmp, 0.95, NULL);
    //vips_image_write_to_file(tmp, "vanagon_95pct.jpg", NULL);

    compare("images/vanagon.jpg\0", "images/vanagon.jpg\0");
    compare("images/vanagon.jpg\0", "images/vanagon_95pct.jpg\0");
    compare("images/vanagon_95pct.jpg\0", "images/vanagon_95pct.jpg\0");
    compare("images/vanagon.jpg\0", "images/half.jpg\0");
    compare("images/vanagon.jpg\0", "images/quarter.jpg\0");
    compare("images/half.jpg\0", "images/quarter.jpg\0");
    compare("images/vanagon.jpg\0", "images/blacked.jpg\0");

    return 0;
}
