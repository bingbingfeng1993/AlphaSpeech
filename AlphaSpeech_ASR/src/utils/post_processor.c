// Copyright (c) 2022 Personal (Bingbing Feng)

#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "post_processor.h"

void process_underscore(char *str) {
    int len = strlen(str);
    
    for (int i = 0; i < len; i ++) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 32;
        }

        if ((*str & 0x80) == 0x80) {
            *str = ' ';
        }

        str ++;
    }
}

void ltrim(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    int len = 0;
    char *p = str;

    while (*p != '\0' && isspace(*p)) {
        p ++;
        len ++;
    }

    memmove(str, p, strlen(str) - len + 1);
}

void rtrim(char *str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    int len = strlen(str);
    char *p = str + len - 1;
    
    while (p >= str && isspace(*p)) {
        *p = '\0';
        p --;
    }
}

void process_spaces(char *str) {
    int len = strlen(str);

    int j = 0;
    int blank_count = 0;

    for (int i = 0; i < len; i ++) {
        if (str[i] == ' ') {
            blank_count ++;
            if (blank_count <= 1) {
                str[j ++] = str[i];
            }
        } else {
            str[j ++] = str[i];
            blank_count = 0;
        }
    }

    str[j] = '\0';
}

void sentence_level_post_process(char *str) {
    process_underscore(str);
    ltrim(str);
    rtrim(str);
    process_spaces(str);
}

void word_level_post_process(char *str) {
    process_underscore(str);
    process_spaces(str);
}

