#include <stdio.h>
#include "cip_img.h"
#include "cip_bmp.h"

int main()
{
       
    cipImgPtr i = cipCreateFromBmp("test/sample.bmp");

    printf("%d \n", i->width);

    cipSaveToBmp("test/save.bmp", i); 
}
