#include "../src/decoder/asr_decoder.h"

int main(int argc, char *argv[]) {

    FILE* wav_fp = fopen(argv[1], "rb");

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

    size_t count = fread(&header, 1, sizeof(header), wav_fp);
    if (count != sizeof(header)) {
        fprintf(stderr, "error");
        exit(1);
    }

    int num_channel = header.channels;                              // 1
    int bits_per_sample = header.bit;
    int num_data = header.data_size / (bits_per_sample / 8);
    int num_samples = num_data / num_channel;                       // sample points per channel

    float *wave = (float *)malloc(sizeof(float) * num_data);

    for (int i = 0; i < num_data; i ++) {
        switch (bits_per_sample) {
            case 8: {
                char sample;
                size_t count_8 = fread(&sample, 1, sizeof(char), wav_fp);
                if (count_8 != sizeof(char)) {
                    fprintf(stderr, "error");
                    exit(1);
                }
                wave[i] = (float) sample;
                break;
            }
            case 16: {
                int16_t sample;
                size_t count_16 = fread(&sample, 1, sizeof(int16_t), wav_fp);
                if (count_16 != sizeof(int16_t)) {
                    fprintf(stderr, "error");
                    exit(1);
                }
                wave[i] = (float) sample;
                break;
            }
            case 32: {
                int sample;
                size_t count_32 = fread(&sample, 1, sizeof(int), wav_fp);
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

    fclose(wav_fp);

    WoHoSpeech_ASR *asr = wohospeech_asr_create("./../out/model_dump_u2++_en");
    wohospeech_asr_process(asr, wave, num_samples, "./../out/dict/lang_char_en.txt");
    wohospeech_asr_destroy(asr);

    return 0;
}

