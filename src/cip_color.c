#include <stdio.h>
#include "cip_color.h"

cip_img_ptr cip_convert_to(cip_img_ptr input, int flag)
{

    cip_img_ptr res = NULL;

    switch (flag) {
    
    case CIP_BGR_TO_GRAY:
        res = __bgr_to_gray(input);
        break;
    case CIP_RGB_TO_GRAY:
        res = __rgb_to_gray(input);
        break;

    default:
        printf("Not supported mode to convert specific color.\n");
    }

    return res;
}


static cip_img_ptr __bgr_to_gray(cip_img_ptr input)
{
    /* Check it already gray scale or not. */
    if (input->channel != 3)
        return cip_copy_img(input);

    cip_img_ptr res = cip_create_img(input->width, input->height, 1, input->depth);
    int ypos, xpos, ch;

    // bgr ordering
    float scale[] = {0.114, 0.587, 0.299};
    for (ypos = 0; ypos < input->height; ypos++) {
        for (xpos = 0; xpos < input->width; xpos++) {
            for (ch = 0; ch < input->channel; ch++) {
                float val = cip_get_img_buf_norm(input, xpos, ypos, ch) * scale[ch];
                cip_add_img_buf_norm(res, xpos, ypos, 0, val);   
            }
        }   
    }

    return res;
}

static cip_img_ptr __rgb_to_gray(cip_img_ptr input)
{

}
