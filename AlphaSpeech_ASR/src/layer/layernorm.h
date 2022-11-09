#ifndef _LAYER_NORM_H_
#define _LAYER_NORM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _LayerNorm_{
    int affine_size;
    bool affine;
    
    Mat *gamma_data_mat;
    Mat *beta_data_mat;

}LayerNorm;


LayerNorm *LayerNorm_create(int affine_size, bool affine);
int LayerNorm_destroy(LayerNorm *ln);
int LayerNorm_load_variables(LayerNorm *ln, char *file);
int LayerNorm_init_variables(LayerNorm *ln, float *weight, float *bias);
int LayerNorm_process_inplace(LayerNorm *ln, Mat *input);
int LayerNorm_dump_header(LayerNorm *ln, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name);


#ifdef __cplusplus
}
#endif

#endif