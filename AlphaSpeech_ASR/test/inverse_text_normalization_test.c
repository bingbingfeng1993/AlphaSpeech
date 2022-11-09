// Copyright (c) 2022 Personal (Bingbing Feng)

#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../src/utils/inverse_text_normalization.h"

int main() {

    // char *msg[] = {"five", "hundred", "billion", "two", "hundred", "and", "thirty", "seven", "million", "one", "hundred", "and", "sixty", "six", "thousand", "two", "hundred", "and", "thirty", "four"};
    // char *msg[] = {"sixteen", "million", "two", "hundred", "and", "fifty", "thousand", "and", "sixty", "four"};
    // char *msg[] = {"two", "hundred", "and", "sixty", "trillion", "four", "hundred", "million", "and", "eight", "hundred"};

    // printf("%s\n", EnNum_to_ArNum(msg, 10));

    // char *str[] = {"But", "between", "two", "hundred", "and", "fifty", "and", "five", "hundred", "users", "the", "time", "doubles"};

    char *str[] = {
        "one", "hundreds", "one", "thousands", "nineteen", "hundreds", "one", "half", "and", "three", "quarters", "one", "and", "one", "third", "o", "ten", "point", "two", "three", 
        "ten", "per", "cent", "zero", "point", "four", "point", "one", "o", "point", "two", "three", "and", "one", "point", "o", "three", "twenty", "first", "o", 
        "one", "five", "two", "o", "o", "three", "four", "nine", "one", "two", "zero", "o", "one", "trillion", "dollors", "one", "thousand", "three", "hundred", "and", 
        "twentieth", "tenth", "one", "percent", "twenty", "five", "point", "six", "seven", "and", "nine", "tenths", "o", "in", "the", "sixties", "of", "the", "nineteenth", "century", 
        "in", "the", "eighteen", "sixties", "o", "o", "nineteen", "forty", "nine", "ten", "o", "one", "o", "o", "nine", "two", "fifty", "three", "nineteen", "o", 
        "two", "October", "thirty", "first", "twenty", "twenty", "the", "first", "of", "March", "two", "thousand", "and", "one", "six", "o", "five", "a", "m", "ten", 
        "zero", "five", "pm", "two", "forty", "five", "a", "m", "ten", "fifteen", "pm", "nine", "thirty", "six", "pounds", "and", "fifty", "pence", "six", "euros", 
        "and", "fifty", "cents", "six", "dollars", "and", "fifty", "cents", "six", "pounds", "and", "fifty", "six", "euros", "and", "fifty", "six", "dollars", "and", "fifty", 
        "six", "pounds", "fifty", "six", "euros", "fifty", "six", "dollars", "fifty", "forty", "pence", "fifty", "cents", "sixty", "euro", "cents", "o", "o", "o", "and", 
        "o", "zero", "o", "and", "one", "two", "and", "three", "four", "five"
    };

    int str_size = 190;
    
    int *res_size1 = (int *)malloc(sizeof(int));
    char **str_out1 = CardinalNumAndOrdinalNum(str, str_size, res_size1);

    for (int i = 0; i < res_size1[0]; i ++) {
        printf("%s ", str_out1[i]);
    }
    printf("\n\n");

    int *res_size2 = (int *)malloc(sizeof(int));
    char **str_out2 = DecimalNum(str_out1, res_size1[0], res_size2);

    for (int i = 0; i < res_size2[0]; i ++) {
        printf("%s ", str_out2[i]);
    }
    printf("\n\n");

    int *res_size3 = (int *)malloc(sizeof(int));
    char **str_out3 = PercentageNum(str_out2, res_size2[0], res_size3);

    for (int i = 0; i < res_size3[0]; i ++) {
        printf("%s ", str_out3[i]);
    }
    printf("\n\n");

    int *res_size4 = (int *)malloc(sizeof(int));
    char **str_out4 = FractionNum(str_out3, res_size3[0], res_size4);

    for (int i = 0; i < res_size4[0]; i ++) {
        printf("%s ", str_out4[i]);
    }
    printf("\n\n");

    int *res_size5 = (int *)malloc(sizeof(int));
    char **str_out5 = CenturyNotationAndDecadeNotation(str_out4, res_size4[0], res_size5);

    for (int i = 0; i < res_size5[0]; i ++) {
        printf("%s ", str_out5[i]);
    }
    printf("\n\n");

    int *res_size6 = (int *)malloc(sizeof(int));
    char **str_out6 = ClockTimeNotation(str_out5, res_size5[0], res_size6);

    for (int i = 0; i < res_size6[0]; i ++) {
        printf("%s ", str_out6[i]);
    }
    printf("\n\n");

    int *res_size7 = (int *)malloc(sizeof(int));
    char **str_out7 = CurrencyNotation(str_out6, res_size6[0], res_size7);

    for (int i = 0; i < res_size7[0]; i ++) {
        printf("%s ", str_out7[i]);
    }
    printf("\n\n");

    int *res_size8 = (int *)malloc(sizeof(int));
    char **str_out8 = YearNotation(str_out7, res_size7[0], res_size8);

    for (int i = 0; i < res_size8[0]; i ++) {
        printf("%s ", str_out8[i]);
    }
    printf("\n\n");

    int *res_size9 = (int *)malloc(sizeof(int));
    char **str_out9 = MonthDayNotation(str_out8, res_size8[0], res_size9);

    for (int i = 0; i < res_size9[0]; i ++) {
        printf("%s ", str_out9[i]);
    }
    printf("\n\n");

    int *res_size10 = (int *)malloc(sizeof(int));
    char **str_out10 = ContinuousOneDigitNum(str_out9, res_size9[0], res_size10);

    for (int i = 0; i < res_size10[0]; i ++) {
        printf("%s ", str_out10[i]);
    }
    printf("\n\n");

    int *res_size11 = (int *)malloc(sizeof(int));
    char **str_out11 = OneDigitNumTextNormalization(str_out10, res_size10[0], res_size11);

    for (int i = 0; i < res_size11[0]; i ++) {
        printf("%s ", str_out11[i]);
    }
    printf("\n\n");

    int *res_size12 = (int *)malloc(sizeof(int));
    char **str_out12 = InverseTextNormalization(str, str_size, res_size12);

    for (int i = 0; i < res_size12[0]; i ++) {
        printf("%s ", str_out12[i]);
    }
    printf("\n\n");

    return 0;
}

