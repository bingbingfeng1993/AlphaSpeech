#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/macros.h"
#include "core/padding.h"

#include "conv1d.h"

// normal conv1d float32

Conv1D *Conv1D_create(int in_channels, int out_channels, int kernel_w, \
                      int dilation_w, int stride_w, int pad_left, \
                      int pad_right, float pad_value, bool bias_used) {
    Conv1D *conv1d = (Conv1D *)malloc(sizeof(Conv1D));
    if (conv1d == NULL) {
        return NULL;
    }

    conv1d->in_channels = in_channels;
    conv1d->out_channels = out_channels;

    conv1d->kernel_w = kernel_w;
    conv1d->dilation_w = dilation_w;
    conv1d->stride_w = stride_w;

    conv1d->pad_left = pad_left;
    conv1d->pad_right = pad_right;
    conv1d->pad_value = pad_value;

    conv1d->bias_used = bias_used;

    conv1d->weight_mat = Mat_1D_create(in_channels * out_channels * kernel_w, 4u, 1);

    if (conv1d->bias_used == true) {
        conv1d->bias_mat = Mat_1D_create(out_channels, 4u, 1);
    }

    conv1d->input_bordered = NULL;
    conv1d->out_mat = NULL;

    return conv1d;
}

int Conv1D_load_variables(Conv1D *conv1d, char *file) {
    if (conv1d == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(file, "rb");
    if (weight_bin_file == NULL) {
        printf("no weight_bin_file\n");
        return -1;
    }

    size_t ret;
    ret = fread(conv1d->weight_mat->data, sizeof(float), conv1d->in_channels * conv1d->out_channels * conv1d->kernel_w, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    
    // Mat_1D_float_printf(conv1d->weight_mat);

    if (conv1d->bias_used == true) {
        ret = fread(conv1d->bias_mat->data, sizeof(float), conv1d->out_channels, weight_bin_file);
        if (ret == 0 || ret == -1) {
            return -1;
        }        
        // Mat_1D_float_printf(conv1d->bias_mat);
    }

    fclose(weight_bin_file);

    return 0;
}

int Conv1D_dump_header(Conv1D *conv1d, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name) {
    if (conv1d == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(param_file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(conv1d->weight_mat->data, sizeof(float), conv1d->in_channels * conv1d->out_channels * conv1d->kernel_w, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    

    if (conv1d->bias_used == true) {
        ret = fread(conv1d->bias_mat->data, sizeof(float), conv1d->out_channels, weight_bin_file);
        if (ret == 0 || ret == -1) {
            return -1;
        }        
    }

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "weight");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "weight");
    float *data_ptr = conv1d->weight_mat->data;
    for (int i = 0; i < conv1d->in_channels * conv1d->out_channels * conv1d->kernel_w; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");
    
    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "bias");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "bias");
    data_ptr = conv1d->bias_mat->data;
    for (int i = 0; i < conv1d->out_channels; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fclose(weight_bin_file);

    return 0;
}


int Conv1D_init_variables(Conv1D *conv1d, float *weight, float *bias) {
    if (conv1d == NULL) {
        return -1;
    }

    memcpy(conv1d->weight_mat->data, weight, sizeof(float) * conv1d->in_channels * conv1d->out_channels * conv1d->kernel_w);
    // Mat_1D_float_printf(conv1d->weight_mat);

    if (conv1d->bias_used == true) {
        memcpy(conv1d->bias_mat->data, bias, sizeof(float) * conv1d->out_channels);
    }

    return 0;
}

int Conv1D_process(Conv1D *conv1d, Mat *input) {
    if (conv1d == NULL) {
        return -1;
    }

    if (conv1d->input_bordered == NULL) {
        if (conv1d->pad_left > 0 || conv1d->pad_right > 0) {
            int outw = input->w + conv1d->pad_left + conv1d->pad_right;
            int outh = input->h;
            conv1d->input_bordered = Mat_2D_create(outw, outh, input->elemsize, input->elempack);
        } else {
            conv1d->input_bordered = input;
        }
    }

    if (conv1d->pad_left > 0 || conv1d->pad_right > 0) {
        Padding_2D_float_process(input, conv1d->input_bordered, 0, conv1d->pad_left, 0, conv1d->pad_value);
    }

    const int w = conv1d->input_bordered->w;
    const size_t elemsize = conv1d->input_bordered->elemsize;
    const int kernel_extent_w = conv1d->dilation_w * (conv1d->kernel_w - 1) + 1;
    const int outw = (w - kernel_extent_w) / conv1d->stride_w + 1;

    if (conv1d->out_mat == NULL) {
        conv1d->out_mat = Mat_2D_create(outw, conv1d->out_channels, elemsize, 1);
    }

    const int h = conv1d->input_bordered->h;
    const int outh = conv1d->out_channels;

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int p = 0; p < outh; p ++) {
        float *outptr = (float *)Mat_row(conv1d->out_mat, p);

        for (int j = 0; j < outw; j ++) {
            float sum = 0.f;

            if (conv1d->bias_used == true)
                sum = ((float *)conv1d->bias_mat->data)[p];

            const float *kptr = (const float *)conv1d->weight_mat->data + conv1d->kernel_w * h * p;

            for (int q = 0; q < h; q ++) {
                const float *sptr = (float *)Mat_row(conv1d->input_bordered, q) + j * conv1d->stride_w;

                for (int k = 0; k < conv1d->kernel_w; k ++) {
                    float val = *sptr;
                    float wt = kptr[k];
                    sum += val * wt;

                    sptr += conv1d->dilation_w;
                }

                kptr += conv1d->kernel_w;
            }

            outptr[j] = sum;
        }
    }

    return 0;
}

Mat *Conv1D_get_output(Conv1D *conv1d) {
    return conv1d->out_mat;
}

int Conv1D_destroy(Conv1D *conv1d) {
    if (conv1d != NULL) {
        Mat_destroy(conv1d->weight_mat);
        Mat_destroy(conv1d->bias_mat);
        if (conv1d->input_bordered != NULL) {
            Mat_destroy(conv1d->input_bordered);
        }
        if (conv1d->out_mat != NULL) {
            Mat_destroy(conv1d->out_mat);
        }
        
        free(conv1d);
        conv1d = NULL;
    }

    return 0;
}