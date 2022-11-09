#ifndef _GLU_H_
#define _GLU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _GLU_{
    int dim;
    Mat *out_mat;

}GLU;


GLU *GLU_create(int dim);
int GLU_destroy(GLU *glu);
int GLU_process(GLU *glu, Mat *input);
Mat *GLU_get_output(GLU *glu);


#ifdef __cplusplus
}
#endif

#endif