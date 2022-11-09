#include <string.h>

#include "macros.h"
#include "packing.h"

int Packing_2D_process(Mat *in, Mat *out) {
    if (in->dims != 2 || out->dims != 2) {
        return -1;
    }

    if (in->elempack == out->elempack) {
        return -1;
    }

    int outh = out->h;
    size_t lane_size = out->elemsize / out->elempack;

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < outh; i ++) {
        unsigned char *outptr = (unsigned char *)out->data + (size_t)i * in->w * out->elemsize;

        for (int j = 0; j < in->w; j ++) {
            unsigned char *out_elem_ptr = outptr + j * out->elemsize;

            for (int k = 0; k < out->elempack; k ++) {
                int srcy = (i * out->elempack + k) / in->elempack;
                if (srcy >= in->h)
                    break;

                int srck = (i * out->elempack + k) % in->elempack;

                const unsigned char *ptr = (const unsigned char *)in->data + (size_t)srcy * in->w * in->elemsize;
                const unsigned char *elem_ptr = ptr + j * in->elemsize;
                memcpy(out_elem_ptr + k * lane_size, elem_ptr + srck * lane_size, lane_size);
            }
        }
    }

    return 0;
}
