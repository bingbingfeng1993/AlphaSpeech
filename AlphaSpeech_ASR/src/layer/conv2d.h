#ifndef _CONV2D_H_
#define _CONV2D_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _Conv2D_{
   int num_output;
   
   int kernel_w;
   int kernel_h;

   int dilation_w;
   int dilation_h;

   int stride_w;
   int stride_h;

   int pad_left;
   int pad_right;
   int pad_top;
   int pad_bottom;

   int weight_data_size;

   float pad_value;

   bool bias_used;

   Mat *weight_mat;
   Mat *bias_mat;

   Mat *input_bordered;
   Mat *out_mat;

   int *space_ofs;

}Conv2D;


Conv2D *Conv2D_create(int in_channels, int num_output, int kernel_w, int kernel_h, \
                      int stride_w, int stride_h, int dilation_w, \
                      int dilation_h, int pad_left, int pad_right, \
                      int pad_top, int pad_bottom, float pad_value, \
                      bool bias_used);

int Conv2D_load_variables(Conv2D *conv2d, char *file);
int Conv2D_init_variables(Conv2D *conv2d, float *weight, float *bias);
int Conv2D_destroy(Conv2D *conv2d);
int Conv2D_process(Conv2D *conv2d, Mat *input);
Mat *Conv2D_get_output(Conv2D *conv2d);
int Conv2D_dump_header(Conv2D *conv2d, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name);


#ifdef __cplusplus
}
#endif

#endif