#include "cip_bmp.h"
#include "cip_img.h"
#include "cip_io.h"
#include <stdio.h>
#include <stdlib.h>


/* Create cip image point with bmp image  */
cip_img_ptr cip_img_load_from_bmp(char *filename) {
    
    cip_img_ptr im = NULL;
    
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return im;

    bmp_fhd_t *bmp_fhd = (bmp_fhd_t *)malloc(sizeof(bmp_fhd_t));
    if (!bmp_fhd)
        goto free_fp;

    /* Read bmp file header*/
    if (!read_bmp_file_header(fp, bmp_fhd))
        goto free_fhd;

    bmp_info_t *bmp_info = (bmp_info_t *)malloc(sizeof(bmp_info_t));
    if (!bmp_info)
        goto free_fhd;

    /* Read bmp info header*/
    if (!read_bmp_info_header(fp, bmp_info))
        goto free_info;

    /* Create default image data */
    im = cip_create_img(bmp_info->width, bmp_info->height, bmp_info->bit_per_pixel / BYTE_SIZE, BYTE_SIZE);

    if (!im || cip_img_buf_is_null(im)) {
        cip_destory_img(im);
        goto free_info;
    }

    /* For debug to show bmp header file*/
    show_bmp_file_header(bmp_fhd);
    show_bmp_info_header(bmp_info);

    /* Insert pixel data in to image from bmp file,
     * Different bit per pixel use differernt insert function
     */
    int success;
    switch (bmp_info->bit_per_pixel) {
    
    case 24:
        success = populate_img_pixel(im, bmp_fhd, bmp_info, fp);    
        break;
    default:
        printf("Unsupport pixel bit per pixel format!\n");
        success = 0;
        break;
    }
    
    if (!success)
        cip_destory_img(im);        

free_info:
    free(bmp_info);

free_fhd:
    free(bmp_fhd);

free_fp:
    fclose(fp);

    return im;
}

/* Save cip image to bmp file */
int cip_img_save_to_bmp(const char *filename, cip_img_ptr img)
{
    if (!filename || cip_img_buf_is_null(img)) 
        return 0;
    
    /* TODO: Still need to add channel and depth information */
    if (img->depth != 8) {
        printf("Still not assign depth information into img!\n");
        return 0;
    }


    FILE *out = fopen(filename, "w");
    if (!out)     
        return 0;

    /* Current support 24bit pixel bmp output */
    int bmp_pixel_buffer_size = img->width * img->channel * img->depth / BYTE_SIZE * img->height;

    int bmp_info_header_size = BITMAPINFOHEADER;

    int total_size = 14 + bmp_info_header_size + bmp_pixel_buffer_size;

    /* Start write header informatio into output file */

    /* Write bmp file header*/
    fputc('B', out);
    fputc('M', out);
    write_bmp_int(total_size, out);
    write_bmp_int(0, out);
    write_bmp_int(14 + bmp_info_header_size, out);
    
    /* Write bmp info header*/
    write_bmp_int(bmp_info_header_size, out); // header size
    write_bmp_int(img->width, out); // image width
    write_bmp_int(img->height, out); // image height
    write_bmp_short(1, out); // image plane
    write_bmp_short((short)(img->channel * img->depth), out); // bit per pixel
    write_bmp_int(BMP_BI_RGB, out); // compression
    write_bmp_int(bmp_pixel_buffer_size, out); // image size
    write_bmp_int(0, out); // x resolution
    write_bmp_int(0, out); // y resolution
    write_bmp_int(256, out); // color used
    write_bmp_int(0, out); // important color

    int padding = (img->width * img->channel) % 4;
    padding = padding ? 4 - padding : padding;

    int xpos, ypos, ch;
    cip_buf p;
    for (ypos = 0; ypos < img->height; ypos++) {
        for (xpos = 0; xpos < img->width; xpos++) {
            /* Get specific pixel, write to the file with bgr order*/
            for (ch = 0; ch < img->channel; ch++) {
                p = cip_get_img_buf(img, xpos, ypos, ch);   
                fputc((unsigned char) p, out); 
            }    
        }
        /* Scan padding bytes in the end of row */
        for (xpos = 0; xpos < padding; xpos++) {
            fputc('\0', out);
        }
    }    

    fclose(out);

    return 1;
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

/* Populate bmp pixel buffer into image data */
static int populate_img_pixel(cip_img_ptr img, bmp_fhd_t *fhd, bmp_info_t *info, FILE *fp)
{
    /* Current version only support non compression populate
     * method.
     * */
    if (info->compression != BMP_BI_RGB) {
        printf("Current version only support BMP_BI_RGB!\n");
        return 0;
    }

    /* Move to fp to pixel buffer position. Use file header offset data
     * to move fp, if move fail fseek will return non-zero.
     */
    if (fseek(fp, fhd->offset, SEEK_SET)) {
        printf("Can't move fp to pixel buffer position.\n");
        return 0;
    }


    /*
     * Calculate padding size. The size of each row is rounded up to a multiple
     * of 4 bytes (a 32 bits DWORD) by padding. Padding bytes must be appended
     * to the end of rows.
     * 
     *       if each row have 6 pixels with 24 bit per pixel, total have 18 bytes data.
     *       Here need to fill 2 bytes padding data to each row to let it can be multuple of 4.
     *       
     *      -------------------> row
     *      | rgbrgbrgbrgbrgbrgb(pp)
     *      |         18          2
     *      v
     *     col
     *
     * Reference: https://stackoverflow.com/questions/2601365/padding-in-24-bits-rgb-bitmap
     * */
    int padding = ((info->width * (info->bit_per_pixel / BYTE_SIZE)) % 4);
    padding = padding ? 4 - padding : padding;

    int index, ypos, xpos, ch;
    unsigned char pdata, pad;
    for (ypos = 0; ypos < info->height; ypos++) {
        for (xpos = 0; xpos < info->width; xpos++) {
            for (ch = 0; ch < img->channel; ch++) {
                if (!cip_read_char(&pdata, fp))
                    return 0;
                cip_set_img_buf(img, xpos, ypos, ch, (float)pdata); 
            }
        }

        /* Scan padding bytes in the end of row */
        for (xpos = 0; xpos < padding; xpos++) {
            if (!cip_read_char(&pad, fp))
                return 0;
        }
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

/* */
static int write_bmp_short(const short data, FILE *fp)
{
   /* Byte order need to be little endian  
    */

    if (fputc(data & 0xff, fp) == EOF)
        return 0;
    if (fputc((data >> 8) & 0xff, fp) == EOF)
        return 0;

    return 1;
}

/* */
static int write_bmp_int(const int data, FILE *fp)
{
   /* Byte order need to be little endian
    *
    */ 

    if (fputc(data & 0xff, fp) == EOF)
        return 0;
    if (fputc((data >> 8) & 0xff, fp) == EOF)
        return 0;
    if (fputc((data >> 16) & 0xff, fp) == EOF)
        return 0;
    if (fputc((data >> 24) & 0xff, fp) == EOF)
        return 0;

    return 1;
}

