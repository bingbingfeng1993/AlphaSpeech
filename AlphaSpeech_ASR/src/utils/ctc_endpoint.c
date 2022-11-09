// Copyright (c) 2022 Personal (Bingbing Feng)

#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ctc_endpoint.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define CHECK_EQ(x, y) (((x)==(y))?(true):(false))
#define CHECK_LT(x, y) (((x)<(y))?(true):(false))
#define CHECK_GT(x, y) (((x)>(y))?(true):(false))
#define CHECK_LE(x, y) (((x)<=(y))?(true):(false))
#define CHECK_GE(x, y) (((x)>=(y))?(true):(false))
#define CHECK_NE(x, y) (((x)!=(y))?(true):(false))

static inline int RuleActivated(CTC_Endpoint_Rule *rule, char *rule_name, int decoded_sth, int trailing_silence, int utterance_length) {
    int ans = (decoded_sth || !rule->must_decoded_sth) && trailing_silence >= rule->min_trailing_silence && utterance_length >= rule->min_utterance_length;

    if (ans) {
        printf("endpoint rule %s activated: %d, %d, %d\n", rule_name, decoded_sth, trailing_silence, utterance_length);
    }

    return ans;
}

CTC_Endpoint *ctc_endpoint_create(int abs_time_step, int vocab_size, int blank, float blank_threshold, int frame_shift_in_ms) {
    CTC_Endpoint *ctc_endpoint_detector = (CTC_Endpoint *)malloc(sizeof(CTC_Endpoint));
    if(ctc_endpoint_detector == NULL){
        return NULL;
    }
    /*
    int subsampling_rate = 4;
    int num_bins = 80;
    int sample_rate = 16000;
    int frame_length = sample_rate / 1000 * 25;
    int frame_shift = sample_rate / 1000 * 10;

    int frame_shift_in_ms = subsampling_rate * frame_shift * 1000 / sample_rate;
    */
    ctc_endpoint_detector->abs_time_step = abs_time_step;
    ctc_endpoint_detector->vocab_size = vocab_size;
    ctc_endpoint_detector->blank = blank;
    ctc_endpoint_detector->blank_threshold = blank_threshold;
    ctc_endpoint_detector->frame_shift_in_ms = frame_shift_in_ms;
    ctc_endpoint_detector->num_frames_decoded = 0;
    ctc_endpoint_detector->num_frames_trailing_blank = 0;

    return ctc_endpoint_detector;
}

int ctc_endpoint_destroy(CTC_Endpoint *ctc_endpoint_detector) {
    if(ctc_endpoint_detector != NULL){
        free(ctc_endpoint_detector);
        ctc_endpoint_detector = NULL;
    }

    return 0;
}

int ctc_endpoint_process(CTC_Endpoint *ctc_endpoint_detector, float **logp, int decoded_something) {
    /// We support three rules. We terminate decoding if ANY of these rules
    /// evaluates to "true". If you want to add more rules, do it by changing this
    /// code. If you want to disable a rule, you can set the silence-timeout for
    /// that rule to a very large number.

    /// rule1 times out after 5000 ms of silence, even if we decoded nothing.
    CTC_Endpoint_Rule *rule1 = (CTC_Endpoint_Rule *)malloc(sizeof(CTC_Endpoint_Rule));
    rule1->must_decoded_sth = false;
    rule1->min_trailing_silence = 5000;
    rule1->min_utterance_length = 0;
    
    /// rule2 times out after 1000 ms of silence after decoding something.
    CTC_Endpoint_Rule *rule2 = (CTC_Endpoint_Rule *)malloc(sizeof(CTC_Endpoint_Rule));
    rule2->must_decoded_sth = true;
    rule2->min_trailing_silence = 1000;
    rule2->min_utterance_length = 0;

    /// rule3 times out after the utterance is 20000 ms long, regardless of anything else.
    CTC_Endpoint_Rule *rule3 = (CTC_Endpoint_Rule *)malloc(sizeof(CTC_Endpoint_Rule));
    rule3->must_decoded_sth = false;
    rule3->min_trailing_silence = 0;
    rule3->min_utterance_length = 20000;

    for (int t = 0; t < ctc_endpoint_detector->abs_time_step; t ++) {
        float blank_prob = exp(logp[t][ctc_endpoint_detector->blank]);

        ctc_endpoint_detector->num_frames_decoded ++;

        if (blank_prob > ctc_endpoint_detector->blank_threshold) {
            ctc_endpoint_detector->num_frames_trailing_blank ++;
        } else {
            ctc_endpoint_detector->num_frames_trailing_blank = 0;
        }
    }

    assert(CHECK_GE(ctc_endpoint_detector->num_frames_decoded, ctc_endpoint_detector->num_frames_trailing_blank));
    assert(CHECK_GT(ctc_endpoint_detector->frame_shift_in_ms, 0));

    int utterance_length = ctc_endpoint_detector->num_frames_decoded * ctc_endpoint_detector->frame_shift_in_ms;
    int trailing_silence = ctc_endpoint_detector->num_frames_trailing_blank * ctc_endpoint_detector->frame_shift_in_ms;

    if (RuleActivated(rule1, "rule1", decoded_something, trailing_silence, utterance_length)) {
        free(rule1);
        rule1 = NULL;
        free(rule2);
        rule2 = NULL;
        free(rule3);
        rule3 = NULL;

        return 1;
    }
    if (RuleActivated(rule2, "rule2", decoded_something, trailing_silence, utterance_length)) {
        free(rule1);
        rule1 = NULL;
        free(rule2);
        rule2 = NULL;
        free(rule3);
        rule3 = NULL;

        return 2;
    }
    if (RuleActivated(rule3, "rule3", decoded_something, trailing_silence, utterance_length)) {
        free(rule1);
        rule1 = NULL;
        free(rule2);
        rule2 = NULL;
        free(rule3);
        rule3 = NULL;

        return 3;
    }

    free(rule1);
    rule1 = NULL;
    free(rule2);
    rule2 = NULL;
    free(rule3);
    rule3 = NULL;

    return false;
}

