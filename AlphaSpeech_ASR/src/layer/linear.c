#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/macros.h"

#include "linear.h"

// normal linear float32

Linear *Linear_create(int in_features, int out_features, bool bias_used) {
    Linear *linear = (Linear *)malloc(sizeof(Linear));
    if (linear == NULL) {
        return NULL;
    }

    linear->in_features = in_features;
    linear->out_features = out_features;
    linear->bias_used = bias_used;

    linear->weight_mat = Mat_2D_create(in_features, out_features, 4, 1);
    
    if (linear->bias_used == true) {
        linear->bias_mat = Mat_1D_create(out_features, 4, 1);
    }

    linear->out_mat = NULL;

    return linear;
}

int Linear_load_variables(Linear *linear, char *file) {
    if (linear == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(linear->weight_mat->data, sizeof(float), linear->in_features * linear->out_features, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }
    // Mat_2D_float_printf(linear->weight_mat);

    if (linear->bias_used == true) {
        ret = fread(linear->bias_mat->data, sizeof(float), linear->out_features, weight_bin_file);
        if (ret == 0 || ret == -1) {
            return -1;
        }        
        // Mat_1D_float_printf(linear->bias_mat);
    }

    fclose(weight_bin_file);

    return 0;
}

int Linear_dump_header(Linear *linear, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name) {
    if (linear == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(param_file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(linear->weight_mat->data, sizeof(float), linear->in_features * linear->out_features, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }

    if (linear->bias_used == true) {
        ret = fread(linear->bias_mat->data, sizeof(float), linear->out_features, weight_bin_file);
        if (ret == 0 || ret == -1) {
            return -1;
        }        
    }

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "weight");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "weight");
    float *data_ptr = linear->weight_mat->data;
    for (int i = 0; i < linear->in_features * linear->out_features; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");
    
    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "bias");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "bias");
    data_ptr = linear->bias_mat->data;
    for (int i = 0; i < linear->out_features; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fclose(weight_bin_file);

    return 0;
}

int Linear_init_variables(Linear *linear, float *weight, float *bias) {
    if (linear == NULL) {
        return -1;
    }

    memcpy(linear->weight_mat->data, weight, sizeof(float) * linear->in_features * linear->out_features);
    // Mat_2D_float_printf(linear->weight_mat);

    if (linear->bias_used == true) {
        memcpy(linear->bias_mat->data, bias, sizeof(float) * linear->out_features);
    }

    return 0;
}

int Linear_process(Linear *linear, Mat *input) {
    if (linear == NULL) {
        return -1;
    }

    if (linear->out_mat == NULL) {
        linear->out_mat = Mat_2D_create(linear->out_features, input->h, sizeof(float), 1);
    }

    if (input->dims == 2 && input->w == linear->in_features) {

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int j = 0; j < input->h; j ++) {

            const float *m = (float *)Mat_row(input, j);
            float *outptr = (float *)Mat_row(linear->out_mat, j);

            for (int p = 0; p < linear->out_features; p ++) {
                const float *kptr = (const float *)linear->weight_mat->data + input->w * p;

                float sum = 0.f;

                if (linear->bias_used) {
                    sum = *((float *)linear->bias_mat->data + p);
                }

                for (int i = 0; i < input->w; i ++) {
                    sum += m[i] * kptr[i];
                }

                outptr[p] = sum;
            }
        }

        return 0;
    }

    return -1;
}

int Linear_process_use_extern_output(Linear *linear, Mat *input, Mat *output) {
    if (linear == NULL) {
        return -1;
    }

    if (input->dims == 2 && input->w == linear->in_features) {

        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int j = 0; j < input->h; j ++) {

            const float *m = (float *)Mat_row(input, j);
            float *outptr = (float *)Mat_row(output, j);

            for (int p = 0; p < linear->out_features; p ++) {
                const float *kptr = (const float *)linear->weight_mat->data + input->w * p;

                float sum = 0.f;

                if (linear->bias_used) {
                    sum = *((float *)linear->bias_mat->data + p);
                }

                for (int i = 0; i < input->w; i++) {
                    sum += m[i] * kptr[i];
                }

                outptr[p] = sum;
            }
        }

        return 0;
    }

    return -1;
}


Mat *Linear_get_output(Linear *linear) {
    return linear->out_mat;
}

int Linear_destroy(Linear *linear) {
    if (linear != NULL) {
        Mat_destroy(linear->weight_mat);
        Mat_destroy(linear->bias_mat);
        if (linear->out_mat != NULL) {
            Mat_destroy(linear->out_mat);
        }  

        free(linear);
        linear = NULL;
    }

    return 0;
}