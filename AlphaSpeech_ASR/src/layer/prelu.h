#ifndef _PRELU_H_
#define _PRELU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _PReLU_{
    int num_slope;
    Mat *slope_data_mat;

}PReLU;


PReLU *PReLU_create(int num_slope);
int PReLU_destroy(PReLU *prelu);
int PReLU_load_variables(PReLU *prelu, char *file);
int PReLU_init_variables(PReLU *prelu, float *weight);
int PReLU_process_inplace(PReLU *prelu, Mat *input);
int PReLU_dump_header(PReLU *prelu, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name);


#ifdef __cplusplus
}
#endif

#endif