#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "crop.h"

int Crop_2D_float_process(Mat *src, Mat *dst, int top, int left) {

    int w = dst->w;
    int h = dst->h;

    const float *ptr = (float *)Mat_row(src, top) + left;
    float *outptr = (float *)dst->data;

    for (int y = 0; y < h; y ++) {
        if (w < 12) {
            for (int x = 0; x < w; x ++) {
                outptr[x] = ptr[x];
            }
        } else {
            memcpy(outptr, ptr, w * sizeof(float));
        }
        outptr += w;
        ptr += src->w;
    }

    return 0;
}
