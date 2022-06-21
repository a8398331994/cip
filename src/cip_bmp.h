#ifndef CIP_BMP_H
#define CIP_BMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "cip_img.h"

#define BITMAPINFOHEADER 40
#define BITMAPV4HEADER 108
#define BITMAPV5HEADER 124

/* bmp_fhd_t - BMP file header, total 14 bytes 
 * @value: 2 Bytes signatureu
 * @filesize: 4 Bytes file size
 * @reversed: 4 Bytes usually unused
 * @offset: 4 Bytes offset from beginning of the bitmap data
 * */    
typedef struct bmp_file_header {
    short sig;
    int filesize;
    int reversed;
    int offset;
} bmp_fhd_t;

/* bmp_info_t - BMP info header 
 * @header_size: 4 Bytes info header size
 * @width: 4 Bytes image width
 * @height: 4 Bytes image height
 * @plane: 2 Bytes image plane
 * @bit_per_pixel: 2 Bytes bit per pixel
 * @compression: 4 Bytes compression flag
 * @image_size: 4 Bytes image size
 * @x_resolution: 4 Bytes horizontal resolution
 * @y_resolution: 4 Bytes vertical resolution
 * @color_used: 4 Bytes used color
 * @important_color: 4 Bytes important color
 * @top_down: flag about top_down order
 *
 * @bit_per_pixel have following combination
 * 1 = 1bit monochrome, num col
 * 4 = 4bit, num color = 16
 * 8 = 8bit, num color = 256 (supported)
 * 16 = 16bit, num color = 6553
 * 24 = 24bit, num color = 16M
 *
 * @compression have following combination
 * 0 = BI_RGB, no compression (supported)
 * 1 = BI_RLE8, 8bit RLE encoding
 * 2 = BI_RLE4, 4bit RLE encoding        
 *
 * @image_size is valid to set 0 if compression = 0
 *
 * @top_down If height < 0, than top_down = 1, else 0
 *
 *
 * */

typedef struct bmp_info_header {
    int header_size;    
    int width;
    int height;
    short plane;
    short bit_per_pixel;
    int compression;
    int image_size;
    int x_resolution;
    int y_resolution;
    int color_used;
    int important_color;
    char top_down;

} bmp_info_t;


/*
 * cipCreateFromBmp - Create cip image point with bmp image
 * @filename: input string filename
 *
 * Return: the pointer of cip image, %NULL if read bmp file occur error.
 */
cipImgPtr cipCreateFromBmp(char *filename); 

/*
 * read_bmp_file_header - Read bmp file file header
 * @fp: file pointer.
 * @bmp_fhd_t: bmp file header structure, data would ne inserted.
 * 
 * Note: this is helper function to read bmp file header
 *
 * Reture: true for read sucess, false for fp is not vaild.
 */
static int read_bmp_file_header(FILE *fp, bmp_fhd_t *fhd);

/*
 * read_bmp_info_header - Read bmp file info header
 * @fp: file pointer.
 * @bmp_info_t: bmp info header structure, data would ne inserted.
 * 
 * Note: this is helper function to read bmp file header
 *
 * Reture: true for read sucess, false for fp is not vaild.
 */
static int read_bmp_info_header(FILE *fp, bmp_info_t *info);

/*
 * read_bmp_file_header_v3 - Read bmp file info header following BITMAPINFOHEADER version
 * @fp: file pointer.
 * @bmp_fhd_t: bmp info header structure, data would ne inserted.
 * 
 * Note: this is helper function to read bmp file header
 *
 * Reture: true for read sucess, false for fp is not vaild.
 */
static int read_bmp_info_header_v3(FILE *fp, bmp_info_t *info);

/*
 * show_bmp_file_header - Show bmp file header infomation
 * @fhd: bmp file header
 * 
 * TODO: need to check sig bit order
 *
 */
static void show_bmp_file_header(const bmp_fhd_t *fhd);

/*
 * show_bmp_info_header - Show bmp info header infomation
 * @info: bmp info header
 * 
 */
static void show_bmp_info_header(const bmp_info_t *info);


#ifdef __cplusplus
}
#endif

#endif


