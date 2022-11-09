// Copyright (c) 2022 Personal (Bingbing Feng)

#ifndef __CTC_Prefix_Beam_Search_H__
#define __CTC_Prefix_Beam_Search_H__

typedef struct _CTC_Prefix_Beam_Search_{

    int abs_time_step;
    int vocab_size;

    int blank;                          // blank id
    int first_beam_size;
    int second_beam_size;

    int *topk_index;
    float *topk_score;

}CTC_Prefix_Beam_Search;

typedef struct _HYPS_{
    
    int *prefix;

    float s;                            // blank ending score, default: -FLT_MAX
    float ns;                           // none blank ending score, default: -FLT_MAX
    float score;

    int prefix_dynamic_size;

}HYPS;

CTC_Prefix_Beam_Search *ctc_prefix_beam_search_create(int abs_time_step, int vocab_size, int blank, int first_beam_size, int second_beam_size);
int ctc_prefix_beam_search_destroy(CTC_Prefix_Beam_Search *ctc_decoder);
int ctc_prefix_beam_search_process(CTC_Prefix_Beam_Search *ctc_decoder, float **logp, int *result);

#endif
