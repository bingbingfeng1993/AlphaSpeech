#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parameter.h"

int Parameter_load_variables(char *file, Mat *in) {
    FILE *weight_bin_file = fopen(file, "rb");
    if (weight_bin_file == NULL) {
        return -1;
    }

    size_t ret;
    ret = fread(in->data, sizeof(float), in->w * in->h * in->c, weight_bin_file);
    if (ret == 0 || ret == -1) {
        return -1;
    }
}