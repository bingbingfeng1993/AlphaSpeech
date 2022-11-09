// Copyright (c) 2022 Personal (Bingbing Feng)

#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../src/utils/fbank.h"

typedef struct _Wav_Header_ {

    char riff[4];
    unsigned int size;
    char wav[4];
    char fmt[4];
    unsigned int fmt_size;
    uint16_t format;
    uint16_t channels;
    unsigned int sample_rate;
    unsigned int bytes_per_second;
    uint16_t block_size;
    uint16_t bit;
    char data[4];
    unsigned int data_size;

}Wav_Header;

int main() {
    
    char wav_path[] = "/home/fengbingbing/nti2022_wohospeech_asr/WoHoSpeech_ASR/test/wav/BAC009S0901W0288.wav";

    FILE* fp = fopen(wav_path, "rb");

    Wav_Header header = {
        {'R', 'I', 'F', 'F'},
        0,
        {'W', 'A', 'V', 'E'},
        {'f', 'm', 't', ' '},
        16,
        1,
        0,
        0,
        0,
        0,
        0,
        {'d', 'a', 't', 'a'},
        0
    };

    size_t count = fread(&header, 1, sizeof(header), fp);
    if (count != sizeof(header)) {
        fprintf(stderr, "error");
        exit(1);
    }

    int num_channel = header.channels;                              // 1
    int bits_per_sample = header.bit;
    int num_data = header.data_size / (bits_per_sample / 8);
    int num_samples = num_data / num_channel;                       // sample points per channel

    float *wave = (float *)malloc(sizeof(float)*num_data);

    for (int i = 0; i < num_data; i ++) {
        switch (bits_per_sample) {
            case 8: {
                char sample;
                size_t count_8 = fread(&sample, 1, sizeof(char), fp);
                if (count_8 != sizeof(char)) {
                    fprintf(stderr, "error");
                    exit(1);
                }
                wave[i] = (float) sample;
                break;
            }
            case 16: {
                int16_t sample;
                size_t count_16 = fread(&sample, 1, sizeof(int16_t), fp);
                if (count_16 != sizeof(int16_t)) {
                    fprintf(stderr, "error");
                    exit(1);
                }
                wave[i] = (float) sample;
                break;
            }
            case 32: {
                int sample;
                size_t count_32 = fread(&sample, 1, sizeof(int), fp);
                if (count_32 != sizeof(int)) {
                    fprintf(stderr, "error");
                    exit(1);
                }
                wave[i] = (float) sample;
                break;
            }
            default:
                fprintf(stderr, "unsupported quantization bits");
                exit(1);
        }
    }

    int num_bins = 80;
    int sample_rate = 16000;
    int frame_length = sample_rate / 1000 * 25;
    int frame_shift = sample_rate / 1000 * 10;

    FBANK *fb = fbank_create(num_bins, sample_rate, frame_length, frame_shift, true, true);

    int num_frames = 1 + ((num_samples - frame_length) / frame_shift);
    printf("%d\n", num_frames);

    float **feat = (float **)malloc(sizeof(float *) * num_frames);
    for (int i = 0; i < num_frames; i ++) {
        feat[i] = (float *)malloc(sizeof(float) * num_bins); 
    }

    fbank_process(fb, wave, num_samples, feat);
    /*
    for (int i = 0; i < num_frames; i ++) {
        for (int j = 0; j < num_bins; j ++) {
            printf("%f ", feat[i][j]);
        }
        printf("\n");
    }*/
    for (int i = 0; i < num_bins; i ++) {
        printf("%10f ", feat[0][i]);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    for(int i = 0; i < num_frames; i ++) {
        free(feat[i]);
        feat[i] = NULL;
    }
    free(feat);
    feat = NULL;

    fclose(fp);

    return 0;
}

