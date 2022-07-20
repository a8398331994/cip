#ifndef CIP_COLOR_H
#define CIP_COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cip_img.h"

#define CIP_BGR_TO_GRAY 0
#define CIP_RGB_TO_GRAY 1
#define CIP_BGR_TO_HSV 2
#define CIP_HSV_TO_BGR 3
#define CIP_BGR_TO_YUV 4
#define CIP_YUV_TO_BGR 5


//enum CIP_COLOR {
//   CIP_BGR_TO_GRAY = 0,
//   CIP_RGB_TO_GRAY,
//   CIP_BGR_TO_HSV, 
//   CIP_HSV_TO_BGR, 
//   CIP_BGR_TO_YUV, 
//   CIP_YUV_TO_BGR 
//};

/*
 * TODO: Not implement yet.
 */
cip_img_ptr cip_convert_to(cip_img_ptr input, int flag);


/*
 * TODO: Not implement yet. 
 */
static cip_img_ptr __bgr_to_gray(cip_img_ptr img);

/* 
 * TODO: Not implement yet.
 */
static cip_img_ptr __rgb_to_gray(cip_img_ptr img);

#ifdef __cplusplus
}
#endif

#endif
