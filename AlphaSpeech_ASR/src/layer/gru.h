#ifndef _GRU_H_
#define _GRU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _GRU_{
   int input_size;
   int hidden_size;
   bool bias_used;

   Mat *weight_xc_mat;
   Mat *weight_hc_mat;
   Mat *bias_c_mat;

   Mat *hidden_mat;
   Mat *gates_mat;

   Mat *out_mat;

}GRU;


GRU *GRU_create(int input_size, int hidden_size, bool bias_used);
int GRU_load_variables(GRU *gru, char *file);
int GRU_init_variables(GRU *gru, float *weight_xc, float *weight_hc, float *bias);
int GRU_destroy(GRU *gru);
int GRU_process(GRU *gru, Mat *input);
Mat *GRU_get_output(GRU *gru);
int GRU_dump_header(GRU *gru, char *param_file, FILE *header_out_file, FILE *source_out_file, char *dump_name);


#ifdef __cplusplus
}
#endif

#endif