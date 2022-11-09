#include <stdio.h>

#include "macros.h"
#include "fastmalloc.h"
#include "mat.h"

inline size_t total(Mat *mat) {
    return mat->cstep * mat->c;
}

Mat *Mat_1D_create(int _w, size_t _elemsize, int _elempack) {
    Mat *mat = (Mat *)malloc(sizeof(Mat));
    if (mat == NULL) {
        return NULL;
    }

    mat->elemsize = _elemsize;
    mat->elempack = _elempack;

    mat->dims = 1;
    mat->w = _w;
    mat->h = 1;
    mat->c = 1;

    mat->cstep = mat->w;

    if (total(mat) > 0) {
        size_t totalsize = alignSize(total(mat) * mat->elemsize, 4);
        mat->data = fastMalloc(totalsize);
    }

    return mat;
}

void Mat_1D_float_printf(Mat *mat) {
    float *mat_data = (float *)mat->data;
    for (int i = 0; i < mat->w; i ++) {
        printf("%f ", mat_data[i]);
    }
    printf("\n");
}

Mat *Mat_2D_create(int _w, int _h, size_t _elemsize, int _elempack) {
    Mat *mat = (Mat *)malloc(sizeof(Mat));
    if (mat == NULL) {
        return NULL;
    }

    mat->elemsize = _elemsize;
    mat->elempack = _elempack;

    mat->dims = 2;
    mat->w = _w;
    mat->h = _h;
    mat->c = 1;

    mat->cstep = (size_t)mat->w * mat->h;

    if (total(mat) > 0) {
        size_t totalsize = alignSize(total(mat) * mat->elemsize, 4);
        mat->data = fastMalloc(totalsize);
    }

    return mat;
}

void Mat_2D_float_fill_zeros(Mat *mat) {
    float *data_ptr = (float *)mat->data;

    for (int i = 0; i < mat->h; i ++) {
        for (int j = 0; j < mat->w; j ++) {
            data_ptr[i * mat->w + j] = 0.0f;
        }
    }
}

void Mat_2D_float_move_one_left_column(Mat *mat) {
    float *data_ptr = (float *)mat->data;
    
    for (int i = 0; i < mat->h; i ++) {
        for (int j = 0; j < mat->w - 1; j ++) {
            data_ptr[i * mat->w + j] = data_ptr[i * mat->w + j + 1];
        }
    }
}

void Mat_2D_float_move_insert_right_column(Mat *mat, float *input){
    float *data_ptr = (float *)mat->data;
    
    for (int i = 0; i < mat->h; i ++) {
        data_ptr[i * mat->w + mat->w - 1] = *input;
        input ++;
    }
}

int Mat_2D_float_add(Mat *mat1, Mat *mat2){
    if (mat1->w != mat2->w || mat1->h != mat2->h || mat1->elempack != mat2->elempack) {
        printf("Mat_2D_float_add error\n");
        return -1;
    }

    float *mat1_data = (float *)mat1->data;
    float *mat2_data = (float *)mat2->data;

    if (mat1->elempack == 1) {
        for (int i = 0; i < mat1->h; i ++) {
            for (int j = 0; j < mat1->w; j ++) {
                mat2_data[i * mat2->w + j] = mat1_data[i * mat1->w + j] + mat2_data[i * mat2->w + j];
            }
        }
    } else if (mat1->elempack == ARM_PACKING_NUM) {
        for (int i = 0; i < mat1->h; i ++) {
            for (int j = 0; j < mat1->w * ARM_PACKING_NUM; j ++) {
                mat2_data[i * mat2->w * ARM_PACKING_NUM + j] = mat1_data[i * mat1->w * ARM_PACKING_NUM + j] + mat2_data[i * mat2->w * ARM_PACKING_NUM + j];
            }
        }
    }

    return 0;
}


void Mat_2D_float_printf(Mat *mat) {
    float *mat_data = (float *)mat->data;

    if (mat->elempack == 1) {
        for (int i = 0; i < mat->h; i ++) {
            printf("%d\n", i);
            for (int j = 0; j < mat->w; j ++) {
                printf("%g ", mat_data[i * mat->w + j]);
            }
            printf("\n");
        }
    } else if (mat->elempack == ARM_PACKING_NUM) {
        // 可以和上面的合并
        for (int i = 0; i < mat->h; i ++) {
            for (int j = 0; j < mat->w * ARM_PACKING_NUM; j ++) {
                // printf("%d ", i * mat->w * ARM_PACKING_NUM + j);
                printf("%g ", mat_data[i * mat->w * ARM_PACKING_NUM + j]);
            }
            printf("\n");
        }
    }
}

Mat *Mat_3D_create(int _w, int _h, int _c, size_t _elemsize, int _elempack) {
    Mat *mat = (Mat *)malloc(sizeof(Mat));
    if (mat == NULL) {
        return NULL;
    }

    mat->elemsize = _elemsize;
    mat->elempack = _elempack;

    mat->dims = 3;
    mat->w = _w;
    mat->h = _h;
    mat->c = _c;

    mat->cstep = alignSize((size_t)mat->w * mat->h * mat->elemsize, 16) / mat->elemsize;

    if (total(mat) > 0) {
        size_t totalsize = alignSize(total(mat) * mat->elemsize, 4);
        mat->data = fastMalloc(totalsize);
    }

    return mat;
}

void Mat_3D_Channel_float_printf(Mat *mat, int channel) {
    float *mat_data = (float *)Mat_channel(mat, channel);

    if (mat->elempack == 1) {
        for (int i = 0; i < mat->h; i ++) {
            printf("%d\n", i);
            for (int j = 0; j < mat->w; j ++) {
                printf("%f ", mat_data[i * mat->w + j]);
            }
            printf("\n");
        }
    } else if (mat->elempack == ARM_PACKING_NUM) {
        // TODO
    }
}

int Mat_destroy(Mat *mat) {
    if (mat != NULL) {
        fastFree(mat->data);
        mat->data = NULL;
        free(mat);
        mat = NULL;
    }
    
    return 0;
}