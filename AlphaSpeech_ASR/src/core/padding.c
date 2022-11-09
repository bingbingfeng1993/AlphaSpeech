#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "padding.h"

// 0=CONSTANT 1=REPLICATE 2=REFLECT
int Padding_2D_float_process(Mat *src, Mat *dst, int top, int left, int type, float v) {

    int w = dst->w;
    int h = dst->h;

    const float *ptr = src->data;
    float *outptr = dst->data;

    if (type == 0) {
        int y = 0;
        // fill top
        for (; y < top; y ++) {
            int x = 0;
            for (; x < w; x ++) {
                outptr[x] = v;
            }
            outptr += w;
        }
        // fill center
        for (; y < (top + src->h); y ++) {
            int x = 0;
            for (; x < left; x ++) {
                outptr[x] = v;
            }
            if (src->w < 12) {
                for (; x < (left + src->w); x ++) {
                    outptr[x] = ptr[x - left];
                }
            } else {
                memcpy(outptr + left, ptr, src->w * sizeof(float));
                x += src->w;
            }
            for (; x < w; x ++) {
                outptr[x] = v;
            }
            ptr += src->w;
            outptr += w;
        }
        // fill bottom
        for (; y < h; y ++) {
            int x = 0;
            for (; x < w; x ++) {
                outptr[x] = v;
            }
            outptr += w;
        }
    }

    if (type == 1) {
        int y = 0;
        // fill top
        for (; y < top; y ++) {
            int x = 0;
            for (; x < left; x ++) {
                outptr[x] = ptr[0];
            }
            if (src->w < 12) {
                for (; x < (left + src->w); x ++) {
                    outptr[x] = ptr[x - left];
                }
            } else {
                memcpy(outptr + left, ptr, src->w * sizeof(float));
                x += src->w;
            }
            for (; x < w; x ++) {
                outptr[x] = ptr[src->w - 1];
            }
            outptr += w;
        }
        // fill center
        for (; y < (top + src->h); y ++) {
            int x = 0;
            for (; x < left; x ++) {
                outptr[x] = ptr[0];
            }
            if (src->w < 12) {
                for (; x < (left + src->w); x ++) {
                    outptr[x] = ptr[x - left];
                }
            } else {
                memcpy(outptr + left, ptr, src->w * sizeof(float));
                x += src->w;
            }
            for (; x < w; x ++) {
                outptr[x] = ptr[src->w - 1];
            }
            ptr += src->w;
            outptr += w;
        }
        // fill bottom
        ptr -= src->w;
        for (; y < h; y ++) {
            int x = 0;
            for (; x < left; x ++) {
                outptr[x] = ptr[0];
            }
            if (src->w < 12) {
                for (; x < (left + src->w); x ++) {
                    outptr[x] = ptr[x - left];
                }
            } else{
                memcpy(outptr + left, ptr, src->w * sizeof(float));
                x += src->w;
            }
            for (; x < w; x ++) {
                outptr[x] = ptr[src->w - 1];
            }
            outptr += w;
        }
    }

    if (type == 2) {
        int y = 0;
        // fill top
        ptr += top * src->w;
        for (; y < top; y ++) {
            int x = 0;
            for (; x < left; x ++) {
                outptr[x] = ptr[left - x];
            }
            if (src->w < 12) {
                for (; x < (left + src->w); x ++) {
                    outptr[x] = ptr[x - left];
                }
            } else {
                memcpy(outptr + left, ptr, src->w * sizeof(float));
                x += src->w;
            }
            for (; x < w; x ++) {
                outptr[x] = ptr[src->w - (x - left - src->w) - 2];
            }
            outptr += w;
            ptr -= src->w;
        }
        // fill center
        for (; y < (top + src->h); y ++) {
            int x = 0;
            for (; x < left; x ++) {
                outptr[x] = ptr[left - x];
            }
            if (src->w < 12) {
                for (; x < (left + src->w); x ++) {
                    outptr[x] = ptr[x - left];
                }
            } else {
                memcpy(outptr + left, ptr, src->w * sizeof(float));
                x += src->w;
            }
            for (; x < w; x ++) {
                outptr[x] = ptr[src->w - (x - left - src->w) - 2];
            }
            ptr += src->w;
            outptr += w;
        }
        // fill bottom
        ptr -= 2 * src->w;
        for (; y < h; y ++) {
            int x = 0;
            for (; x < left; x ++) {
                outptr[x] = ptr[left - x];
            }
            if (src->w < 12) {
                for (; x < (left + src->w); x ++) {
                    outptr[x] = ptr[x - left];
                }
            } else {
                memcpy(outptr + left, ptr, src->w * sizeof(float));
                x += src->w;
            }
            for (; x < w; x ++) {
                outptr[x] = ptr[src->w - (x - left - src->w) - 2];
            }
            outptr += w;
            ptr -= src->w;
        }
    }

    return 0;
}
