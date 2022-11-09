// Copyright (c) 2022 Personal (Bingbing Feng)

#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "fbank.h"
#include "fft.h"

#define CHECK_EQ(x, y) (((x)==(y))?(true):(false))
#define CHECK_LT(x, y) (((x)<(y))?(true):(false))
#define CHECK_GT(x, y) (((x)>(y))?(true):(false))
#define CHECK_LE(x, y) (((x)<=(y))?(true):(false))
#define CHECK_GE(x, y) (((x)>=(y))?(true):(false))
#define CHECK_NE(x, y) (((x)!=(y))?(true):(false))

static inline float MelScale(float freq) {
    return 1127.0f * logf(1.0f + freq / 700.0f);
}

static inline float InverseMelScale(float mel_freq) {
    return 700.0f * (expf(mel_freq / 1127.0f) - 1.0f);
}

static int UpperPowerOfTwo(int n) {
    return (int) (pow(2, ceil(log(n) / log(2))));
}

// pre-emphasis
static void PreEmphasis(float coeff, float *data, int len) {
    if (coeff == 0.0) return;
    for (int i = len - 1; i > 0; i --)
        data[i] -= coeff * data[i - 1];
    data[0] -= coeff * data[0];
}

FBANK *fbank_create(int num_bins, int sample_rate, int frame_length, int frame_shift, bool use_log, bool remove_dc_offset) {
    FBANK *fb = (FBANK *)malloc(sizeof(FBANK));
    if (fb == NULL) {
        return NULL;
    }

    fb->num_bins = num_bins;
    fb->sample_rate = sample_rate;
    fb->frame_length = frame_length;
    fb->frame_shift = frame_shift;
    fb->use_log = use_log;
    fb->remove_dc_offset = remove_dc_offset;

    fb->fft_points = UpperPowerOfTwo(fb->frame_length);

    // generate bit reversal table and trigonometric function table
    const int fft_points_4 = fb->fft_points / 4;
    fb->bitrev = (int *)malloc(sizeof(int)*fb->fft_points);
    fb->sintbl = (float *)malloc(sizeof(float)*(fb->fft_points + fft_points_4));
    make_sintbl(fb->fft_points, fb->sintbl);
    make_bitrev(fb->fft_points, fb->bitrev);

    int num_fft_bins = fb->fft_points / 2;
    float fft_bin_width = (float) fb->sample_rate / fb->fft_points;
    int low_freq = 20, high_freq = fb->sample_rate / 2;
    
    float mel_low_freq = MelScale(low_freq);
    float mel_high_freq = MelScale(high_freq);
    float mel_freq_delta = (mel_high_freq - mel_low_freq) / (num_bins + 1);

    fb->bins = (float **)malloc(sizeof(float *)*fb->num_bins);
    fb->center_freqs = (float *)malloc(sizeof(float)*fb->num_bins);
    fb->this_bin = (float *)malloc(sizeof(float)*num_fft_bins);
    fb->bins_index = (int *)malloc(sizeof(int)*fb->num_bins);
    fb->bins_size = (int *)malloc(sizeof(int)*fb->num_bins);

    for (int bin = 0; bin < num_bins; bin ++) {
        float left_mel = mel_low_freq + bin * mel_freq_delta;
        float center_mel = mel_low_freq + (bin + 1) * mel_freq_delta;
        float right_mel = mel_low_freq + (bin + 2) * mel_freq_delta;

        fb->center_freqs[bin] = InverseMelScale(center_mel);

        int first_index = -1, last_index = -1;
        for (int i = 0; i < num_fft_bins; i ++) {
            float freq = fft_bin_width * i;  // Center frequency of this fft bin.
            float mel = MelScale(freq);
            if (mel > left_mel && mel < right_mel) {
                float weight;
                if (mel <= center_mel)
                    weight = (mel - left_mel) / (center_mel - left_mel);
                else
                    weight = (right_mel - mel) / (right_mel - center_mel);
                fb->this_bin[i] = weight;
                if (first_index == -1) first_index = i;
                last_index = i;
            }
        }
        assert(CHECK_NE(first_index, -1));
        assert(CHECK_GE(last_index, first_index));

        fb->bins_index[bin] = first_index;
        int size = last_index + 1 - first_index;
        fb->bins_size[bin] = size;
        fb->bins[bin] = (float *)malloc(sizeof(float)*size);
        for (int i = 0; i < size; i ++) {
            fb->bins[bin][i] = fb->this_bin[first_index + i];
        }
    }

    // povey window
    fb->povey_window = (float *)malloc(sizeof(float)*fb->frame_length);
    double a = M_2PI / (frame_length - 1);
    for (int i = 0; i < frame_length; i ++) {
        fb->povey_window[i] = pow(0.5 - 0.5 * cos(a * i), 0.85);
    }

    fb->fft_real = (float *)malloc(sizeof(float)*fb->fft_points);
    fb->fft_img = (float *)malloc(sizeof(float)*fb->fft_points);
    fb->power = (float *)malloc(sizeof(float)*fb->fft_points/2);

    return fb;
}


