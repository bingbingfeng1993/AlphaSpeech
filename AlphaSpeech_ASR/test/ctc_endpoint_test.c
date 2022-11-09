// Copyright (c) 2022 Personal (Bingbing Feng)

#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../src/utils/ctc_endpoint.h"

int main() {

	float data[3][3] = {  {0.25, 0.40, 0.35},
                        {0.40, 0.35, 0.25},
                        {0.10, 0.50, 0.40}  };
    
	int abs_time_step = sizeof(data) / sizeof(data[0]);
	int vocab_size = sizeof(data[0]) / sizeof(data[0][0]);

    for (int i = 0; i < abs_time_step; i ++) {
        for (int j = 0; j < vocab_size; j ++) {
            data[i][j] = log(data[i][j]);
        }
    }

    float **logp = (float **)malloc(sizeof(float *)*abs_time_step);
	for (int i = 0; i < abs_time_step; i ++) {
		logp[i] = data[i];
	}

    int blank = 0;
    float blank_threshold = 0.8;
    int frame_shift_in_ms = 40;

    CTC_Endpoint *ctc_endpoint_detector = ctc_endpoint_create(abs_time_step, vocab_size, blank, blank_threshold, frame_shift_in_ms);

    int decoded_something = 1;
    int is_endpoint = ctc_endpoint_process(ctc_endpoint_detector, logp, decoded_something);
    printf("%d\n", is_endpoint);

    return 0;
}

