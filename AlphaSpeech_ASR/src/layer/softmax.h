#ifndef _SOFTMAX_H_
#define _SOFTMAX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _SOFTMAX_{
    int axis;

}Softmax;


Softmax *Softmax_create();
int Softmax_destroy(Softmax *softmax);
int Softmax_process_inplace(Softmax *softmax, Mat *input);


#ifdef __cplusplus
}
#endif

#endif