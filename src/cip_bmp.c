#include "cip_bmp.h"
#include "cip_img.h"
#include "cip_io.h"
#include <stdio.h>
#include <stdlib.h>

/* Create cip image point with bmp image  */
cipImgPtr cipCreateFromBmp(char *filename) {
    cipImgPtr im = NULL;
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return NULL;

    bmp_fhd_t *bmp_fhd = (bmp_fhd_t *)malloc(sizeof(bmp_fhd_t));
    if (!bmp_fhd)
        goto free_fp;

    if (!read_bmp_file_header(fp, bmp_fhd))
        goto free_fhd;

    bmp_info_t *bmp_info = (bmp_info_t *)malloc(sizeof(bmp_info_t));
    if (!bmp_info)
        goto free_fhd;

    if (!read_bmp_info_header(fp, bmp_info))
        goto free_info;

    im = cipImageCreate(bmp_info->width, bmp_info->height);

    if (!im)
        goto free_info;

    show_bmp_file_header(bmp_fhd);
    show_bmp_info_header(bmp_info);

free_info:
    free(bmp_info);

free_fhd:
    free(bmp_fhd);

free_fp:
    fclose(fp);

    return im;
}

/* Read bmp file info header */
static int read_bmp_file_header(FILE *fp, bmp_fhd_t *fhd) {
    if (!fhd)
        return 0;

    if (!cip_read_short(&fhd->sig, fp) || !cip_read_int(&fhd->filesize, fp) ||
        !cip_read_int(&fhd->reversed, fp) || !cip_read_int(&fhd->offset, fp)) {
        return 0;
    }

    return 1;
}

/* Read bmp file info header */
static int read_bmp_info_header(FILE *fp, bmp_info_t *info) {
    if (!info)
        return 0;

    if (!cip_read_int(&info->header_size, fp))
        return 0;

    switch (info->header_size) {

    case BITMAPINFOHEADER:
    case BITMAPV4HEADER:
    case BITMAPV5HEADER:
        if (!read_bmp_info_header_v3(fp, info))
            return 0;
        break;
    default:
        printf("Unimplement header type %d\n", info->header_size);
        return 0;
    }

    return 1;
}

/* Read bmp file info header following BITMAPINFOHEADER version */
static int read_bmp_info_header_v3(FILE *fp, bmp_info_t *info) {

    /* Read header size with specific size */
    if (!cip_read_int(&info->width, fp) || !cip_read_int(&info->height, fp) ||
        !cip_read_short(&info->plane, fp) ||
        !cip_read_short(&info->bit_per_pixel, fp) ||
        !cip_read_int(&info->compression, fp) ||
        !cip_read_int(&info->image_size, fp) ||
        !cip_read_int(&info->x_resolution, fp) ||
        !cip_read_int(&info->y_resolution, fp) ||
        !cip_read_int(&info->color_used, fp) ||
        !cip_read_int(&info->important_color, fp)) {
        return 0;
    }

    /* Check header availablity */
    if (info->width <= 0 || info->plane <= 0 || info->bit_per_pixel <= 0 ||
        info->color_used < 0 || info->important_color < 0)
        return 0;

    if (info->height == 0)
        return 0;

    /* if height < 0 mean scan direction is top to down */
    if (info->height < 0) {
        info->top_down = 1;
        info->height = -1 * info->height;
    } else {
        info->top_down = 0;
    }

    return 1;
}

/* Show bmp file header infomation */
static void show_bmp_file_header(const bmp_fhd_t *fhd) {
    printf("-----BMP file header-----\n");
    char c[2];
    c[0] = (fhd->sig >> 8) & 0xff;
    c[1] = fhd->sig & 0xff;

    printf("Signature:%c%c\n", c[0], c[1]);
    printf("FileSize:%.2fKB\n", (float)fhd->filesize / 1000.0);
    printf("Reversed:%d\n", fhd->reversed);
    printf("Offset:%d\n", fhd->offset);

    printf("-------------------------\n");
}

/* Show bmp info header infomation */
static void show_bmp_info_header(const bmp_info_t *info) {
    printf("-----BMP info header-----\n");

    printf("HeaderSize:%d\n", info->header_size);
    printf("Width:%d\n", info->width);
    printf("Height:%d\n", info->height);
    printf("Plane:%d\n", info->plane);
    printf("BitPerPixel:%d\n", info->bit_per_pixel);
    printf("Compression:%d\n", info->compression);
    printf("ImageSize:%d\n", info->image_size);
    printf("XResolution:%d\n", info->x_resolution);
    printf("YResolution:%d\n", info->y_resolution);
    printf("UsedColor:%d\n", info->color_used);
    printf("ImportantColor:%d\n", info->important_color);
    printf("IsTopDown:%d\n", info->top_down);

    printf("-------------------------\n");
}
