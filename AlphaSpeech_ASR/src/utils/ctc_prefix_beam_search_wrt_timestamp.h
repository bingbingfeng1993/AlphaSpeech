// Copyright (c) 2022 Personal (Bingbing Feng)

#ifndef __CTC_Prefix_Beam_Search_WRT_Timestamp_H__
#define __CTC_Prefix_Beam_Search_WRT_Timestamp_H__

typedef struct _CTC_Prefix_Beam_Search_WRT_Timestamp_{

    int abs_time_step;
    int vocab_size;

    int blank;                          // blank id
    int first_beam_size;
    int second_beam_size;

    int *topk_index;
    float *topk_score;

}CTC_Prefix_Beam_Search_WRT_Timestamp;

typedef struct _HYPS_TS_{
    
    int *prefix;

    float s;                            // blank ending score, default: -FLT_MAX
    float ns;                           // none blank ending score, default: -FLT_MAX
    float score;

    int prefix_dynamic_size;

    float v_s;                          // viterbi blank ending score, default: -FLT_MAX
    float v_ns;                         // viterbi none blank ending score, default: -FLT_MAX
    float viterbi_score;

    float cur_token_prob;               // prob of current token of viterbi none blank path, default: -FLT_MAX

    int *times_s;                       // times of viterbi blank path
    int *times_ns;                      // times of viterbi none blank path
    int *times;

    int times_s_dynamic_size;
    int times_ns_dynamic_size;
    int times_dynamic_size;

}HYPS_TS;

CTC_Prefix_Beam_Search_WRT_Timestamp *ctc_prefix_beam_search_wrt_timestamp_create(int abs_time_step, int vocab_size, int blank, int first_beam_size, int second_beam_size);
int ctc_prefix_beam_search_wrt_timestamp_destroy(CTC_Prefix_Beam_Search_WRT_Timestamp *ctc_decoder);
int ctc_prefix_beam_search_wrt_timestamp_process(CTC_Prefix_Beam_Search_WRT_Timestamp *ctc_decoder, float **logp, int *result, int *result_times, int cont_time_step);

#endif
