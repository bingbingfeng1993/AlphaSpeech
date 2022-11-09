#ifndef _LSTM_H_
#define _LSTM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _LSTM_{
   int input_size;
   int hidden_size;
   bool bias_used;

   Mat *weight_xc_mat;
   Mat *weight_hc_mat;
   Mat *bias_c_mat;

   Mat *hidden_mat;
   Mat *cell_mat;
   Mat *gates_mat;

   Mat *out_mat;

}LSTM;


LSTM *LSTM_create(int input_size, int hidden_size, bool bias_used);
int LSTM_load_variables(LSTM *lstm, char *file);
int LSTM_init_variables(LSTM *lstm, float *weight_xc, float *weight_hc, float *bias);
int LSTM_destroy(LSTM *lstm);
int LSTM_process(LSTM *lstm, Mat *input);
Mat *LSTM_get_output(LSTM *lstm);


#ifdef __cplusplus
}
#endif

#endif