#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"

#include "silu.h"

SiLU *SiLU_create() {
    SiLU *silu = (SiLU *)malloc(sizeof(SiLU));
    if (silu == NULL) {
        return NULL;
    }

    return silu;
}

int SiLU_process_inplace(SiLU *silu, Mat *input) {
    if (silu == NULL) {
        return -1;
    }

    int size = input->w * input->h;

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int q = 0; q < input->c; q ++) {
        float *ptr =  (float *)Mat_channel(input, q);

        for (int i = 0; i < size; i ++) {
            ptr[i] = ptr[i] * (float)(1.f / (1.f + exp(-ptr[i])));
        }
    }

    return 0;
}

int SiLU_destroy(SiLU *silu) {
    if (silu != NULL) {
        free(silu);
        silu = NULL;
    }

    return 0;
}