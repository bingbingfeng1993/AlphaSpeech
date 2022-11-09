#ifndef _CROP_H_
#define _CROP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mat.h"

int Crop_2D_float_process(Mat *in, Mat *out, int top, int left);


#ifdef __cplusplus
}
#endif

#endif