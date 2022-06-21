#ifndef CIPIMG_H
#define CIPIMG_H


/*
 * Reference:
 * https://github.com/libgd/libgd
 *
 */


#ifdef __cplusplus
extern "C" {
#endif

/*
 * pixel - Image pixel structure
 * @red: 
 * @green:
 * @blue:
 * @alpha:
 */
typedef struct pixel_data {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} pixel;

/*
 * cipImg - Base image data format
 * @pixel: 
 * @width:
 * @height:
 * @depth:
 * @channel:
 * @filename:
 *
 */
typedef struct image_data {

    /* Non compression pixel data */
    pixel *pixels;
    /* Image size*/
    int width;
    int height;
    /* Bit number of color*/
    int depth;
    /* Image color channel, gray: 1 channel, rgb: 3 channel*/
    int channel;
    
    /* For read image filename */
    char *filename;

} cipImg;


/* Base image pointer */
typedef cipImg *cipImgPtr;

/* Declare function to manipulate images */
cipImgPtr cipImageCreate(int width, int height);
void cipImageDestory(cipImgPtr img);
void cipFree(cipImgPtr img);

/* Helper macro */
#define cipImageWidth(im) (im)->width;
#define cipImageHeight(im) (im)->height;

#ifdef __cplusplus
}
#endif

#endif
