#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"

#include "relu.h"

ReLU *ReLU_create() {
    ReLU *relu = (ReLU *)malloc(sizeof(ReLU));
    if (relu == NULL) {
        return NULL;
    }

    return relu;
}

int ReLU_process_inplace(ReLU *relu, Mat *input) {
    if (relu == NULL) {
        return -1;
    }

    int size = input->w * input->h;

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int q = 0; q < input->c; q ++) {
        float *ptr = (float *)Mat_channel(input, q);

        for (int i = 0; i < size; i ++) {
            if (ptr[i] < 0)
                ptr[i] = 0;
        }
    }

    return 0;
}

int ReLU_destroy(ReLU *relu) {
    if (relu != NULL) {
        free(relu);
        relu = NULL;
    }
    
    return 0;
}