int fbank_destroy(FBANK *fb) {
    if (fb != NULL) {
        free(fb->bitrev);
        fb->bitrev = NULL;

        free(fb->sintbl);
        fb->sintbl = NULL;

        free(fb->center_freqs);
        fb->center_freqs = NULL;

        free(fb->this_bin);
        fb->this_bin = NULL;

        free(fb->bins_index);
        fb->bins_index = NULL;

        free(fb->bins_size);
        fb->bins_size = NULL;
        
        for(int i = 0; i < fb->num_bins; i ++) {
            free(fb->bins[i]);
            fb->bins[i] = NULL;
        }
        free(fb->bins);
        fb->bins = NULL;

        free(fb->povey_window);
        fb->povey_window = NULL;

        free(fb->fft_real);
        fb->fft_real = NULL;

        free(fb->fft_img);
        fb->fft_img = NULL;

        free(fb->power);
        fb->power = NULL;
    }

    return 0;
}

int fbank_process(FBANK *fb, float *in, int num_samples, float **feat) {

    if (num_samples < fb->frame_length) return 0;
    int num_frames = 1 + ((num_samples - fb->frame_length) / fb->frame_shift);

    for (int i = 0; i < num_frames; i ++) {
        float *data = (float *)malloc(sizeof(float)*fb->frame_length);

        memcpy(data, in + i * fb->frame_shift, sizeof(float) * fb->frame_length);

        // optinal remove dc offset
        if (fb->remove_dc_offset) {
            float mean = 0.0;
            for (size_t j = 0; j < fb->frame_length; j ++) mean += data[j];
            mean /= fb->frame_length;
            for (size_t j = 0; j < fb->frame_length; j ++) data[j] -= mean;
        }

        PreEmphasis(0.97, data, fb->frame_length);

        // Apply povey window on data in place
        for (int i = 0; i < fb->frame_length; i ++) {
            data[i] *= fb->povey_window[i];
        }

        // copy data to fft_real
        memset(fb->fft_img, 0, sizeof(float) * fb->fft_points);
        memset(fb->fft_real + fb->frame_length, 0, sizeof(float) * (fb->fft_points - fb->frame_length));
        memcpy(fb->fft_real, data, sizeof(float) * fb->frame_length);

        fft(fb->bitrev, fb->sintbl, fb->fft_real, fb->fft_img, fb->fft_points);

        // power
        for (int j = 0; j < fb->fft_points / 2; j ++) {
            fb->power[j] = fb->fft_real[j] * fb->fft_real[j] + fb->fft_img[j] * fb->fft_img[j];
        }

        // cepstral coefficients, triangle filter array    
        for (int j = 0; j < fb->num_bins; j ++) {
            float mel_energy = 0.0;
            int s = fb->bins_index[j];
            for (size_t k = 0; k < fb->bins_size[j]; k ++) {
                mel_energy += fb->bins[j][k] * fb->power[s + k];
            }

            // optional use log
            if (fb->use_log) {
                if (mel_energy < FLT_EPSILON)
                    mel_energy = FLT_EPSILON;
                mel_energy = logf(mel_energy);
            }

            feat[i][j] = mel_energy;
        }
    
        free(data);
        data = NULL;
    }

    return 0;
}

