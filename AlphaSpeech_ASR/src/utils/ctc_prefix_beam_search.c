// Copyright (c) 2022 Personal (Bingbing Feng)

#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "ctc_prefix_beam_search.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static inline float LogAdd(float x, float y) {
  static float num_min = -FLT_MAX;
  if (x <= num_min) return y;
  if (y <= num_min) return x;
  float max = MAX(x, y);
  return log(exp(x - max) + exp(y - max)) + max;
}

static inline int cmpfunc(const void *e1, const void *e2) {
    return *(float *)e1 < *(float *)e2 ? 1 : -1;
}

static inline void swap_flt(float *a, float *b) {
	float temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

static inline void swap_int(int *a, int *b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

static void TopK(float *data_v, int n, int k, float *values, int *indices) {
    int *data_i = (int *)malloc(sizeof(int)*n);
    for (int i = 0; i < n; i ++) {
        data_i[i] = i;
    }

    for (int i = 0; i < n - 1; i ++) {
        for (int j = n - 1; j > i; j --) {
            if (data_v[j] > data_v[j - 1]) {
                swap_flt(&data_v[j], &data_v[j - 1]);
                swap_int(&data_i[j], &data_i[j - 1]);
            }
        }
    }

    for (int i = 0; i < k; i ++) {
        values[i] = data_v[i];
        indices[i] = data_i[i];
    }

    free(data_i);
    data_i = NULL;
}

static int intcmp(int *arr_1, int len_1, int *arr_2, int len_2) {
    if (len_1 != len_2) return false;
    for (int i = 0; i < len_1; i ++) {
        if (arr_1[i] != arr_2[i]) return false;
    }

    return true;
}

CTC_Prefix_Beam_Search *ctc_prefix_beam_search_create(int abs_time_step, int vocab_size, int blank, int first_beam_size, int second_beam_size) {
    CTC_Prefix_Beam_Search *ctc_decoder = (CTC_Prefix_Beam_Search *)malloc(sizeof(CTC_Prefix_Beam_Search));
    if(ctc_decoder == NULL) {
        return NULL;
    }

    ctc_decoder->abs_time_step = abs_time_step;
    ctc_decoder->vocab_size = vocab_size;
    ctc_decoder->blank = blank;
    ctc_decoder->first_beam_size = first_beam_size;
    ctc_decoder->second_beam_size = second_beam_size;

    ctc_decoder->topk_index = (int *)malloc(sizeof(int)*first_beam_size);
    ctc_decoder->topk_score = (float *)malloc(sizeof(float)*first_beam_size);

    return ctc_decoder;
}

int ctc_prefix_beam_search_destroy(CTC_Prefix_Beam_Search *ctc_decoder) {
    if(ctc_decoder != NULL) {
        free(ctc_decoder->topk_index);
        ctc_decoder->topk_index = NULL;
        free(ctc_decoder->topk_score);
        ctc_decoder->topk_score = NULL;
        free(ctc_decoder);
        ctc_decoder = NULL;
    }

    return 0;
}

int ctc_prefix_beam_search_process(CTC_Prefix_Beam_Search *ctc_decoder, float **logp, int *result) {
    HYPS *cur_hyps = (HYPS *)malloc(sizeof(HYPS));
    int cur_hyps_dynamic_size = 0;

    cur_hyps[cur_hyps_dynamic_size].s = 0.0;
    cur_hyps[cur_hyps_dynamic_size].ns = -FLT_MAX;
    cur_hyps[cur_hyps_dynamic_size].score = LogAdd(cur_hyps[cur_hyps_dynamic_size].s, cur_hyps[cur_hyps_dynamic_size].ns);
    cur_hyps[cur_hyps_dynamic_size].prefix = NULL;
    cur_hyps[cur_hyps_dynamic_size].prefix_dynamic_size = 0;

    cur_hyps_dynamic_size ++;

    for (int t = 0; t < ctc_decoder->abs_time_step; t ++) {
        HYPS *next_hyps = (HYPS *)malloc(sizeof(HYPS));
        int next_hyps_dynamic_size = -1;

        // 1. First beam prune, only select topk best candidates
        TopK(logp[t], ctc_decoder->vocab_size, ctc_decoder->first_beam_size, ctc_decoder->topk_score, ctc_decoder->topk_index);

        // qsort(logp[t], ctc_decoder->vocab_size, sizeof(float), cmpfunc);

        // 2. Token passing
        for (int i = 0; i < ctc_decoder->first_beam_size; i ++) {
            int id = ctc_decoder->topk_index[i];
            float prob = ctc_decoder->topk_score[i];

            for (int j = 0; j < cur_hyps_dynamic_size; j ++) {
                if (id == ctc_decoder->blank) {
                    // Case 0: *a + ε => *a, *aε + ε => *a
                    int cur_hyps_prefix_exist_flag = 0;
                    for (int r = 0; r < next_hyps_dynamic_size + 1; r ++) {
                        if (intcmp(next_hyps[r].prefix, next_hyps[r].prefix_dynamic_size, cur_hyps[j].prefix, cur_hyps[j].prefix_dynamic_size) == 1) {
                            next_hyps[r].s = LogAdd(next_hyps[r].s, cur_hyps[j].score + prob);
                            next_hyps[r].score = LogAdd(next_hyps[r].s, next_hyps[r].ns);
                            cur_hyps_prefix_exist_flag = 1;
                            break;
                        }
                    }
                    if (cur_hyps_prefix_exist_flag == 0) {
                        next_hyps_dynamic_size ++;

                        next_hyps = (HYPS *)realloc(next_hyps, sizeof(HYPS)*(next_hyps_dynamic_size + 1));
                        next_hyps[next_hyps_dynamic_size].s = -FLT_MAX;
                        next_hyps[next_hyps_dynamic_size].ns = -FLT_MAX;
                        next_hyps[next_hyps_dynamic_size].score = LogAdd(next_hyps[next_hyps_dynamic_size].s, next_hyps[next_hyps_dynamic_size].ns);
                        if (cur_hyps[j].prefix_dynamic_size != 0) {
                            next_hyps[next_hyps_dynamic_size].prefix = (int *)malloc(sizeof(int)*cur_hyps[j].prefix_dynamic_size);
                            for (int r = 0; r < cur_hyps[j].prefix_dynamic_size; r ++) { next_hyps[next_hyps_dynamic_size].prefix[r] = cur_hyps[j].prefix[r]; }
                        } else { next_hyps[next_hyps_dynamic_size].prefix = NULL; }
                        next_hyps[next_hyps_dynamic_size].prefix_dynamic_size = cur_hyps[j].prefix_dynamic_size;
                        next_hyps[next_hyps_dynamic_size].s = LogAdd(next_hyps[next_hyps_dynamic_size].s, cur_hyps[j].score + prob);
                        next_hyps[next_hyps_dynamic_size].score = LogAdd(next_hyps[next_hyps_dynamic_size].s, next_hyps[next_hyps_dynamic_size].ns);
                    }

                    // PrefixScore& next_score = next_hyps[prefix];
                    // next_score.s = LogAdd(next_score.s, prefix_score.score() + prob);
                } else if (cur_hyps[j].prefix_dynamic_size != 0 && id == cur_hyps[j].prefix[cur_hyps[j].prefix_dynamic_size - 1]) {
                    // Case 1: *a + a => *a
                    int cur_hyps_prefix_exist_flag_1 = 0;
                    for (int r = 0; r < next_hyps_dynamic_size + 1; r ++) {
                        if (intcmp(next_hyps[r].prefix, next_hyps[r].prefix_dynamic_size, cur_hyps[j].prefix, cur_hyps[j].prefix_dynamic_size) == 1) {
                            next_hyps[r].ns = LogAdd(next_hyps[r].ns, cur_hyps[j].ns + prob);
                            next_hyps[r].score = LogAdd(next_hyps[r].s, next_hyps[r].ns);
                            cur_hyps_prefix_exist_flag_1 = 1;
                            break;
                        }
                    }
                    if (cur_hyps_prefix_exist_flag_1 == 0) {
                        next_hyps_dynamic_size ++;

                        next_hyps = (HYPS *)realloc(next_hyps, sizeof(HYPS)*(next_hyps_dynamic_size + 1));
                        next_hyps[next_hyps_dynamic_size].s = -FLT_MAX;
                        next_hyps[next_hyps_dynamic_size].ns = -FLT_MAX;
                        next_hyps[next_hyps_dynamic_size].score = LogAdd(next_hyps[next_hyps_dynamic_size].s, next_hyps[next_hyps_dynamic_size].ns);
                        next_hyps[next_hyps_dynamic_size].prefix = (int *)malloc(sizeof(int)*cur_hyps[j].prefix_dynamic_size);
                        for (int r = 0; r < cur_hyps[j].prefix_dynamic_size; r ++) { next_hyps[next_hyps_dynamic_size].prefix[r] = cur_hyps[j].prefix[r]; }
                        next_hyps[next_hyps_dynamic_size].prefix_dynamic_size = cur_hyps[j].prefix_dynamic_size;
                        next_hyps[next_hyps_dynamic_size].ns = LogAdd(next_hyps[next_hyps_dynamic_size].ns, cur_hyps[j].ns + prob);
                        next_hyps[next_hyps_dynamic_size].score = LogAdd(next_hyps[next_hyps_dynamic_size].s, next_hyps[next_hyps_dynamic_size].ns);
                    }

                    // PrefixScore& next_score1 = next_hyps[prefix];
                    // next_score1.ns = LogAdd(next_score1.ns, prefix_score.ns + prob);

                    // Case 2: *aε + a => *aa
                    int *n_prefix = (int *)malloc(sizeof(int)*(cur_hyps[j].prefix_dynamic_size + 1));
                    for (int r = 0; r < cur_hyps[j].prefix_dynamic_size; r ++) { n_prefix[r] = cur_hyps[j].prefix[r]; }
                    n_prefix[cur_hyps[j].prefix_dynamic_size] = id;

                    int cur_hyps_prefix_exist_flag_2 = 0;
                    for (int r = 0; r < next_hyps_dynamic_size + 1; r ++) {
                        if (intcmp(next_hyps[r].prefix, next_hyps[r].prefix_dynamic_size, n_prefix, cur_hyps[j].prefix_dynamic_size + 1) == 1) {
                            next_hyps[r].ns = LogAdd(next_hyps[r].ns, cur_hyps[j].s + prob);
                            next_hyps[r].score = LogAdd(next_hyps[r].s, next_hyps[r].ns);
                            cur_hyps_prefix_exist_flag_2 = 1;
                            break;
                        }
                    }
                    if (cur_hyps_prefix_exist_flag_2 == 0) {
                        next_hyps_dynamic_size ++;

                        next_hyps = (HYPS *)realloc(next_hyps, sizeof(HYPS)*(next_hyps_dynamic_size + 1));
                        next_hyps[next_hyps_dynamic_size].s = -FLT_MAX;
                        next_hyps[next_hyps_dynamic_size].ns = -FLT_MAX;
                        next_hyps[next_hyps_dynamic_size].score = LogAdd(next_hyps[next_hyps_dynamic_size].s, next_hyps[next_hyps_dynamic_size].ns);
                        next_hyps[next_hyps_dynamic_size].prefix = (int *)malloc(sizeof(int)*(cur_hyps[j].prefix_dynamic_size + 1));
                        for (int r = 0; r < cur_hyps[j].prefix_dynamic_size; r ++) { next_hyps[next_hyps_dynamic_size].prefix[r] = cur_hyps[j].prefix[r]; }
                        next_hyps[next_hyps_dynamic_size].prefix[cur_hyps[j].prefix_dynamic_size] = id;
                        next_hyps[next_hyps_dynamic_size].prefix_dynamic_size = cur_hyps[j].prefix_dynamic_size + 1;
                        next_hyps[next_hyps_dynamic_size].ns = LogAdd(next_hyps[next_hyps_dynamic_size].ns, cur_hyps[j].s + prob);
                        next_hyps[next_hyps_dynamic_size].score = LogAdd(next_hyps[next_hyps_dynamic_size].s, next_hyps[next_hyps_dynamic_size].ns);
                    }

                    free(n_prefix);
                    n_prefix = NULL;

                    // std::vector<int> new_prefix(prefix);
                    // new_prefix.emplace_back(id);
                    // PrefixScore& next_score2 = next_hyps[new_prefix];
                    // next_score2.ns = LogAdd(next_score2.ns, prefix_score.s + prob);
                } else {
                    // Case 3: *a + b => *ab, *aε + b => *ab
                    int *n_prefix = (int *)malloc(sizeof(int)*(cur_hyps[j].prefix_dynamic_size + 1));
                    for (int r = 0; r < cur_hyps[j].prefix_dynamic_size; r ++) { n_prefix[r] = cur_hyps[j].prefix[r]; }
                    n_prefix[cur_hyps[j].prefix_dynamic_size] = id;

                    int cur_hyps_prefix_exist_flag = 0;
                    for (int r = 0; r < next_hyps_dynamic_size + 1; r ++) {
                        if (intcmp(next_hyps[r].prefix, next_hyps[r].prefix_dynamic_size, n_prefix, cur_hyps[j].prefix_dynamic_size + 1) == 1) {
                            next_hyps[r].ns = LogAdd(next_hyps[r].ns, cur_hyps[j].score + prob);
                            next_hyps[r].score = LogAdd(next_hyps[r].s, next_hyps[r].ns);
                            cur_hyps_prefix_exist_flag = 1;
                            break;
                        }
                    }
                    if (cur_hyps_prefix_exist_flag == 0) {
                        next_hyps_dynamic_size ++;

                        next_hyps = (HYPS *)realloc(next_hyps, sizeof(HYPS)*(next_hyps_dynamic_size + 1));
                        next_hyps[next_hyps_dynamic_size].s = -FLT_MAX;
                        next_hyps[next_hyps_dynamic_size].ns = -FLT_MAX;
                        next_hyps[next_hyps_dynamic_size].score = LogAdd(next_hyps[next_hyps_dynamic_size].s, next_hyps[next_hyps_dynamic_size].ns);
                        next_hyps[next_hyps_dynamic_size].prefix = (int *)malloc(sizeof(int)*(cur_hyps[j].prefix_dynamic_size + 1));
                        for (int r = 0; r < cur_hyps[j].prefix_dynamic_size; r ++) { next_hyps[next_hyps_dynamic_size].prefix[r] = cur_hyps[j].prefix[r]; }
                        next_hyps[next_hyps_dynamic_size].prefix[cur_hyps[j].prefix_dynamic_size] = id;
                        next_hyps[next_hyps_dynamic_size].prefix_dynamic_size = cur_hyps[j].prefix_dynamic_size + 1;
                        next_hyps[next_hyps_dynamic_size].ns = LogAdd(next_hyps[next_hyps_dynamic_size].ns, cur_hyps[j].score + prob);
                        next_hyps[next_hyps_dynamic_size].score = LogAdd(next_hyps[next_hyps_dynamic_size].s, next_hyps[next_hyps_dynamic_size].ns);
                    }

                    free(n_prefix);
                    n_prefix = NULL;

                    // std::vector<int> new_prefix(prefix);
                    // new_prefix.emplace_back(id);
                    // PrefixScore& next_score = next_hyps[new_prefix];
                    // next_score.ns = LogAdd(next_score.ns, prefix_score.score() + prob);
                }
            }
        }

        // 3. Second beam prune, only keep topn best paths
        float *next_hyps_score_data_v = (float *)malloc(sizeof(float)*(next_hyps_dynamic_size + 1));
        for (int i = 0; i < next_hyps_dynamic_size + 1; i ++) { next_hyps_score_data_v[i] = next_hyps[i].score; }

        float *next_hyps_score_values = (float *)malloc(sizeof(float)*(next_hyps_dynamic_size + 1));
        int *next_hyps_score_indices = (int *)malloc(sizeof(int)*(next_hyps_dynamic_size + 1));

        TopK(next_hyps_score_data_v, next_hyps_dynamic_size + 1, next_hyps_dynamic_size + 1, next_hyps_score_values, next_hyps_score_indices);

        // 4. Update cur_hyps and get new result
        for (int i = 0; i < cur_hyps_dynamic_size; i ++) {
            free(cur_hyps[i].prefix);
            cur_hyps[i].prefix = NULL;
        }
        free(cur_hyps);
        cur_hyps = NULL;

        cur_hyps = (HYPS *)realloc(cur_hyps, sizeof(HYPS)*ctc_decoder->second_beam_size);

        for (int i = 0; i < ctc_decoder->second_beam_size; i ++) {
            cur_hyps[i].s = next_hyps[next_hyps_score_indices[i]].s;
            cur_hyps[i].ns = next_hyps[next_hyps_score_indices[i]].ns;
            cur_hyps[i].score = next_hyps[next_hyps_score_indices[i]].score;
            cur_hyps[i].prefix = (int *)malloc(sizeof(int)*next_hyps[next_hyps_score_indices[i]].prefix_dynamic_size);
            for (int j = 0; j < next_hyps[next_hyps_score_indices[i]].prefix_dynamic_size; j ++) { cur_hyps[i].prefix[j] = next_hyps[next_hyps_score_indices[i]].prefix[j]; }
            cur_hyps[i].prefix_dynamic_size = next_hyps[next_hyps_score_indices[i]].prefix_dynamic_size;
        }

        cur_hyps_dynamic_size = ctc_decoder->second_beam_size;

        for (int i = 0; i < next_hyps_dynamic_size + 1; i ++) {
            free(next_hyps[i].prefix);
            next_hyps[i].prefix = NULL;
        }        
        free(next_hyps);
        next_hyps = NULL;

        free(next_hyps_score_data_v);
        next_hyps_score_data_v = NULL;
        free(next_hyps_score_values);
        next_hyps_score_values = NULL;
        free(next_hyps_score_indices);
        next_hyps_score_indices = NULL;
    }

    for (int i = 0; i < cur_hyps[0].prefix_dynamic_size; i ++) { result[i] = cur_hyps[0].prefix[i]; }
    int best_prefix_dynamic_size = cur_hyps[0].prefix_dynamic_size;

    for (int i = 0; i < cur_hyps_dynamic_size; i ++) {
        free(cur_hyps[i].prefix);
        cur_hyps[i].prefix = NULL;
    }
    free(cur_hyps);
    cur_hyps = NULL;

    return best_prefix_dynamic_size;
}

