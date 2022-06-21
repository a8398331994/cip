#include <stdio.h>
#include <stdlib.h>
#include "cip_img.h"

cipImgPtr cipImageCreate(int width, int height)
{
    cipImgPtr img;

    img = (cipImgPtr) calloc(1, sizeof(cipImg));
    if (!img)
        return NULL;

    img->width = width;
    img->height = height;

    img->depth = 0;
    img->channel = 0;

    return img;
}

void cipImageDestory(cipImgPtr img)
{
    int i;
    if (img->pixels) {
        
    }

    if (img->filename) {
        
    }

    free(img);
}

void cipFree(cipImgPtr img)
{

}
