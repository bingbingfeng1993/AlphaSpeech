#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core/macros.h"
#include "layernorm.h"

LayerNorm *LayerNorm_create(int affine_size, bool affine) {
    LayerNorm *ln = (LayerNorm *)malloc(sizeof(LayerNorm));
    if (ln == NULL) {
        return NULL;
    }

    ln->affine_size = affine_size;
    ln->affine = affine;

    ln->gamma_data_mat = Mat_1D_create(affine_size, 4u, 1);
    ln->beta_data_mat = Mat_1D_create(affine_size, 4u, 1);

    return ln;
}

int LayerNorm_load_variables(LayerNorm *ln, char *file) {
    if (ln == NULL) {
        return -1;
    }

    if (ln->affine == false) {
        return 0;
    }

    FILE *weight_bin_file = fopen(file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;

    ret = fread(ln->gamma_data_mat->data, sizeof(float), ln->affine_size, weight_bin_file);
    ret = fread(ln->beta_data_mat->data, sizeof(float), ln->affine_size, weight_bin_file);
    // Mat_1D_float_printf(ln->beta_data_mat);

    fclose(weight_bin_file);

    return 0;
}

int LayerNorm_dump_header(LayerNorm *ln, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name) {
    if (ln == NULL) {
        return -1;
    }

    if (ln->affine == false) {
        return 0;
    }

    FILE *weight_bin_file = fopen(param_file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;

    ret = fread(ln->gamma_data_mat->data, sizeof(float), ln->affine_size, weight_bin_file);
    ret = fread(ln->beta_data_mat->data, sizeof(float), ln->affine_size, weight_bin_file);

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "weight");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "weight");
    float *data_ptr = ln->gamma_data_mat->data;
    for (int i = 0; i < ln->affine_size; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "bias");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "bias");
    data_ptr = ln->beta_data_mat->data;
    for (int i = 0; i < ln->affine_size; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fclose(weight_bin_file);

    return 0;
}

int LayerNorm_init_variables(LayerNorm *ln, float *weight, float *bias) {
    if (ln == NULL) {
        return -1;
    }

    if (ln->affine == false) {
        return 0;
    }
    
    memcpy(ln->gamma_data_mat->data, weight, sizeof(float) * ln->affine_size);
    memcpy(ln->beta_data_mat->data, bias, sizeof(float) * ln->affine_size);

    return 0;
}

int LayerNorm_process_inplace(LayerNorm *ln, Mat *input) {
    if (ln == NULL) {
        return -1;
    }

    // x = (x - mean) / sqrt(var + eps) * gamma + beta

    int dims = input->dims;
    float eps = 1e-5;
    float *gamma_data = (float *)ln->gamma_data_mat->data;
    float *beta_data = (float *)ln->beta_data_mat->data;

    if (dims == 1) {
        int w = input->w;
        // assert affine_size == w
        float *ptr = (float *)input->data;

        // mean and var
        float sum = 0.f;
        float sqsum = 0.f;
        for (int i = 0; i < w; i ++) {
            sum += ptr[i];
            // sqsum += ptr[i] * ptr[i];
        }
        float mean = sum / w;
        float tmp = 0.f;
        for (int i = 0; i < w; i ++) {
            tmp = ptr[i] - mean;
            sqsum += tmp * tmp;
        }
        float var = sqsum / w;
        // the var maybe minus due to accuracy
        // float var = sqsum / w - mean * mean;

        float a = (1.f / (sqrt(var + eps)));
        float b = -mean * a;

        if (ln->affine) {
            for (int i = 0; i < w; i ++) {
                ptr[i] = (ptr[i] * a + b) * gamma_data[i] + beta_data[i];
            }
        } else {
            for (int i = 0; i < w; i ++) {
                ptr[i] = ptr[i] * a + b;
            }
        }
    }
    
    if (dims == 2) {
        int w = input->w;
        int h = input->h;
        // assert affine_size == w

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < h; i ++) {
            float *ptr = (float *)Mat_row(input, i);

            // mean and var
            float sum = 0.f;
            float sqsum = 0.f;
            for (int j = 0; j < w; j ++) {
                sum += ptr[j];
                // sqsum += ptr[j] * ptr[j];
            }
            float mean = sum / w;
            float tmp = 0.f;
            for (int j = 0; j < w; j ++) {
                tmp = ptr[j] - mean;
                sqsum += tmp * tmp;
            }
            float var = sqsum / w;
            // the var maybe minus due to accuracy
            // float var = sqsum / w - mean * mean;

            float a = (1.f / (sqrt(var + eps)));
            float b = -mean * a;

            if (ln->affine) {
                for (int j = 0; j < w; j ++) {
                    ptr[j] = (ptr[j] * a + b) * gamma_data[j] + beta_data[j];
                }
            } else {
                for (int j = 0; j < w; j ++) {
                    ptr[j] = ptr[j] * a + b;
                }
            }
        }
    }

    if (dims == 3) {
        int w = input->w;
        int h = input->h;
        int channels = input->c;
        int size = w * h;

        if (ln->affine_size == w) {
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int q = 0; q < channels; q ++) {
                for (int i = 0; i < h; i ++) {
                    float *ptr = (float *)Mat_channel_row(input, q, i);

                    // mean and var
                    float sum = 0.f;
                    float sqsum = 0.f;
                    for (int j = 0; j < w; j ++) {
                        sum += ptr[j];
                        // sqsum += ptr[j] * ptr[j];
                    }
                    float mean = sum / w;
                    float tmp = 0.f;
                    for (int j = 0; j < w; j ++) {
                        tmp = ptr[j] - mean;
                        sqsum += tmp * tmp;
                    }
                    float var = sqsum / w;
                    // the var maybe minus due to accuracy
                    // float var = sqsum / w - mean * mean;

                    float a = (1.f / (sqrt(var + eps)));
                    float b = -mean * a;

                    if (ln->affine) {
                        for (int j = 0; j < w; j ++) {
                            ptr[j] = (ptr[j] * a + b) * gamma_data[j] + beta_data[j];
                        }
                    } else {
                        for (int j = 0; j < w; j ++) {
                            ptr[j] = ptr[j] * a + b;
                        }
                    }
                }
            }
        } else { // if (affine_size == size)
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int q = 0; q < channels; q ++) {
                float *ptr = (float *)Mat_channel(input, q);

                // mean and var
                float sum = 0.f;
                float sqsum = 0.f;
                for (int i = 0; i < size; i ++) {
                    sum += ptr[i];
                    // sqsum += ptr[i] * ptr[i];
                }
                float mean = sum / size;
                float tmp = 0.f;
                for (int i = 0; i < size; i ++) {
                    tmp = ptr[i] - mean;
                    sqsum += tmp * tmp;
                }
                float var = sqsum / size;
                // the var maybe minus due to accuracy
                // float var = sqsum / size - mean * mean;

                float a = (1.f / (sqrt(var + eps)));
                float b = -mean * a;

                if (ln->affine) {
                    for (int i = 0; i < size; i ++) {
                        ptr[i] = (ptr[i] * a + b) * gamma_data[i] + beta_data[i];
                    }
                } else {
                    for (int i = 0; i < size; i ++) {
                        ptr[i] = ptr[i] * a + b;
                    }
                }
            }
        }
    }

    return 0;
}


int LayerNorm_destroy(LayerNorm *bn) {
    if (bn != NULL) {
        Mat_destroy(bn->gamma_data_mat);
        Mat_destroy(bn->beta_data_mat);

        free(bn);
        bn = NULL;
    }

    return 0;
}