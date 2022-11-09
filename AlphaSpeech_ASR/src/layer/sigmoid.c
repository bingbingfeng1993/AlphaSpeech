#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"

#include "sigmoid.h"

Sigmoid *Sigmoid_create() {
    Sigmoid *sigmoid = (Sigmoid *)malloc(sizeof(Sigmoid));
    if (sigmoid == NULL) {
        return NULL;
    }

    return sigmoid;
}

int Sigmoid_process_inplace(Sigmoid *sigmoid, Mat *input) {
    if (sigmoid == NULL) {
        return -1;
    }

    int size = input->w * input->h;

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int q = 0; q < input->c; q ++) {
        float *ptr =  (float *)Mat_channel(input, q);

        for (int i = 0; i < size; i ++) {
            ptr[i] = (float)(1.f / (1.f + exp(-ptr[i])));
        }
    }

    return 0;
}

int Sigmoid_destroy(Sigmoid *sigmoid) {
    if (sigmoid != NULL) {
        free(sigmoid);
        sigmoid = NULL;
    }

    return 0;
}