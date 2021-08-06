#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <vips/vips.h>

#include "sqlite3.h"
#include "dev.h"
#include "imghash.h"


bool compare(char *path1, char *path2)
{
    VipsImage *p_img1 = vips_image_new_from_file(path1, NULL);
    VipsImage *p_img2 = vips_image_new_from_file(path2, NULL);

    if (p_img1 == NULL || p_img2 == NULL) {
        printf("Failed to load image\n");
        return false;
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
    return true;
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

uint64_t ahash_file(char *filename)
{
    VipsImage *img = vips_image_new_from_file(filename, NULL);
    if (img == NULL)
    {
        printf("Failed to load %s\n", filename);
        return 0;
    }
    uint64_t value = ahash(img);
    g_object_unref(img);
    return value;
}
uint64_t dhash_file(char *filename)
{
    VipsImage *img = vips_image_new_from_file(filename, NULL);
    if (img == NULL)
    {
        printf("Failed to load %s\n", filename);
        return 0;
    }
    uint64_t value = dhash(img);
    g_object_unref(img);
    return value;
}


int main(int argc, char** argv)
{
    VIPS_INIT(argv[0]);
    //vips_leak_set(TRUE);
    char *filename = "images/vanagon/vanagon.jpg\0";
    VipsImage *p_img = vips_image_new_from_file(filename, NULL);
    if (p_img == NULL)
    {
        printf("Failed to load %s\n", filename);
        return 1;
    }

    FILE *fp = fopen("images.txt", "r");
    if (fp == NULL)
        return 1;

    /* Read filenames from a text file */
    char buf[255];
    size_t line_count = 0;
    char strings[255][255];
    while (fgets(buf, 255, fp))
    {
        size_t len = strlen(buf) - 1;
        // don't include newlines
        if (buf[len] == '\n')
            buf[len] = '\0';
        strcpy(strings[line_count], buf);
        line_count += 1;
    }
    fclose(fp);

    /* iterate filenames and construct a path */
    char *dir = "images/\0";
    char path[255];
    size_t iterations = 1000;
    for (size_t i=0; i<=iterations; ++i) 
    {
        for (size_t j=0; j<line_count; ++j)
        //for (size_t j=0; j<2; ++j)
        {
            strcpy(path, dir);
            strcat(path, strings[j]);
            //printf("Processing %s\n", path);
            uint64_t hashval = ahash_file(path);
            printf("%-32s - %16lu\n", path, hashval);
            dhash_file(path);
        }
        //printf("\n");
    }

    g_object_unref(p_img);

    vips_shutdown();
    return 0;
}
