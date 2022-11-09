// Copyright (c) 2022 Personal (Bingbing Feng)

#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../src/utils/ctc_prefix_beam_search.h"

#ifndef UNIT_NUM
#define UNIT_NUM 4233
#endif

#ifndef UNIT_MAX
#define UNIT_MAX 20
#endif

typedef struct _Load_Dict_{

	char key[UNIT_MAX];
    char value[UNIT_MAX];

}Load_Dict;

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
    int first_beam_size = 3;
    int second_beam_size = 3;

    CTC_Prefix_Beam_Search *ctc_decoder = ctc_prefix_beam_search_create(abs_time_step, vocab_size, blank, first_beam_size, second_beam_size);

    int *result = (int *)malloc(sizeof(int)*abs_time_step);
    int best_prefix_dynamic_size = ctc_prefix_beam_search_process(ctc_decoder, logp, result);

    // load dict
	char dict_path[] = "../out/dict/lang_char_cn.txt";
	FILE *fp = fopen(dict_path, "r");
	Load_Dict char_dict[UNIT_NUM];
	for (int i = 0; fscanf(fp, "%s %s", char_dict[i].value, char_dict[i].key) != EOF; i ++) {}
	fclose(fp);

    for (int i = 0; i < best_prefix_dynamic_size; i ++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    for (int i = 0; i < best_prefix_dynamic_size; i ++) {
        printf("%s ", char_dict[result[i]].value);
    }
    printf("\n");

    int char_num = 1;
    char *content = (char *)malloc(sizeof(char)*char_num);
    content[0] = '\0';
    for (int i = 0; i < best_prefix_dynamic_size; i ++) {
        char_num += strlen(char_dict[result[i]].value);
        printf("%d\n", char_num);
        content = (char *)realloc(content, sizeof(char)*char_num);
        strcat(content, char_dict[result[i]].value);
        printf("%s\n", content);
    }
    printf("%s\n", content);

    free(content);
    content = NULL;

    return 0;
}

