#ifndef _MAT_H_
#define _MAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct _Mat_{
    int w;
    int h;
    int c;
    
    // element size in bytes
    // 4 = float32/int32
    // 2 = float16
    // 1 = int8/uint8
    // 0 = empty
    size_t elemsize;

    // packed count inside element
    // c/1-d-h-w-1  c/1-h-w-1  h/1-w-1  w/1-1  scalar
    // c/4-d-h-w-4  c/4-h-w-4  h/4-w-4  w/4-4  sse/neon
    // c/8-d-h-w-8  c/8-h-w-8  h/8-w-8  w/8-8  avx/fp16
    int elempack;

    size_t cstep;

    int dims;

    // pointer to the data
    void *data;

}Mat;


Mat *Mat_1D_create(int _w, size_t _elemsize, int _elempack);
Mat *Mat_2D_create(int _w, int _h, size_t _elemsize, int _elempack);
Mat *Mat_3D_create(int _w, int _h, int _c, size_t _elemsize, int _elempack);

int Mat_destroy(Mat *mat);

void Mat_1D_float_printf(Mat *mat);
void Mat_2D_float_printf(Mat *mat);
void Mat_2D_float_fill_zeros(Mat *mat);
void Mat_2D_float_move_one_left_column(Mat *mat);
void Mat_2D_float_move_insert_right_column(Mat *mat, float *input);
int Mat_2D_float_add(Mat *mat1, Mat *mat2);
void Mat_3D_Channel_float_printf(Mat *mat, int channel);

inline unsigned char *Mat_channel(Mat *mat, int _c) {
    return (unsigned char *)mat->data + mat->cstep * _c * mat->elemsize;
}

inline unsigned char *Mat_row(Mat *mat, int y) {
    return (unsigned char *)mat->data + (size_t)mat->w * y * mat->elemsize;
}

inline unsigned char *Mat_channel_row(Mat *mat, int _c, int y) {
    unsigned char *channel_data = (unsigned char *)mat->data + mat->cstep * _c * mat->elemsize;
    return (unsigned char *)channel_data + (size_t)mat->w * y * mat->elemsize;
}

inline unsigned char *Mat_data_row(unsigned char *data, int y, int w, int elemsize) {
    return (unsigned char *)data + (size_t)w * y * elemsize;
}


#ifdef __cplusplus
}
#endif

#endif