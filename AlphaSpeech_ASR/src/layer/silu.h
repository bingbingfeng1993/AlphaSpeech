#ifndef _SILU_H_
#define _SILU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "core/mat.h"

typedef struct _SILU_{

}SiLU;


SiLU *SiLU_create();
int SiLU_destroy(SiLU *silu);
int SiLU_process_inplace(SiLU *silu, Mat *input);


#ifdef __cplusplus
}
#endif

#endif