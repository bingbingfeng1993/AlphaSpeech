#ifndef _SIGMOID_H_
#define _SIGMOID_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _Sigmoid_{

}Sigmoid;


Sigmoid *Sigmoid_create();
int Sigmoid_destroy(Sigmoid *sigmoid);
int Sigmoid_process_inplace(Sigmoid *sigmoid, Mat *input);


#ifdef __cplusplus
}
#endif

#endif