// Copyright (c) 2022 Personal (Bingbing Feng)

#ifndef __INVERSE_TEXT_NORMALIZATION_H__
#define __INVERSE_TEXT_NORMALIZATION_H__

#include <stdbool.h>

#define key_byte_max 20
#define value_byte_max 20

#define cardinal_numeral_s_max 28
#define cardinal_numeral_l_max 4

#define cardinal_numeral_en_s_max 9
#define cardinal_numeral_en_m1_max 10
#define cardinal_numeral_en_m2_max 8
#define cardinal_numeral_en_l_max 5
#define ordinal_numeral_en_s_max 9
#define ordinal_numeral_en_m1_max 10
#define ordinal_numeral_en_m2_max 8
#define ordinal_numeral_en_l_max 5

#define ordinal_numeral_en_denominator_max 8
#define cardinal_numeral_ar_numerator_max 8
#define ordinal_numeral_en_denominators_max 8

#define cardinal_numeral_plural_en_max 9
#define cardinal_numeral_singular_ar_max 9

#define cardinal_numeral_hour_ar_max 12
#define cardinal_numeral_minute_ar_max 50

#define month_name_en_abbr_max 12
#define ordinal_numeral_en_day_max 31

#define cardinal_numeral_ar_s_max 10

#define month_name_abbr_en_max 12
#define cardinal_numeral_ar_en_max 10

typedef struct _STR_INT_Pair_{

	char key[key_byte_max];
    long long value;

}STR_INT_Pair;

typedef struct _STR_STR_Pair_{

	char key[key_byte_max];
    char value[value_byte_max];

}STR_STR_Pair;

bool isNum(char *src, int str_size);
char *EnNum_to_ArNum(char **msg, int msg_size);
char **CardinalNumAndOrdinalNum(char **src_str, int str_size, int *res_size);
char **DecimalNum(char **src_str, int str_size, int *res_size);
char **PercentageNum(char **src_str, int str_size, int *res_size);
char **FractionNum(char **src_str, int str_size, int *res_size);
char **CenturyNotationAndDecadeNotation(char **src_str, int str_size, int *res_size);
char **ClockTimeNotation(char **src_str, int str_size, int *res_size);
char **CurrencyNotation(char **src_str, int str_size, int *res_size);
char **YearNotation(char **src_str, int str_size, int *res_size);
char **MonthDayNotation(char **src_str, int str_size, int *res_size);
char **ContinuousOneDigitNum(char **src_str, int str_size, int *res_size);
char **OneDigitNumTextNormalization(char **src_str, int str_size, int *res_size);
char **InverseTextNormalization(char **src_str, int str_size, int *res_size);

#endif
