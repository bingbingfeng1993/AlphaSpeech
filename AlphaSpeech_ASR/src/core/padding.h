#ifndef _PADDING_H_
#define _PADDING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "mat.h"

// 0=CONSTANT 1=REPLICATE 2=REFLECT
int Padding_2D_float_process(Mat *in, Mat *out, int top, int left, int type, float v);


#ifdef __cplusplus
}
#endif

#endif