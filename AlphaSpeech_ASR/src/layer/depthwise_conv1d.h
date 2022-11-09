#ifndef _DEPTHWISE_CONV1D_H_
#define _DEPTHWISE_CONV1D_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _Depthwise_Conv1D_{
   int in_channels;
   int out_channels;

   int kernel_w;
   int dilation_w;
   int stride_w;
   int groups;

   int pad_left;
   int pad_right;
   float pad_value;

   bool bias_used;

   Mat *weight_mat;
   Mat *bias_mat;

   Mat *input_bordered;
   Mat *out_mat;

}Depthwise_Conv1D;


Depthwise_Conv1D *Depthwise_Conv1D_create(int in_channels, int out_channels, int kernel_w, \
                                          int dilation_w, int stride_w, int groups, int pad_left, \
                                          int pad_right, float pad_value, bool bias_used);
int Depthwise_Conv1D_load_variables(Depthwise_Conv1D *conv1d, char *file);
int Depthwise_Conv1D_init_variables(Depthwise_Conv1D *conv1d, float *weight, float *bias);
int Depthwise_Conv1D_destroy(Depthwise_Conv1D *conv1d);
int Depthwise_Conv1D_process(Depthwise_Conv1D *conv1d, Mat *input);
Mat *Depthwise_Conv1D_get_output(Depthwise_Conv1D *conv1d);
int Depthwise_Conv1D_dump_header(Depthwise_Conv1D *conv1d, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name);


#ifdef __cplusplus
}
#endif

#endif