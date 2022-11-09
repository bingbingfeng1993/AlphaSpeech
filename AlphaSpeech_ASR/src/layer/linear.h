#ifndef _LINEAR_H_
#define _LINEAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _Linear_{
   int in_features;
   int out_features;
   bool bias_used;

   Mat *weight_mat;
   Mat *bias_mat;

   Mat *out_mat;

}Linear;


Linear *Linear_create(int in_features, int out_features, bool bias_used);
int Linear_load_variables(Linear *linear, char *file);
int Linear_init_variables(Linear *linear, float *weight, float *bias);
int Linear_destroy(Linear *linear);
int Linear_process(Linear *linear, Mat *input);
int Linear_process_use_extern_output(Linear *linear, Mat *input, Mat *output);
Mat *Linear_get_output(Linear *linear);
int Linear_dump_header(Linear *linear, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name);


#ifdef __cplusplus
}
#endif

#endif