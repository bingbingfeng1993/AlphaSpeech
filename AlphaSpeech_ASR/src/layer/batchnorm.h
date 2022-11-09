#ifndef _BATCHNORM_H_
#define _BATCHNORM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _BatchNorm_{
    int channels;
    
    Mat *slope_data_mat;
    Mat *mean_data_mat;
    Mat *var_data_mat;
    Mat *bias_data_mat;

    Mat *a_data_mat;
    Mat *b_data_mat;
    
}BatchNorm;


BatchNorm *BatchNorm_create(int channels);
int BatchNorm_destroy(BatchNorm *bn);
int BatchNorm_load_variables(BatchNorm *bn, char *file);
int BatchNorm_init_variables(BatchNorm *bn, float* weight, float *bias, float *mean, float *var);
int BatchNorm_process_inplace(BatchNorm *bn, Mat *input);
int BatchNorm_dump_header(BatchNorm *bn, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name);


#ifdef __cplusplus
}
#endif

#endif