#include <stdio.h>
#include <time.h>

#include <vips/vips.h>

#include "dev.h"
#include "imghash.h"


void compare(char *path1, char *path2)
{
    VipsImage *p_img1 = vips_image_new_from_file(path1, NULL);
    VipsImage *p_img2 = vips_image_new_from_file(path2, NULL);

    if (p_img1 == NULL || p_img2 == NULL) {
        printf("Failed to load image\n");
        return;
    }

    for (int i=0; i<64; ++i) { printf("="); }
    printf("\nComparing \"%s\" vs \"%s\":\n", path1, path2);
    for (int i=0; i<64; ++i) { printf("-"); }
    printf("\n");

    uint64_t ah1 = ahash(p_img1);
    uint64_t ah2 = ahash(p_img2);
    uint64_t dh1 = dhash(p_img1);
    uint64_t dh2 = dhash(p_img2);

    //printf("ahash difference: %u\n", distance(ahash(p_img1), ahash(p_img2)));
    //printf("dhash difference: %u\n", distance(dhash(p_img1), dhash(p_img2)));

    printf("ahash difference: %u\n", distance(ah1, ah2));
    //test_bits(ah1, ah2);

    printf("dhash difference: %u\n", distance(dh1, dh2));
    //test_bits(dh1, dh2);

    for (int i=0; i<64; ++i) { printf("-"); }
    printf("\n");
}

void inspect_pixel_values(char *filename)
{
    VipsImage *p_img = vips_image_new_from_file(filename, NULL);
    if (p_img == NULL)
    {
        printf("Failed to load %s\n", filename);
        return;
    }

    VipsImage *tmp;
    vips_resize(
        p_img, &tmp, 
        scale_factor(8, vips_image_get_width(p_img)),          // hscale
        "vscale", scale_factor(8, vips_image_get_height(p_img)),  // vscale
        NULL
    );
    vips_colourspace(tmp, &tmp, VIPS_INTERPRETATION_B_W, NULL);
    int values[64];
    pixel_values(tmp, values, 8, 8);
    //print_array_named(values, 64, filename);
    int sum = 0;
    for (int i=0; i<64; ++i) {
        sum += values[i];
    }
    printf("(sum=%d)", sum);
    printf("(avg=%f) ", average(values, 64));
    print_array(values, 64);
}

void test_bits(uint64_t lhs, uint64_t rhs)
{
    print_bits(lhs);
    print_bits(rhs);
    print_bits(lhs ^ rhs);
    printf("%d\n", distance(lhs, rhs));
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
    FILE *fp = fopen("files.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file\n");
    }

    // TODO write test code to read filenames
    // TODO write function to generate a combination of pairs
    // TODO compare the pairs against each other

    /*
    compare("images/vanagon.jpg\0", "images/vanagon.jpg\0");
    compare("images/vanagon.jpg\0", "images/vanagon_95pct.jpg\0");
    compare("images/vanagon_95pct.jpg\0", "images/vanagon_95pct.jpg\0");
    compare("images/vanagon.jpg\0", "images/half.jpg\0");
    compare("images/vanagon.jpg\0", "images/quarter.jpg\0");
    compare("images/half.jpg\0", "images/quarter.jpg\0");
    compare("images/fin_blue.jpg\0", "images/fin_blue.jpg\0");
    compare("images/fin_blue.jpg\0", "images/fin_blue_2.jpg\0");
    compare("images/fin_blue.jpg\0", "images/fin_yellow.jpg\0");
    compare("images/fin_yellow.jpg\0", "images/fin_yellow.jpg\0");
    compare("images/vanagon.jpg\0", "images/fin_blue.jpg\0");
    compare("images/gloves.jpg\0", "images/gloves.jpg\0");
    compare("images/gloves.jpg\0", "images/gloves_GIMP.jpg\0");
    */


    return 0;
}
