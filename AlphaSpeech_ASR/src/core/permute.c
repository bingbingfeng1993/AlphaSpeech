#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"

#include "permute.h"

int Permute_process(Mat *in, Mat *out, int order_type) {
    int w = in->w;
    int h = in->h;
    int channels = in->c;

    size_t elemsize = in->elemsize;

    int dims = in->dims;

    if (dims == 2) {
        // order_type
        // 0 = w h
        // 1 = h w
        if (order_type == 1) {
            if (in->w != out->h && in->h != out->w) {
                printf("error permute in and out mat w h is wrong!\n");
                return -1;
            }
            float *outptr = out->data;

            for (int i = 0; i < w; i ++) {
                for (int j = 0; j < h; j ++) {
                    *(outptr ++) = ((float *)Mat_row(in, j))[i];
                }
            }
        }
    }

    if (dims == 3) {
        // order_type
        // 0 = w h c
        // 1 = h w c
        // 2 = w c h
        // 3 = c w h
        // 4 = h c w
        // 5 = c h w

        if (order_type == 1) {
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int q = 0; q < channels; q ++) {
                float *outptr = (float *)Mat_channel(out, q);

                for (int i = 0; i < w; i ++) {
                    for (int j = 0; j < h; j ++) {
                        *(outptr ++) = ((float *)Mat_channel_row(in, q, j))[i];
                    }
                }
            }
        }
        if (order_type == 2) {
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int q = 0; q < h; q ++) {
                float *outptr = (float *)Mat_channel(out, q);

                for (int i = 0; i < channels; i ++) {
                    const float *ptr = (float *)Mat_channel_row(in, i, q);

                    for (int j = 0; j < w; j ++) {
                        *(outptr ++) = ptr[j];
                    }
                }
            }
        }
        if (order_type == 3) {
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int q = 0; q < h; q ++) {
                float *outptr = (float *)Mat_channel(out, q);

                for (int i = 0; i < w; i ++) {
                    for (int j = 0; j < channels; j ++) {
                        *(outptr ++) = ((float *)Mat_channel_row(in, j, q))[i];
                    }
                }
            }
        }
        if (order_type == 4) {
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int q = 0; q < w; q ++) {
                float *outptr = (float *)Mat_channel(out, q);

                for (int i = 0; i < channels; i ++) {
                    for (int j = 0; j < h; j ++) {
                        *(outptr ++) = ((float *)Mat_channel_row(in, i, j))[q];
                    }
                }
            }
        }
        if (order_type == 5) {
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int q = 0; q < w; q ++) {
                float *outptr = (float *)Mat_channel(out, q);

                for (int i = 0; i < h; i ++) {
                    for (int j = 0; j < channels; j ++) {
                        *(outptr ++) = ((float *)Mat_channel_row(in, j, i))[q];
                    }
                }
            }
        }
    }

    return 0;
}

