// Copyright (c) 2022 Personal (Bingbing Feng)

#ifndef __CTC_ENDPOINT_H__
#define __CTC_ENDPOINT_H__

typedef struct _CTC_Endpoint_Rule_{

    int must_decoded_sth;
    int min_trailing_silence;
    int min_utterance_length;

}CTC_Endpoint_Rule;

typedef struct _CTC_Endpoint_{

    int abs_time_step;
    int vocab_size; 

    /// We consider blank as silence for purposes of endpointing.
    int blank;                    // blank id
    float blank_threshold;        // blank threshold to be silence

    int frame_shift_in_ms;
    int num_frames_decoded;
    int num_frames_trailing_blank;

}CTC_Endpoint;

CTC_Endpoint *ctc_endpoint_create(int abs_time_step, int vocab_size, int blank, float blank_threshold, int frame_shift_in_ms);
int ctc_endpoint_destroy(CTC_Endpoint *ctc_endpoint_detector);
int ctc_endpoint_process(CTC_Endpoint *ctc_endpoint_detector, float **logp, int decoded_something);

#endif
