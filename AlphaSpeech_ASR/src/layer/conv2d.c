#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/macros.h"
#include "core/padding.h"

#include "conv2d.h"

// normal conv1d float32

Conv2D *Conv2D_create(int in_channels, int num_output, int kernel_w, int kernel_h, \
                      int stride_w, int stride_h, int dilation_w, \
                      int dilation_h, int pad_left, int pad_right, \
                      int pad_top, int pad_bottom, float pad_value, \
                      bool bias_used) {
    Conv2D *conv2d = (Conv2D *)malloc(sizeof(Conv2D));
    if (conv2d == NULL) {
        return NULL;
    }

    conv2d->num_output = num_output;

    conv2d->kernel_w = kernel_w;
    conv2d->kernel_h = kernel_h;

    conv2d->stride_w = stride_w;
    conv2d->stride_h = stride_h;

    conv2d->dilation_w = dilation_w;
    conv2d->dilation_h = dilation_h;

    conv2d->pad_left = pad_left;
    conv2d->pad_right = pad_right;
    conv2d->pad_top = pad_top;
    conv2d->pad_bottom = pad_bottom;

    conv2d->pad_value = pad_value;

    conv2d->bias_used = bias_used;

    conv2d->weight_data_size = in_channels * kernel_w * kernel_h * num_output;

    conv2d->weight_mat = Mat_1D_create(conv2d->weight_data_size, 4u, 1);

    if (conv2d->bias_used == true) {
        conv2d->bias_mat = Mat_1D_create(num_output, 4u, 1);
    }

    conv2d->input_bordered = NULL;
    conv2d->out_mat = NULL;

    conv2d->space_ofs = (int *)malloc(conv2d->kernel_w * conv2d->kernel_h * sizeof(int));

    return conv2d;
}

