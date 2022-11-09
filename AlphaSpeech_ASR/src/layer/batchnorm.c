#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"
#include "batchnorm.h"

BatchNorm *BatchNorm_create(int channels) {
    BatchNorm *bn = (BatchNorm *)malloc(sizeof(BatchNorm));
    if (bn == NULL) {
        return NULL;
    }

    bn->channels = channels;

    bn->slope_data_mat = Mat_2D_create(channels, 1, 4u, 1);
    bn->mean_data_mat = Mat_2D_create(channels, 1, 4u, 1);
    bn->var_data_mat = Mat_2D_create(channels, 1, 4u, 1);
    bn->bias_data_mat = Mat_2D_create(channels, 1, 4u, 1);

    bn->a_data_mat = Mat_1D_create(channels, 4u, 1);
    bn->b_data_mat = Mat_1D_create(channels, 4u, 1);

    return bn;
}

int BatchNorm_load_variables(BatchNorm *bn, char *file) {
    if (bn == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;

    ret = fread(bn->slope_data_mat->data, sizeof(float), bn->channels, weight_bin_file);
    ret = fread(bn->bias_data_mat->data, sizeof(float), bn->channels, weight_bin_file);
    ret = fread(bn->mean_data_mat->data, sizeof(float), bn->channels, weight_bin_file);
    ret = fread(bn->var_data_mat->data, sizeof(float), bn->channels, weight_bin_file);

    float eps = 0.00001;

    for (int i = 0; i < bn->channels; i ++) {
        float sqrt_var = sqrt(((float *)bn->var_data_mat->data)[i] + eps);
        
        ((float *)bn->a_data_mat->data)[i] = ((float *)bn->bias_data_mat->data)[i] - \
                                             ((float *)bn->slope_data_mat->data)[i] * ((float *)bn->mean_data_mat->data)[i] / sqrt_var;
        
        ((float *)bn->b_data_mat->data)[i] = ((float *)bn->slope_data_mat->data)[i] / sqrt_var;
    }

    fclose(weight_bin_file);

    return 0;
}

int BatchNorm_dump_header(BatchNorm *bn, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name) {
    if (bn == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(param_file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;

    ret = fread(bn->slope_data_mat->data, sizeof(float), bn->channels, weight_bin_file);
    ret = fread(bn->bias_data_mat->data, sizeof(float), bn->channels, weight_bin_file);
    ret = fread(bn->mean_data_mat->data, sizeof(float), bn->channels, weight_bin_file);
    ret = fread(bn->var_data_mat->data, sizeof(float), bn->channels, weight_bin_file);

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "weight");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "weight");
    float *data_ptr = bn->slope_data_mat->data;
    for (int i = 0; i < bn->channels; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "bias");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "bias");
    data_ptr = bn->bias_data_mat->data;
    for (int i = 0; i < bn->channels; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "mean");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "mean");
    data_ptr = bn->mean_data_mat->data;
    for (int i = 0; i < bn->channels; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "var");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "var");
    data_ptr = bn->var_data_mat->data;
    for (int i = 0; i < bn->channels; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fclose(weight_bin_file);

    return 0;
}

int BatchNorm_init_variables(BatchNorm *bn, float *weight, float *bias, float *mean, float *var) {
    if (bn == NULL) {
        return -1;
    }

    memcpy(bn->slope_data_mat->data, weight, sizeof(float) * bn->channels);
    memcpy(bn->bias_data_mat->data, bias, sizeof(float) * bn->channels);
    memcpy(bn->mean_data_mat->data, mean, sizeof(float) * bn->channels);
    memcpy(bn->var_data_mat->data, var, sizeof(float) * bn->channels);

    float eps = 0.00001;

    for (int i = 0; i < bn->channels; i ++) {
        float sqrt_var = sqrt(((float *)bn->var_data_mat->data)[i] + eps);
        
        ((float *)bn->a_data_mat->data)[i] = ((float *)bn->bias_data_mat->data)[i] - \
                                             ((float *)bn->slope_data_mat->data)[i] * ((float *)bn->mean_data_mat->data)[i] / sqrt_var;

        ((float *)bn->b_data_mat->data)[i] = ((float *)bn->slope_data_mat->data)[i] / sqrt_var;
    }

    return 0;
}

int BatchNorm_process_inplace(BatchNorm *bn, Mat *input) {
    if (bn == NULL) {
        return -1;
    }

    // a = bias - slope * mean / sqrt(var)
    // b = slope / sqrt(var)
    // value = b * value + a

    int dims = input->dims;

    if (dims == 1) {
        int w = input->w;

        float *ptr = (float *)input->data;

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < w; i ++) {
            ptr[i] = ((float *)bn->b_data_mat->data)[i] * ptr[i] + ((float *)bn->a_data_mat->data)[i];
        }
    }

    if (dims == 2) {
        int w = input->w;
        int h = input->h;

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < h; i ++) {
            float *ptr = (float *)Mat_row(input, i);
            float a = ((float *)bn->a_data_mat->data)[i];
            float b = ((float *)bn->b_data_mat->data)[i];

            for (int j = 0; j < w; j ++) {
                ptr[j] = b * ptr[j] + a;
            }
        }
    }

    if (dims == 3) {
        int w = input->w;
        int h = input->h;
        int size = w * h;

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int q = 0; q < bn->channels; q ++) {
            float *ptr = (float *)Mat_channel(input, q);
            float a = ((float *)bn->a_data_mat->data)[q];
            float b = ((float *)bn->b_data_mat->data)[q];

            for (int i = 0; i < size; i ++) {
                ptr[i] = b * ptr[i] + a;
            }
        }
    }
    
    return 0;
}


int BatchNorm_destroy(BatchNorm *bn) {
    if (bn != NULL) {
        Mat_destroy(bn->slope_data_mat);
        Mat_destroy(bn->mean_data_mat);
        Mat_destroy(bn->var_data_mat);
        Mat_destroy(bn->bias_data_mat);

        Mat_destroy(bn->a_data_mat);
        Mat_destroy(bn->b_data_mat);

        free(bn);
        bn = NULL;
    }

    return 0;
}