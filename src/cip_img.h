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

/*
 * cipImgCreate - Create basic image data structure.
 * @width: image width
 * @height: image height
 *
 * TODO:Still need to add more information
 *
 */
cipImgPtr cipImgCreate(int width, int height);

/*
 * cipImgDestory - Destory image 
 * @img: 
 *
 * TODO:Can change to not free image immediately but also
 * can use its malloc memory to set new buffer. 
 */
void cipImgDestory(cipImgPtr img);

/*
 *
 */
cipImgPtr cipImgRead(char *filename);

/* */
void cipFree(cipImgPtr img);

/* Helper macro */
#define getImgWidth(im) (im)->width
#define getImgHeight(im) (im)->height
#define getImgPixel(im, x, y) \
    im->pixels[y * img->width + x]

#ifdef __cplusplus
}
#endif

#endif
