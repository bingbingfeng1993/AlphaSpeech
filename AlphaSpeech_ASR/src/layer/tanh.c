#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"

#include "tanh.h"

Tanh *Tanh_create() {
    Tanh *tanh_obj = (Tanh *)malloc(sizeof(Tanh));
    if (tanh_obj == NULL) {
        return NULL;
    }

    return tanh_obj;
}

int Tanh_process_inplace(Tanh *tanh_obj, Mat *input) {
    if (tanh_obj == NULL) {
        return -1;
    }
    
    int w = input->w;
    int h = input->h;
    int channels = input->c;
    int size = w * h;

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int q = 0; q < channels; q ++) {
        float *ptr = (float *)Mat_channel(input, q);

        for (int i = 0; i < size; i ++) {
            ptr[i] = (float)(tanh(ptr[i]));
        }
    }
    
    return 0;
}

int Tanh_destroy(Tanh *tanh_obj) {
    if (tanh_obj != NULL) {
        free(tanh_obj);
        tanh_obj = NULL;
    }
    
    return 0;
}