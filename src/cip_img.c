#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cip_img.h"

/* Create basic image data structure. */
cip_img_ptr cip_create_img(int width, int height, int channel, int depth)
{
    cip_img_ptr res = cip_create_empty_img(width, height, channel, depth);

    if (res) {
        if (!cip_allocate_img_buf(&res)) {
            cip_free(res);
            return NULL; 
        }
    }

    return res;
}

/* Create empty image data structure without buffer memory*/
cip_img_ptr cip_create_empty_img(int width, int height, int channel, int depth)
{
    /* TODO: Add more exception handler to check different scenery*/
    if (channel > 4)
        printf("Channel can not excess 4.\n");
    if (depth > 12)
        printf("Depth can not excess 12.\n");
    
    cip_img_ptr res = (cip_img_ptr) malloc(sizeof(cip_img));
    if (!res) 
        return NULL;
   
    // Use designated initialiser
    *res = (cip_img) {
        .buf = NULL,
        .width = width,
        .height = height,
        .depth = depth,
        .channel = channel
    };

    return res;
}

/* Allocate image buffer data in cip image*/
int cip_allocate_img_buf(cip_img_ptr *input)
{
    /* Use calloc to initilize buf value to 0.*/
    (*input)->buf = calloc((*input)->width * (*input)->height * (*input)->channel, sizeof(cip_buf));
    //(*input)->buf = (cip_buf *)malloc((*input)->width * (*input)->height * (*input)->channel * sizeof(cip_buf));

    if (!(*input)->buf)
       return 0;
    return 1; 
}


/* Copy destination image*/
cip_img_ptr cip_copy_img(cip_img_ptr input)
{
    cip_img_ptr res = cip_create_img(input->width, input->height, input->channel, input->depth);

    /* if input image buf is existed and allocate res image 
     * buffer succsss.*/
    if (res && cip_allocate_img_buf(&res)) {
        memcpy(res->buf, input->buf, input->width * input->height * input->channel * sizeof(cip_buf));
    }

    return res;
}

/* Destory image */
void cip_destory_img(cip_img_ptr img)
{
    if (img->buf) {
        free(img->buf);     
    }

    cip_free(img);

}

/* */
void cip_free(cip_img_ptr img)
{
    free(img);
}
