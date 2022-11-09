#ifndef _Tanh_H_
#define _Tanh_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _Tanh_{

}Tanh;


Tanh *Tanh_create();
int Tanh_destroy(Tanh *tanh_obj);
int Tanh_process_inplace(Tanh *tanh_obj, Mat *input);


#ifdef __cplusplus
}
#endif

#endif