int Conv2D_load_variables(Conv2D *conv2d, char *file) {
    if (conv2d == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(file, "rb");
    if (weight_bin_file == NULL) {
        printf("no weight_bin_file\n");
        return -1;
    }

    size_t ret;
    ret = fread(conv2d->weight_mat->data, sizeof(float), conv2d->weight_data_size, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    
    // Mat_1D_float_printf(conv2d->weight_mat);

    if (conv2d->bias_used == true) {
        ret = fread(conv2d->bias_mat->data, sizeof(float), conv2d->num_output, weight_bin_file);
        if (ret == 0 || ret == -1) {
            return -1;
        }
        // Mat_1D_float_printf(conv2d->bias_mat);
    }

    fclose(weight_bin_file);

    return 0;
}

int Conv2D_dump_header(Conv2D *conv2d, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name) {
    if (conv2d == NULL) {
        return -1;
    }

    FILE *weight_bin_file = fopen(param_file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(conv2d->weight_mat->data, sizeof(float), conv2d->weight_data_size, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }    

    if (conv2d->bias_used == true) {
        ret = fread(conv2d->bias_mat->data, sizeof(float), conv2d->num_output, weight_bin_file);
        if (ret == 0 || ret == -1) {
            return -1;
        }        
    }

    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "weight");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "weight");
    float *data_ptr = conv2d->weight_mat->data;
    for (int i = 0; i < conv2d->weight_data_size; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");
    
    fprintf(header_out_file, "extern %s %s_%s[];\n", "float", dump_name, "bias");
    fprintf(source_out_file, "%s %s_%s[] = {", "float", dump_name, "bias");
    data_ptr = conv2d->bias_mat->data;
    for (int i = 0; i < conv2d->num_output; i ++) {
        fprintf(source_out_file, "%f,", data_ptr[i]);
    }
    fprintf(source_out_file, "};\n\n");

    fclose(weight_bin_file);

    return 0;
}


int Conv2D_init_variables(Conv2D *conv2d, float *weight, float *bias) {
    if (conv2d == NULL) {
        return -1;
    }

    memcpy(conv2d->weight_mat->data, weight, sizeof(float) * conv2d->weight_data_size);
    // Mat_1D_float_printf(conv1d->weight_mat);

    if (conv2d->bias_used == true) {
        memcpy(conv2d->bias_mat->data, bias, sizeof(float) * conv2d->num_output);
    }

    return 0;
}

int Conv2D_process(Conv2D *conv2d, Mat *input) {
    if (conv2d == NULL) {
        return -1;
    }

    if (conv2d->input_bordered == NULL) {
        if (conv2d->pad_left > 0 || conv2d->pad_right > 0 || conv2d->pad_bottom > 0 || conv2d->pad_top > 0) {
            int outw = input->w + conv2d->pad_left + conv2d->pad_right;
            int outh = input->h + conv2d->pad_top + conv2d->pad_bottom;
            conv2d->input_bordered = Mat_2D_create(outw, outh, input->elemsize, input->elempack);
        } else {
            conv2d->input_bordered = input;
        }
    }

    if (conv2d->pad_left > 0 || conv2d->pad_right > 0 || conv2d->pad_bottom > 0 || conv2d->pad_top > 0) {
        // 知道总的长宽,且知道top,left就可以推出bottom,right
        Padding_2D_float_process(input, conv2d->input_bordered, conv2d->pad_top, conv2d->pad_left, 0, conv2d->pad_value);
    }

    const int w = conv2d->input_bordered->w;
    const int h = conv2d->input_bordered->h;
    const size_t elemsize = conv2d->input_bordered->elemsize;

    const int kernel_extent_w = conv2d->dilation_w * (conv2d->kernel_w - 1) + 1;
    const int kernel_extent_h = conv2d->dilation_h * (conv2d->kernel_h - 1) + 1;

    const int outw = (w - kernel_extent_w) / conv2d->stride_w + 1;
    const int outh = (h - kernel_extent_h) / conv2d->stride_h + 1;

    if (conv2d->out_mat == NULL) {
        conv2d->out_mat = Mat_3D_create(outw, outh, conv2d->num_output, elemsize, 1);
    }

    const int inch = conv2d->input_bordered->c;
    const int outch = conv2d->out_mat->c;

    const bool bias_term = conv2d->bias_used;

    const int maxk = conv2d->kernel_w * conv2d->kernel_h;

    // kernel offsets
    int p1 = 0;
    int p2 = 0;
    int gap = w * conv2d->dilation_h - conv2d->kernel_w * conv2d->dilation_w;
    for (int i = 0; i < conv2d->kernel_h; i ++) {
        for (int j = 0; j < conv2d->kernel_w; j ++) {
            conv2d->space_ofs[p1] = p2;
            p1 ++;
            p2 += conv2d->dilation_w;
        }
        p2 += gap;
    }

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int p = 0; p < outch; p ++) {
        float *outptr = (float *)Mat_channel(conv2d->out_mat, p);

        for (int i = 0; i < outh; i ++) {
            for (int j = 0; j < outw; j ++) {
                float sum = 0.f;

                if (bias_term)
                    sum = ((float *)conv2d->bias_mat->data)[p];

                const float *kptr = (const float *)conv2d->weight_mat->data + maxk * inch * p;
                for (int q = 0; q < inch; q ++) {
                    const float *sptr = (float *)Mat_channel_row(conv2d->input_bordered, q, i * conv2d->stride_h) + j * conv2d->stride_w;

                    for (int k = 0; k < maxk; k ++) {
                        float val = sptr[conv2d->space_ofs[k]];
                        float wt = kptr[k];
                        sum += val * wt;
                    }

                    kptr += maxk;
                }

                outptr[j] = sum;
            }

            outptr += outw;
        }
    }    

    return 0;
}

Mat *Conv2D_get_output(Conv2D *conv2d) {
    return conv2d->out_mat;
}

int Conv2D_destroy(Conv2D *conv2d) {
    if (conv2d != NULL) {
        Mat_destroy(conv2d->weight_mat);
        Mat_destroy(conv2d->bias_mat);
        if (conv2d->input_bordered != NULL) {
            Mat_destroy(conv2d->input_bordered);
        }
        if (conv2d->out_mat != NULL) {
            Mat_destroy(conv2d->out_mat);
        }

        free(conv2d->space_ofs);
        conv2d->space_ofs = NULL;
        free(conv2d);
        conv2d = NULL;
    }
    
    return 0;
}