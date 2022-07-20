#ifndef CIPIMG_H
#define CIPIMG_H


/*
 * Reference:
 * https://github.com/libgd/libgd
 * https://github.com/symisc/sod
 */


#ifdef __cplusplus
extern "C" {
#endif


#define cip_buf float

/*
 * pixel - Image pixel structure
 * @red: 
 * @green:
 * @blue:
 * @alpha:
 */
//typedef struct pixel_data {
//    unsigned char red;
//    unsigned char green;
//    unsigned char blue;
//    unsigned char alpha;
//} cipPixel;

/*
 * cipImg - Base image data format
 * @pixel: 
 * @width:
 * @height:
 * @depth:
 * @channel:
 *
 */
typedef struct image_data {

    /* Non compression pixel data */
    //pixel *pixels;
    
    cip_buf *buf;
    /* Image size*/
    int width;
    int height;
    /* Bit depth of each buffer */
    short depth;
    /* Image color channel, gray: 1 channel, rgb: 3 channel*/
    int channel;    

} cip_img;


/* Base image pointer */
typedef cip_img *cip_img_ptr;

/* Declare function to manipulate images */

/*
 * cip_img_create - Create basic image data structure and allocate
 * buffer memory.
 * .
 * @width: image width
 * @height: image height
 * @channel: image channel
 *
 * TODO:Still need to add more information, and check exception condtion
 *
 */
cip_img_ptr cip_create_img(int width, int height, int channel, int depth);

/*
 * cip_img_create_empty - Create empty image data structure without * buffer memory.
 *
 *
 * TODO: Not implement yet.
 */
cip_img_ptr cip_create_empty_img(int width, int height, int channel, int depth);

/*
 * cip_allocate_img_buf - Allocate image buffer data in cip image.
 *
 * TODO: Check whether destory buffer is existed or not. If is 
 * existed, this function can use already allocate buffer 
 * assign to input image.
 *
 * NOTE: Input parameter should use pointer of pointer because 
 * we want to modify member in input parameter. 
 */
int cip_allocate_img_buf(cip_img_ptr *input);

/* Copy destination image 
 *
 * TODO: Not implement yet.
 */
cip_img_ptr cip_copy_img(cip_img_ptr input);

/*
 * cip_img_destory - Destory image 
 * @img: 
 *
 * TODO:Can change to not free image immediately but also
 * can use its malloc memory to set new buffer. 
 */
void cip_destory_img(cip_img_ptr img);

/*
 *
 */
cip_img cip_read_img(char *filename);

/* */
void cip_free(cip_img_ptr img);


/* Helper macro */
#define cip_get_img_width(im) (im)->width
#define cip_get_img_height(im) (im)->height
#define cip_img_buf_is_null(im) (im)->buf == NULL

/* cip_get_img_buf - get image buffer value with specific position*
 * 
 *
 */
#define cip_get_img_buf(img, x, y, c) \
    img->buf[c * img->width * img->height + y * img->width + x] * ((1U << img->depth) - 1)

/* cip_set_img_buf - set image buffer value with specific position 
 *
 */
#define cip_set_img_buf(img, x, y, c, val) \
    img->buf[c * img->width * img->height + y * img->width + x] = ((val) / (cip_buf) ((1U << img->depth) - 1));

/* cip_get_img_buf_norm - get image normallized value with specific position
 *
 */
#define cip_get_img_buf_norm(img, x, y, c) \
    img->buf[c * img->width * img->height + y * img->width + x]

/* cip_set_img_buf_norm - set image normallized value with specific position
 *
 */
#define cip_set_img_buf_norm(img, x, y, c, val) \
    img->buf[c * img->width * img->height + y * img->width + x] = (val)


/* cip_add_img_buf_norm - add image buf in normallized range with specific postion
 */
#define cip_add_img_buf_norm(img, x, y, c, val) \
    img->buf[c * img->width * img->height + y * img->width + x] += (val)

#ifdef __cplusplus
}
#endif

#endif
