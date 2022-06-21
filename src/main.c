#include <stdio.h>
#include "cip_img.h"
#include "cip_bmp.h"

int main()
{
   
    cipImgPtr i = cipCreateFromBmp("test/sample.bmp");

    cipImgPtr t = cipImageCreate(1000, 800);

    printf("%d \n", t->width);
//    printf("%d \n", i->width);
}
