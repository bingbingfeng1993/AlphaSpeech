#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "core/macros.h"

#include "softmax.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

Softmax *Softmax_create(int axis) {
    Softmax *softmax = (Softmax *)malloc(sizeof(Softmax));
    if (softmax == NULL) {
        return NULL;
    }

    softmax->axis = axis;

    return softmax;
}

int Softmax_process_inplace(Softmax *softmax, Mat *input) {
    if (softmax == NULL) {
        return -1;
    }

    int dims = input->dims;
    size_t elemsize = input->elemsize;
    int positive_axis = softmax->axis < 0 ? dims + softmax->axis : softmax->axis;

    if (dims == 1) {
        int w = input->w;

        float *ptr = (float *)input->data;

        float max = -FLT_MAX;
        for (int i = 0; i < w; i ++) {
            max = MAX(max, ptr[i]);
        }

        float sum = 0.f;
        for (int i = 0; i < w; i ++) {
            ptr[i] = (float)(exp(ptr[i] - max));
            sum += ptr[i];
        }

        for (int i = 0; i < w; i ++) {
            ptr[i] /= sum;
        }
    }    

    if (dims == 2 && positive_axis == 0) {
        int w = input->w;
        int h = input->h;

        Mat *max = Mat_1D_create(w, elemsize, 1);
        for (int i = 0; i < max->w; i ++) {
            ((float *)max->data)[i] = -FLT_MAX;
        }

        for (int i = 0; i < h; i ++) {
            const float *ptr = (float *)Mat_row(input, i);
            for (int j = 0; j < w; j ++) {
                ((float *)max->data)[j] = MAX(((float *)max->data)[j], ptr[j]);
            }
        }

        Mat *sum = Mat_1D_create(w, elemsize, 1);
        for (int i = 0; i < sum->w; i ++) {
            ((float *)sum->data)[i] = 0.f;
        }

        for (int i = 0; i < h; i ++) {
            float *ptr = (float *)Mat_row(input, i);
            for (int j = 0; j < w; j ++) {
                ptr[j] = (float)(exp(ptr[j] - ((float *)max->data)[j]));
                ((float *)sum->data)[j] += ptr[j];
            }
        }

        for (int i = 0; i < h; i ++) {
            float *ptr = (float *)Mat_row(input, i);
            for (int j = 0; j < w; j ++) {
                ptr[j] /= ((float *)sum->data)[j];
            }
        }

        Mat_destroy(max);
        Mat_destroy(sum);
    }

    if (dims == 2 && positive_axis == 1) {
        int w = input->w;
        int h = input->h;

        for (int i = 0; i < h; i ++) {
            float *ptr = (float *)Mat_row(input, i);
            float m = -FLT_MAX;
            for (int j = 0; j < w; j ++) {
                m = MAX(m, ptr[j]);
            }

            float s = 0.f;
            for (int j = 0; j < w; j ++) {
                ptr[j] = (float)(exp(ptr[j] - m));
                s += ptr[j];
            }

            for (int j = 0; j < w; j ++) {
                ptr[j] /= s;
            }
        }
    }

    if (dims == 3 && positive_axis == 0) {
        int w = input->w;
        int h = input->h;
        int channels = input->c;
        int size = w * h;

        Mat *max = Mat_2D_create(w, h, elemsize, 1);
        float *max_data = (float *)max->data;
        for (int i = 0; i < max->h; i ++) {
            for (int j = 0; j < max->w; j ++) {
                max_data[i * max->w + j] = -FLT_MAX;
            }
        }

        for (int q = 0; q < channels; q ++) {
            const float *ptr = (float *)Mat_channel(input, q);

            for (int i = 0; i < size; i ++) {
                max_data[i] = MAX(max_data[i], ptr[i]);
            }
        }

        Mat *sum = Mat_2D_create(w, h, elemsize, 1);
        float *sum_data = (float *)sum->data;
        Mat_2D_float_fill_zeros(sum);

        for (int q = 0; q < channels; q ++) {
            float *ptr = (float *)Mat_channel(input, q);

            for (int i = 0; i < size; i ++) {
                ptr[i] = (float)(exp(ptr[i] - max_data[i]));
                sum_data[i] += ptr[i];
            }
        }

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int q = 0; q < channels; q ++) {
            float *ptr = (float *)Mat_channel(input, q);

            for (int i = 0; i < size; i ++) {
                ptr[i] /= sum_data[i];
            }
        }

        Mat_destroy(max);
        Mat_destroy(sum);
    }

    if (dims == 3 && positive_axis == 1) {
        int w = input->w;
        int h = input->h;
        int channels = input->c;

        Mat *max = Mat_2D_create(w, channels, elemsize, 1);
        float *max_data = (float *)max->data;
        for (int i = 0; i < max->h; i ++) {
            for (int j = 0; j < max->w; j ++) {
                max_data[i * max->w + j] = -FLT_MAX;
            }
        }

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int q = 0; q < channels; q ++) {
            const float *ptr = (float *)Mat_channel(input, q);
            float *maxptr = (float *)Mat_row(max, q);

            for (int i = 0; i < h; i ++) {
                for (int j = 0; j < w; j ++) {
                    maxptr[j] = MAX(maxptr[j], ptr[j]);
                }

                ptr += w;
            }
        }

        Mat *sum = Mat_2D_create(w, channels, elemsize, 1);
        float *sum_data = (float *)sum->data;
        Mat_2D_float_fill_zeros(sum);

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int q = 0; q < channels; q ++) {
            float *ptr = (float *)Mat_channel(input, q);
            float *maxptr = (float *)Mat_row(max, q);
            float *sumptr = (float *)Mat_row(sum, q);

            for (int i = 0; i < h; i ++) {
                for (int j = 0; j < w; j ++) {
                    ptr[j] = (float)(exp(ptr[j] - maxptr[j]));
                    sumptr[j] += ptr[j];
                }

                ptr += w;
            }
        }

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int q = 0; q < channels; q ++) {
            float *ptr = (float *)Mat_channel(input, q);
            float *sumptr = (float *)Mat_row(sum, q);

            for (int i = 0; i < h; i ++) {
                for (int j = 0; j < w; j ++) {
                    ptr[j] /= sumptr[j];
                }

                ptr += w;
            }
        }

        Mat_destroy(max);
        Mat_destroy(sum);
    }

    if (dims == 3 && positive_axis == 2) {
        int w = input->w;
        int h = input->h;
        int channels = input->c;

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int q = 0; q < channels; q ++) {
            float *ptr = (float *)Mat_channel(input, q);

            for (int i = 0; i < h; i ++) {
                float max = -FLT_MAX;
                for (int j = 0; j < w; j ++) {
                    max = MAX(max, ptr[j]);
                }

                float sum = 0.f;
                for (int j = 0; j < w; j ++) {
                    ptr[j] = (float)(exp(ptr[j] - max));
                    sum += ptr[j];
                }

                for (int j = 0; j < w; j ++) {
                    ptr[j] /= sum;
                }

                ptr += w;
            }
        }
    }
    
    return 0;
}

int Softmax_destroy(Softmax *softmax) {
    if (softmax != NULL) {
        free(softmax);
        softmax = NULL;
    }
    
    return 0;
}