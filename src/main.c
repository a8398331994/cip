#include <stdio.h>
#include "cip_img.h"
#include "cip_bmp.h"
#include "cip_color.h"

int main()
{
       
    cip_img_ptr i = cip_img_load_from_bmp("test/sample.bmp");

    printf("%d \n", i->width);

    
//    cip_img_ptr gray = cip_convert_to(i, CIP_BGR_TO_GRAY); 

    cip_img_save_to_bmp("test/gray.bmp", i); 
}
