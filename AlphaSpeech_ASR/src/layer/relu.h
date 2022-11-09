#ifndef _RELU_H_
#define _RELU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _ReLU_{

}ReLU;


ReLU *ReLU_create();
int ReLU_destroy(ReLU *relu);
int ReLU_process_inplace(ReLU *relu, Mat *input);


#ifdef __cplusplus
}
#endif

#endif