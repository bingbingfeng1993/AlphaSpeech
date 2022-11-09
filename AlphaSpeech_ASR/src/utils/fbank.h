// Copyright (c) 2022 Personal (Bingbing Feng)

#ifndef __FBANK_H__
#define __FBANK_H__

#include <stdbool.h>

typedef struct _FBANK_{
    
    int num_bins;
    int sample_rate;
    int frame_length;
    int frame_shift;
    bool use_log;
    bool remove_dc_offset;

    int fft_points;

    int *bitrev;        // bit reversal table
    float *sintbl;      // trigonometric function table

    float **bins;
    float *center_freqs;
    float *this_bin;
    int *bins_index;
    int *bins_size;

    float *povey_window;

    float *fft_real;
    float *fft_img;
    float *power;

}FBANK;

FBANK *fbank_create(int num_bins, int sample_rate, int frame_length, int frame_shift, bool use_log, bool remove_dc_offset);
int fbank_destroy(FBANK *fb);
int fbank_process(FBANK *fb, float *in, int num_samples, float **feat);

#endif
