#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"

#include "prelu.h"

PReLU *PReLU_create(int num_slope) {
    PReLU *prelu = (PReLU *)malloc(sizeof(PReLU));
    if (prelu == NULL) {
        return NULL;
    }

    prelu->num_slope = num_slope;
    prelu->slope_data_mat = Mat_1D_create(num_slope, 4u, 1);

    return prelu;
}

int PReLU_load_variables(PReLU *prelu, char *file) {
    if (prelu == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(prelu->slope_data_mat->data, sizeof(float), prelu->num_slope, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }

    fclose(weight_bin_file);

    return 0;
}

int PReLU_dump_header(PReLU *prelu, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name) {
    if (prelu == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(param_file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(prelu->slope_data_mat->data, sizeof(float), prelu->num_slope, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }

    fclose(weight_bin_file);

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "weight");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "weight");
    float *data_ptr = prelu->slope_data_mat->data;
    for (int i = 0; i < prelu->num_slope; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }

    fprintf(source_out_file, "};\n\n");

    return 0;
}

int PReLU_init_variables(PReLU *prelu, float *weight) {
    if (prelu == NULL) {
        return -1;
    }

    memcpy(prelu->slope_data_mat->data, weight, sizeof(float) * prelu->num_slope);

    return 0;
}

int PReLU_process_inplace(PReLU *prelu, Mat *input) {
    if (prelu == NULL) {
        return -1;
    }

    int dims = input->dims;

    if (dims == 1) {
        int w = input->w;

        float *ptr = (float *)input->data;

        if (prelu->num_slope > 1) {
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int i = 0; i < w; i ++) {
                if (ptr[i] < 0)
                    ptr[i] *= ((float *)prelu->slope_data_mat->data)[i];
            }
        } else {
            float slope = ((float *)prelu->slope_data_mat->data)[0];

            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int i = 0; i < w; i ++) {
                if (ptr[i] < 0)
                    ptr[i] *= slope;
            }
        }
    }

    if (dims == 2) {
        int w = input->w;
        int h = input->h;

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < h; i ++) {
            float *ptr = (float *)Mat_row(input, i);
            float slope = prelu->num_slope > 1 ? \
                        ((float *)prelu->slope_data_mat->data)[i] : ((float *)prelu->slope_data_mat->data)[0];

            for (int j = 0; j < w; j ++) {
                if (ptr[j] < 0)
                    ptr[j] *= slope;
            }
        }
    }

    if (dims == 3) {
        int w = input->w;
        int h = input->h;
        int channels = input->c;
        int size = w * h;

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int q = 0; q < channels; q ++) {
            float *ptr = (float *)Mat_channel(input, q);
            float slope = prelu->num_slope > 1 ? \
                          ((float *)prelu->slope_data_mat->data)[q] : ((float *)prelu->slope_data_mat->data)[0];

            for (int i = 0; i < size; i ++) {
                if (ptr[i] < 0)
                    ptr[i] *= slope;
            }
        }
    }
    
    return 0;
}

int PReLU_destroy(PReLU *prelu) {
    if (prelu != NULL) {
        Mat_destroy(prelu->slope_data_mat);

        free(prelu);
        prelu = NULL;
    }

    return 0;
}