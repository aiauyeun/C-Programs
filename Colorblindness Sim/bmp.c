#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

typedef struct bmp {
    uint32_t height;
    uint32_t width;
    Color palette[MAX_COLORS];
    uint8_t **a;
} BMP;

//takes in a buffer, creates a BMP file based on the info from the bufile
BMP *bmp_create(Buffer *buf) {
    BMP *bmp = calloc(1, sizeof(BMP));
    // TODO check for bmp == NULL(?)
    uint8_t type1, type2, skip8;
    uint16_t bits_per_pixel, skip16;
    uint32_t compression, bitmap_header_size, colors_used, skip32;
    read_uint8(buf, &type1);
    read_uint8(buf, &type2);
    read_uint32(buf, &skip32);
    read_uint16(buf, &skip16);
    read_uint16(buf, &skip16);
    read_uint32(buf, &skip32);
    read_uint32(buf, &bitmap_header_size);
    read_uint32(buf, &bmp->width);
    read_uint32(buf, &bmp->height);
    read_uint16(buf, &skip16);
    read_uint16(buf, &bits_per_pixel);
    read_uint32(buf, &compression);
    read_uint32(buf, &skip32);
    read_uint32(buf, &skip32);
    read_uint32(buf, &skip32);
    read_uint32(buf, &colors_used);
    read_uint32(buf, &skip32);
    if ((type1 == 'B') && (type2 == 'M') && (bitmap_header_size == 40) && (bits_per_pixel == 8)
        && (compression == 0)) {
        uint32_t num_colors = colors_used;
        if (num_colors == 0) {
            num_colors = (1 << bits_per_pixel);
        }
        for (uint32_t i = 0; i < num_colors; i++) {
            read_uint8(buf, &bmp->palette[i].blue);
            read_uint8(buf, &bmp->palette[i].green);
            read_uint8(buf, &bmp->palette[i].red);
            read_uint8(buf, &skip8);
        }
        uint32_t rounded_width = (bmp->width + 3) & ~3;

        bmp->a = calloc(rounded_width, sizeof(bmp->a[0]));
        for (uint32_t x = 0; x < rounded_width; x++) {
            bmp->a[x] = calloc(bmp->height, sizeof(bmp->a[x][0]));
        }
        for (uint32_t y = 0; y < bmp->height; y++) {
            for (uint32_t x = 0; x < rounded_width; x++) {
                read_uint8(buf, &bmp->a[x][y]);
            }
        }
    } else {
        printf("error!!! this ain't a bmp file ig?\n");
        printf("%d type1\n", type1);
        printf("%d type2\n", type2);
        printf("%d Bitmapheadersize\n", bitmap_header_size);
        printf("%d Bits per pixel\n", bits_per_pixel);
        printf("%d compression\n", compression);
    }
    return bmp;
}

void bmp_free(BMP **bmp) {
    uint32_t rounded_width = ((*bmp)->width + 3) & ~3;
    for (uint32_t i = 0; i < rounded_width; i++) {
        free((*bmp)->a[i]);
    }
    free((*bmp)->a);
    free(*bmp);
    *bmp = NULL;
}

void bmp_write(const BMP *bmp, Buffer *buf) {
    int32_t rounded_width = (bmp->width + 3) & ~3;
    int32_t image_size = (bmp->height) * rounded_width;
    int32_t file_header_size = 14;
    int32_t bitmap_header_size = 40;
    int32_t num_colors = 256;
    int32_t palette_size = 4 * num_colors;
    int32_t bitmap_offset = file_header_size + bitmap_header_size + palette_size;
    int32_t file_size = bitmap_offset + image_size;
    write_uint8(buf, 66); //B
    write_uint8(buf, 77); //M
    write_uint32(buf, file_size);
    write_uint16(buf, 0); //0
    write_uint16(buf, 0); //0
    write_uint32(buf, bitmap_offset);
    write_uint32(buf, bitmap_header_size);
    write_uint32(buf, bmp->width);
    write_uint32(buf, bmp->height);
    write_uint16(buf, 1); //1
    write_uint16(buf, 8); //8
    write_uint32(buf, 0); //0
    write_uint32(buf, image_size);
    write_uint32(buf, 2835); //2835??
    write_uint32(buf, 2835); //2835??
    write_uint32(buf, num_colors);
    write_uint32(buf, num_colors);
    for (int i = 0; i < num_colors; i++) {
        write_uint8(buf, bmp->palette[i].blue);
        write_uint8(buf, bmp->palette[i].green);
        write_uint8(buf, bmp->palette[i].red);
        write_uint8(buf, 0);
    }
    for (uint32_t y = 0; y < bmp->height; y++) {
        for (uint32_t x = 0; x < bmp->width; x++) {
            write_uint8(buf, bmp->a[x][y]);
        }

        for (int x = bmp->width; x < rounded_width; x++) {
            write_uint8(buf, 0); //0???
        }
    }
}

int constrain(int x, int a, int b) {
    return x < a ? a : x > b ? b : x;
}

void bmp_reduce_palette(BMP *bmp) {
    for (int i = 0; i < MAX_COLORS; i++) {
        int r = bmp->palette[i].red;
        int g = bmp->palette[i].green;
        int b = bmp->palette[i].blue;

        int new_r, new_g, new_b;
        double SQLE = 0.00999 * r + 0.0664739 * g + 0.7317 * b;
        double SELQ = 0.153384 * r + 0.316624 * g + 0.057134 * b;

        if (SQLE < SELQ) {
            // use 575-nm equations
            new_r = 0.426331 * r + 0.875102 * g + 0.0801271 * b + 0.5;
            new_g = 0.281100 * r + 0.571195 * g + -0.0392627 * b + 0.5;
            new_b = -0.0177052 * r + 0.0270084 * g + 1.00247 * b + 0.5;
        } else {
            // use 475-nm equations
            new_r = 0.758100 * r + 1.45387 * g + -1.48060 * b + 0.5;
            new_g = 0.118532 * r + 0.287595 * g + 0.725501 * b + 0.5;
            new_b = -0.00746579 * r + 0.0448711 * g + 0.954303 * b + 0.5;
        }

        new_r = constrain(new_r, 0, UINT8_MAX);
        new_g = constrain(new_g, 0, UINT8_MAX);
        new_b = constrain(new_b, 0, UINT8_MAX);
        bmp->palette[i].red = new_r;
        bmp->palette[i].green = new_g;
        bmp->palette[i].blue = new_b;
    }
}
