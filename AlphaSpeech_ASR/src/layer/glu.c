#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"

#include "glu.h"

GLU *GLU_create(int dim) {
    GLU *glu = (GLU *)malloc(sizeof(GLU));
    if (glu == NULL) {
        return NULL;
    }

    glu->dim = dim;
    glu->out_mat = NULL;

    return glu;
}

int GLU_process(GLU *glu, Mat *input) {
    if (glu == NULL) {
        return -1;
    }

    if (input->dims == 2 && glu->dim == 0) {
        if (glu->out_mat == NULL) {
            glu->out_mat = Mat_2D_create(input->w, input->h / 2, sizeof(float), 1);
        }

        float *ptr =  (float *)input->data;
        float *out_data = (float *)glu->out_mat->data;

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < input->w; i ++) {
            for (int j = 0; j < input->h / 2; j ++) {
                out_data[j * glu->out_mat->w + i] = ptr[j * input->w + i] * (float)(1.f / (1.f + exp(-ptr[(j + input->h / 2) * input->w + i])));
            }
        }
    }

    return 0;
}

Mat *GLU_get_output(GLU *glu) {
    return glu->out_mat;
}

int GLU_destroy(GLU *glu) {
    if (glu != NULL) {
        free(glu->out_mat);
        glu->out_mat = NULL;
        free(glu);
        glu = NULL;
    }
    
    return 0;
}