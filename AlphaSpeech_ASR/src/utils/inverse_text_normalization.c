// Copyright (c) 2022 Personal (Bingbing Feng)

#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "inverse_text_normalization.h"

bool isNum(char *str, int str_size) {
    for ( int i = 0; i < str_size; i ++ ) {
        int ascii = (int)str[i];
        if ( ( ascii >= 48 && ascii <= 57 ) || ascii == 44 ) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}


char *EnNum_to_ArNum(char **msg, int msg_size) {
    STR_INT_Pair cardinal_numeral_s[cardinal_numeral_s_max];

    strcpy(cardinal_numeral_s[0].key, "one");		    cardinal_numeral_s[0].value = 1;
    strcpy(cardinal_numeral_s[1].key, "two");		    cardinal_numeral_s[1].value = 2;
    strcpy(cardinal_numeral_s[2].key, "three");		    cardinal_numeral_s[2].value = 3;
	strcpy(cardinal_numeral_s[3].key, "four");		    cardinal_numeral_s[3].value = 4;
	strcpy(cardinal_numeral_s[4].key, "five");		    cardinal_numeral_s[4].value = 5;
	strcpy(cardinal_numeral_s[5].key, "six");		    cardinal_numeral_s[5].value = 6;
	strcpy(cardinal_numeral_s[6].key, "seven");		    cardinal_numeral_s[6].value = 7;
	strcpy(cardinal_numeral_s[7].key, "eight");		    cardinal_numeral_s[7].value = 8;
	strcpy(cardinal_numeral_s[8].key, "nine");		    cardinal_numeral_s[8].value = 9;
	strcpy(cardinal_numeral_s[9].key, "ten");		    cardinal_numeral_s[9].value = 10;
	strcpy(cardinal_numeral_s[10].key, "eleven");	    cardinal_numeral_s[10].value = 11;
	strcpy(cardinal_numeral_s[11].key, "twelve");	    cardinal_numeral_s[11].value = 12;
	strcpy(cardinal_numeral_s[12].key, "thirteen");	    cardinal_numeral_s[12].value = 13;
	strcpy(cardinal_numeral_s[13].key, "fourteen");	    cardinal_numeral_s[13].value = 14;
	strcpy(cardinal_numeral_s[14].key, "fifteen");	    cardinal_numeral_s[14].value = 15;
	strcpy(cardinal_numeral_s[15].key, "sixteen");	    cardinal_numeral_s[15].value = 16;
	strcpy(cardinal_numeral_s[16].key, "seventeen");    cardinal_numeral_s[16].value = 17;
	strcpy(cardinal_numeral_s[17].key, "eighteen");	    cardinal_numeral_s[17].value = 18;
	strcpy(cardinal_numeral_s[18].key, "nineteen");	    cardinal_numeral_s[18].value = 19;
	strcpy(cardinal_numeral_s[19].key, "twenty");	    cardinal_numeral_s[19].value = 20;
	strcpy(cardinal_numeral_s[20].key, "thirty");	    cardinal_numeral_s[20].value = 30;
	strcpy(cardinal_numeral_s[21].key, "forty");	    cardinal_numeral_s[21].value = 40;
	strcpy(cardinal_numeral_s[22].key, "fifty");	    cardinal_numeral_s[22].value = 50;
	strcpy(cardinal_numeral_s[23].key, "sixty");	    cardinal_numeral_s[23].value = 60;
	strcpy(cardinal_numeral_s[24].key, "seventy");	    cardinal_numeral_s[24].value = 70;
	strcpy(cardinal_numeral_s[25].key, "eighty");	    cardinal_numeral_s[25].value = 80;
	strcpy(cardinal_numeral_s[26].key, "ninety");	    cardinal_numeral_s[26].value = 90;
	strcpy(cardinal_numeral_s[27].key, "hundred");	    cardinal_numeral_s[27].value = 100;

    STR_INT_Pair cardinal_numeral_l[cardinal_numeral_l_max];

    strcpy(cardinal_numeral_l[0].key, "thousand");	    cardinal_numeral_l[0].value = 1000;
    strcpy(cardinal_numeral_l[1].key, "million");	    cardinal_numeral_l[1].value = 1000000;
    strcpy(cardinal_numeral_l[2].key, "billion");	    cardinal_numeral_l[2].value = 1000000000;
    strcpy(cardinal_numeral_l[3].key, "trillion");	    cardinal_numeral_l[3].value = 1000000000000;

    long long sum = 0;
    long long partial_sum = 0;

    for ( int i = 0; i < msg_size; i ++ ) {
        long long flag = 0;

        if ( msg[i] == "and" ) continue;

        for ( int x = 0; x < cardinal_numeral_s_max; x ++ ) {
            if ( !strcmp(cardinal_numeral_s[x].key, "hundred") ) {
                if ( !strcmp(msg[i], cardinal_numeral_s[x].key) ) {
                    partial_sum *= cardinal_numeral_s[x].value;
                    if ( i == msg_size - 1 ) {
                        sum += partial_sum;
                    }
                    flag = 1; break;
                }
            } else {
                if ( !strcmp(msg[i], cardinal_numeral_s[x].key) ) {
                    partial_sum += cardinal_numeral_s[x].value;
                    if ( i == msg_size - 1 ) {
                        sum += partial_sum;
                    }
                    flag = 1; break;
                }
            }
        }
        if ( flag == 1 ) continue;

        for ( int y = 0; y < cardinal_numeral_l_max; y ++ ) {
            if ( !strcmp(msg[i], cardinal_numeral_l[y].key) ) {
                partial_sum *= cardinal_numeral_l[y].value;
                sum += partial_sum;
                partial_sum = 0;
                break;
            }
        }
    }

    char *str = (char *)malloc(sizeof(char) * 256);
    sprintf(str, "%lld", sum);
    char *str_comma = (char *)malloc(sizeof(char) * 256);
    if ( strlen(str) > 4 ) {
        int j = 0;
        int index = strlen(str) % 3 > 0 ? strlen(str) % 3 : 3;
        
        for ( int i = 0; i < strlen(str); i ++ ) {
            if ( i == index ) {
                str_comma[j ++] = ',';
                str_comma[j ++] = str[i];

                index += 3;
            } else {
                str_comma[j ++] = str[i];
            }
        }
        
        str_comma[j] = '\0';
    } else {
        for ( int i = 0; i < strlen(str); i ++ ) { str_comma[i] = str[i]; }

        str_comma[strlen(str)] = '\0';
    }

    free(str);
    str = NULL;

    return str_comma;
}


char **CardinalNumAndOrdinalNum(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    char *cardinal_numeral_en_s[cardinal_numeral_en_s_max] = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine"
    };

    char *cardinal_numeral_en_m1[cardinal_numeral_en_m1_max] = {
        "ten",
        "eleven",
        "twelve",
        "thirteen",
        "fourteen",
        "fifteen",
        "sixteen",
        "seventeen",
        "eighteen",
        "nineteen"
    };

    char *cardinal_numeral_en_m2[cardinal_numeral_en_m2_max] = {
        "twenty",
        "thirty",
        "forty",
        "fifty",
        "sixty",
        "seventy",
        "eighty",
        "ninety"
    };

    char *cardinal_numeral_en_l[cardinal_numeral_en_l_max] = {
        "hundred",
        "thousand",
        "million",
        "billion",
        "trillion"
    };

    STR_STR_Pair ordinal_numeral_en_s[ordinal_numeral_en_s_max];

    strcpy(ordinal_numeral_en_s[0].key, "first");       strcpy(ordinal_numeral_en_s[0].value, "one");
    strcpy(ordinal_numeral_en_s[1].key, "second");      strcpy(ordinal_numeral_en_s[1].value, "two");
    strcpy(ordinal_numeral_en_s[2].key, "third");       strcpy(ordinal_numeral_en_s[2].value, "three");
    strcpy(ordinal_numeral_en_s[3].key, "fourth");      strcpy(ordinal_numeral_en_s[3].value, "four");
    strcpy(ordinal_numeral_en_s[4].key, "fifth");       strcpy(ordinal_numeral_en_s[4].value, "five");
    strcpy(ordinal_numeral_en_s[5].key, "sixth");       strcpy(ordinal_numeral_en_s[5].value, "six");
    strcpy(ordinal_numeral_en_s[6].key, "seventh");     strcpy(ordinal_numeral_en_s[6].value, "seven");
    strcpy(ordinal_numeral_en_s[7].key, "eighth");      strcpy(ordinal_numeral_en_s[7].value, "eight");
    strcpy(ordinal_numeral_en_s[8].key, "ninth");       strcpy(ordinal_numeral_en_s[8].value, "nine");

    STR_STR_Pair ordinal_numeral_en_m1[ordinal_numeral_en_m1_max];

    strcpy(ordinal_numeral_en_m1[0].key, "tenth");      strcpy(ordinal_numeral_en_m1[0].value, "ten");
    strcpy(ordinal_numeral_en_m1[1].key, "eleventh");   strcpy(ordinal_numeral_en_m1[1].value, "eleven");
    strcpy(ordinal_numeral_en_m1[2].key, "twelfth");    strcpy(ordinal_numeral_en_m1[2].value, "twelve");
    strcpy(ordinal_numeral_en_m1[3].key, "thirteenth"); strcpy(ordinal_numeral_en_m1[3].value, "thirteen");
    strcpy(ordinal_numeral_en_m1[4].key, "fourteenth"); strcpy(ordinal_numeral_en_m1[4].value, "fourteen");
    strcpy(ordinal_numeral_en_m1[5].key, "fifteenth");  strcpy(ordinal_numeral_en_m1[5].value, "fifteen");
    strcpy(ordinal_numeral_en_m1[6].key, "sixteenth");  strcpy(ordinal_numeral_en_m1[6].value, "sixteen");
    strcpy(ordinal_numeral_en_m1[7].key, "seventeenth");strcpy(ordinal_numeral_en_m1[7].value, "seventeen");
    strcpy(ordinal_numeral_en_m1[8].key, "eighteenth"); strcpy(ordinal_numeral_en_m1[8].value, "eighteen");
    strcpy(ordinal_numeral_en_m1[9].key, "nineteenth"); strcpy(ordinal_numeral_en_m1[9].value, "nineteen");

    STR_STR_Pair ordinal_numeral_en_m2[ordinal_numeral_en_m2_max];

    strcpy(ordinal_numeral_en_m2[0].key, "twentieth");  strcpy(ordinal_numeral_en_m2[0].value, "twenty");
    strcpy(ordinal_numeral_en_m2[1].key, "thirtieth");  strcpy(ordinal_numeral_en_m2[1].value, "thirty");
    strcpy(ordinal_numeral_en_m2[2].key, "fortieth");   strcpy(ordinal_numeral_en_m2[2].value, "forty");
    strcpy(ordinal_numeral_en_m2[3].key, "fiftieth");   strcpy(ordinal_numeral_en_m2[3].value, "fifty");
    strcpy(ordinal_numeral_en_m2[4].key, "sixtieth");   strcpy(ordinal_numeral_en_m2[4].value, "sixty");
    strcpy(ordinal_numeral_en_m2[5].key, "seventieth"); strcpy(ordinal_numeral_en_m2[5].value, "seventy");
    strcpy(ordinal_numeral_en_m2[6].key, "eightieth");  strcpy(ordinal_numeral_en_m2[6].value, "eighty");
    strcpy(ordinal_numeral_en_m2[7].key, "ninetieth");  strcpy(ordinal_numeral_en_m2[7].value, "ninety");

    STR_STR_Pair ordinal_numeral_en_l[ordinal_numeral_en_l_max];

    strcpy(ordinal_numeral_en_l[0].key, "hundredth");   strcpy(ordinal_numeral_en_l[0].value, "hundred");
    strcpy(ordinal_numeral_en_l[1].key, "thousandth");  strcpy(ordinal_numeral_en_l[1].value, "thousand");
    strcpy(ordinal_numeral_en_l[2].key, "millionth");   strcpy(ordinal_numeral_en_l[2].value, "million");
    strcpy(ordinal_numeral_en_l[3].key, "billionth");   strcpy(ordinal_numeral_en_l[3].value, "billion");
    strcpy(ordinal_numeral_en_l[4].key, "trillionth");  strcpy(ordinal_numeral_en_l[4].value, "trillion");

    long long *label = NULL;
    int label_size = 0;
    long long *stamp = NULL;
    int stamp_size = 0;

    char ***msg = NULL;
    int *msg_index = NULL;
    int msg_size = 0;
    char **msg_elem = NULL;
    int msg_elem_size = 0;
    
    long long toggle = 1;

    for ( int i = 0; i < str_size; i ++ ) {
        // printf("%s\n", str[i]);

        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;
        stamp_size += 1;
        stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
        stamp[stamp_size - 1] = 0;

        long long sign = 0;
        long long tag = 0;
        long long tab = 0;
        long long mark = 0;

        if ( toggle == 1 ) {
            for ( int j = 0; j < cardinal_numeral_en_s_max; j ++ ) {
                if ( !strcmp(str[i], cardinal_numeral_en_s[j]) ) {
                    msg_elem_size += 1;
                    msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                    msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                    msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                    if ( i == str_size - 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;
                    }
                    label[label_size - 1] = 1;
                    toggle = 0;
                    for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    break;
                }
            }

            for ( int j = 0; j < cardinal_numeral_en_m1_max; j ++ ) {
                if ( !strcmp(str[i], cardinal_numeral_en_m1[j]) ) {
                    msg_elem_size += 1;
                    msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                    msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                    msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                    if ( i == str_size - 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;
                    }
                    label[label_size - 1] = 1;
                    toggle = 0;
                    for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    break;
                }
            }

            for ( int j = 0; j < cardinal_numeral_en_m2_max; j ++ ) {
                if ( !strcmp(str[i], cardinal_numeral_en_m2[j]) ) {
                    msg_elem_size += 1;
                    msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                    msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                    msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                    if ( i == str_size - 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;
                    }
                    label[label_size - 1] = 1;
                    toggle = 0;
                    for ( int z = 0; z < ordinal_numeral_en_s_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_s[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_s[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_s[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_s[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_s[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    break;
                }
            }
        } else {
            for ( int j = 0; j < cardinal_numeral_en_s_max; j ++ ) {
                if ( !strcmp(str[i], cardinal_numeral_en_s[j]) ) {
                    for ( int k = 0; k < cardinal_numeral_en_s_max; k ++ ) {
                        if ( !strcmp(msg_elem[msg_elem_size - 1], cardinal_numeral_en_s[k]) ) {
                            sign = 1;
                            break;
                        }
                    }
                    for ( int k = 0; k < cardinal_numeral_en_m1_max; k ++ ) {
                        if ( !strcmp(msg_elem[msg_elem_size - 1], cardinal_numeral_en_m1[k]) ) {
                            sign = 1;
                            break;
                        }
                    }
                    if ( sign == 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;

                        msg_elem_size += 1;
                        msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                        msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                        msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                        if ( i == str_size - 1 ) {
                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;
                        }
                        label[label_size - 1] = 1;
                        for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                            if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                                msg_elem_size += 1;
                                msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                                msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                                for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                                msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                                msg_size += 1;
                                msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                                msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                                }
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                        msg[msg_size - 1][s][t] = msg_elem[s][t];
                                    }
                                    msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                                }
                                msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                                msg_index[msg_size - 1] = msg_elem_size;
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    free(msg_elem[s]);
                                    msg_elem[s] = NULL;
                                }
                                free(msg_elem);
                                msg_elem = NULL;
                                msg_elem_size = 0;

                                label_size += 1;
                                label = (long long *)realloc(label, sizeof(long long) * label_size);
                                label[label_size - 1] = 2;
                                stamp_size += 1;
                                stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                                stamp[stamp_size - 1] = 3;

                                i += 1;
                                toggle = 1;
                                break;
                            }
                        }
                        tag = 1;
                        break;
                    }

                    msg_elem_size += 1;
                    msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                    msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                    msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                    if ( i == str_size - 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;
                    }
                    label[label_size - 1] = 2;
                    for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    tag = 1;
                    break;
                }
            }

            for ( int j = 0; j < cardinal_numeral_en_m1_max; j ++ ) {
                if ( !strcmp(str[i], cardinal_numeral_en_m1[j]) ) {
                    for ( int k = 0; k < cardinal_numeral_en_s_max; k ++ ) {
                        if ( !strcmp(msg_elem[msg_elem_size - 1], cardinal_numeral_en_s[k]) ) {
                            sign = 1;
                            break;
                        }
                    }
                    for ( int k = 0; k < cardinal_numeral_en_m1_max; k ++ ) {
                        if ( !strcmp(msg_elem[msg_elem_size - 1], cardinal_numeral_en_m1[k]) ) {
                            sign = 1;
                            break;
                        }
                    }
                    for ( int k = 0; k < cardinal_numeral_en_m2_max; k ++ ) {
                        if ( !strcmp(msg_elem[msg_elem_size - 1], cardinal_numeral_en_m2[k]) ) {
                            sign = 1;
                            break;
                        }
                    }
                    if ( sign == 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;

                        msg_elem_size += 1;
                        msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                        msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                        msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                        if ( i == str_size - 1 ) {
                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;
                        }
                        label[label_size - 1] = 1;
                        for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                            if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                                msg_elem_size += 1;
                                msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                                msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                                for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                                msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                                msg_size += 1;
                                msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                                msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                                }
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                        msg[msg_size - 1][s][t] = msg_elem[s][t];
                                    }
                                    msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                                }
                                msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                                msg_index[msg_size - 1] = msg_elem_size;
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    free(msg_elem[s]);
                                    msg_elem[s] = NULL;
                                }
                                free(msg_elem);
                                msg_elem = NULL;
                                msg_elem_size = 0;

                                label_size += 1;
                                label = (long long *)realloc(label, sizeof(long long) * label_size);
                                label[label_size - 1] = 2;
                                stamp_size += 1;
                                stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                                stamp[stamp_size - 1] = 3;

                                i += 1;
                                toggle = 1;
                                break;
                            }
                        }
                        tag = 1;
                        break;
                    }

                    msg_elem_size += 1;
                    msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                    msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                    msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                    if ( i == str_size - 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;
                    }
                    label[label_size - 1] = 2;
                    for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    tag = 1;
                    break;
                }
            }

            for ( int j = 0; j < cardinal_numeral_en_m2_max; j ++ ) {
                if ( !strcmp(str[i], cardinal_numeral_en_m2[j]) ) {
                    for ( int k = 0; k < cardinal_numeral_en_s_max; k ++ ) {
                        if ( !strcmp(msg_elem[msg_elem_size - 1], cardinal_numeral_en_s[k]) ) {
                            sign = 1;
                            break;
                        }
                    }
                    for ( int k = 0; k < cardinal_numeral_en_m1_max; k ++ ) {
                        if ( !strcmp(msg_elem[msg_elem_size - 1], cardinal_numeral_en_m1[k]) ) {
                            sign = 1;
                            break;
                        }
                    }
                    for ( int k = 0; k < cardinal_numeral_en_m2_max; k ++ ) {
                        if ( !strcmp(msg_elem[msg_elem_size - 1], cardinal_numeral_en_m2[k]) ) {
                            sign = 1;
                            break;
                        }
                    }
                    if ( sign == 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;

                        msg_elem_size += 1;
                        msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                        msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                        msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                        if ( i == str_size - 1 ) {
                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;
                        }
                        label[label_size - 1] = 1;
                        for ( int z = 0; z < ordinal_numeral_en_s_max; z ++ ) {
                            if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_s[z].key) ) {
                                msg_elem_size += 1;
                                msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                                msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_s[z].value) + 1));
                                for (int r = 0; r < strlen(ordinal_numeral_en_s[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_s[z].value[r]; }
                                msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_s[z].value)] = '\0';

                                msg_size += 1;
                                msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                                msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                                }
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                        msg[msg_size - 1][s][t] = msg_elem[s][t];
                                    }
                                    msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                                }
                                msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                                msg_index[msg_size - 1] = msg_elem_size;
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    free(msg_elem[s]);
                                    msg_elem[s] = NULL;
                                }
                                free(msg_elem);
                                msg_elem = NULL;
                                msg_elem_size = 0;

                                label_size += 1;
                                label = (long long *)realloc(label, sizeof(long long) * label_size);
                                label[label_size - 1] = 2;
                                stamp_size += 1;
                                stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                                stamp[stamp_size - 1] = 3;

                                i += 1;
                                toggle = 1;
                                break;
                            }
                        }
                        for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                            if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                                msg_elem_size += 1;
                                msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                                msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                                for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                                msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                                msg_size += 1;
                                msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                                msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                                }
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                        msg[msg_size - 1][s][t] = msg_elem[s][t];
                                    }
                                    msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                                }
                                msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                                msg_index[msg_size - 1] = msg_elem_size;
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    free(msg_elem[s]);
                                    msg_elem[s] = NULL;
                                }
                                free(msg_elem);
                                msg_elem = NULL;
                                msg_elem_size = 0;

                                label_size += 1;
                                label = (long long *)realloc(label, sizeof(long long) * label_size);
                                label[label_size - 1] = 2;
                                stamp_size += 1;
                                stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                                stamp[stamp_size - 1] = 3;

                                i += 1;
                                toggle = 1;
                                break;
                            }
                        }
                        tag = 1;
                        break;
                    }

                    msg_elem_size += 1;
                    msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                    msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                    msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                    if ( i == str_size - 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;
                    }
                    label[label_size - 1] = 2;
                    for ( int z = 0; z < ordinal_numeral_en_s_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_s[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_s[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_s[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_s[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_s[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    tag = 1;
                    break;
                }
            }

            for ( int j = 0; j < cardinal_numeral_en_l_max; j ++ ) {
                if ( !strcmp(str[i], cardinal_numeral_en_l[j]) ) {
                    msg_elem_size += 1;
                    msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                    msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                    msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                    if ( i == str_size - 1 ) {
                        msg_size += 1;
                        msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                        msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                        for (int s = 0; s < msg_elem_size; s ++) {
                            msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                        }
                        for (int s = 0; s < msg_elem_size; s ++) {
                            for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                msg[msg_size - 1][s][t] = msg_elem[s][t];
                            }
                            msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                        }
                        msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                        msg_index[msg_size - 1] = msg_elem_size;
                        for (int s = 0; s < msg_elem_size; s ++) {
                            free(msg_elem[s]);
                            msg_elem[s] = NULL;
                        }
                        free(msg_elem);
                        msg_elem = NULL;
                        msg_elem_size = 0;
                    }
                    label[label_size - 1] = 2;
                    for ( int z = 0; z < ordinal_numeral_en_s_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_s[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_s[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_s[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_s[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_s[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    for ( int z = 0; z < ordinal_numeral_en_m1_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_m1[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_m1[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_m1[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_m1[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_m1[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    for ( int z = 0; z < ordinal_numeral_en_m2_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_m2[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_m2[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_m2[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_m2[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_m2[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    for ( int z = 0; z < ordinal_numeral_en_l_max; z ++ ) {
                        if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_l[z].key) ) {
                            msg_elem_size += 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_l[z].value) + 1));
                            for (int r = 0; r < strlen(ordinal_numeral_en_l[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_l[z].value[r]; }
                            msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_l[z].value)] = '\0';

                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;

                            label_size += 1;
                            label = (long long *)realloc(label, sizeof(long long) * label_size);
                            label[label_size - 1] = 2;
                            stamp_size += 1;
                            stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                            stamp[stamp_size - 1] = 3;

                            i += 1;
                            toggle = 1;
                            break;
                        }
                    }
                    tag = 1;
                    break;
                }
            }

            if ( tag == 0 ) {
                if ( !strcmp(str[i], "and") ) {
                    for ( int j = 0; j < cardinal_numeral_en_l_max; j ++ ) {
                        if ( !strcmp(msg_elem[msg_elem_size - 1], cardinal_numeral_en_l[j]) ) {
                            tab = 1;
                            break;
                        }
                    }
                    if ( tab == 1 ) {
                        msg_elem_size += 1;
                        msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                        msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { msg_elem[msg_elem_size - 1][r] = str[i][r]; }
                        msg_elem[msg_elem_size - 1][strlen(str[i])] = '\0';
                        label[label_size - 1] = 2;
                        if ( i == str_size - 1 ) {
                            msg_elem_size -= 1;
                            msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                            label[label_size - 1] = 0;
                            msg_size += 1;
                            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                            for (int s = 0; s < msg_elem_size; s ++) {
                                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                            }
                            for (int s = 0; s < msg_elem_size; s ++) {
                                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                                }
                                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                            }
                            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                            msg_index[msg_size - 1] = msg_elem_size;
                            for (int s = 0; s < msg_elem_size; s ++) {
                                free(msg_elem[s]);
                                msg_elem[s] = NULL;
                            }
                            free(msg_elem);
                            msg_elem = NULL;
                            msg_elem_size = 0;
                        }
                        for ( int z = 0; z < ordinal_numeral_en_s_max; z ++ ) {
                            if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_s[z].key) ) {
                                msg_elem_size += 1;
                                msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                                msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_s[z].value) + 1));
                                for (int r = 0; r < strlen(ordinal_numeral_en_s[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_s[z].value[r]; }
                                msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_s[z].value)] = '\0';

                                msg_size += 1;
                                msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                                msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                                }
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                        msg[msg_size - 1][s][t] = msg_elem[s][t];
                                    }
                                    msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                                }
                                msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                                msg_index[msg_size - 1] = msg_elem_size;
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    free(msg_elem[s]);
                                    msg_elem[s] = NULL;
                                }
                                free(msg_elem);
                                msg_elem = NULL;
                                msg_elem_size = 0;

                                label_size += 1;
                                label = (long long *)realloc(label, sizeof(long long) * label_size);
                                label[label_size - 1] = 2;
                                stamp_size += 1;
                                stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                                stamp[stamp_size - 1] = 3;

                                i += 1;
                                toggle = 1;
                                break;
                            }
                        }
                        for ( int z = 0; z < ordinal_numeral_en_m1_max; z ++ ) {
                            if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_m1[z].key) ) {
                                msg_elem_size += 1;
                                msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                                msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_m1[z].value) + 1));
                                for (int r = 0; r < strlen(ordinal_numeral_en_m1[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_m1[z].value[r]; }
                                msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_m1[z].value)] = '\0';

                                msg_size += 1;
                                msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                                msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                                }
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                        msg[msg_size - 1][s][t] = msg_elem[s][t];
                                    }
                                    msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                                }
                                msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                                msg_index[msg_size - 1] = msg_elem_size;
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    free(msg_elem[s]);
                                    msg_elem[s] = NULL;
                                }
                                free(msg_elem);
                                msg_elem = NULL;
                                msg_elem_size = 0;

                                label_size += 1;
                                label = (long long *)realloc(label, sizeof(long long) * label_size);
                                label[label_size - 1] = 2;
                                stamp_size += 1;
                                stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                                stamp[stamp_size - 1] = 3;

                                i += 1;
                                toggle = 1;
                                break;
                            }
                        }
                        for ( int z = 0; z < ordinal_numeral_en_m2_max; z ++ ) {
                            if ( i < str_size - 1 && !strcmp(str[i + 1], ordinal_numeral_en_m2[z].key) ) {
                                msg_elem_size += 1;
                                msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                                msg_elem[msg_elem_size - 1] = (char *)malloc(sizeof(char) * (strlen(ordinal_numeral_en_m2[z].value) + 1));
                                for (int r = 0; r < strlen(ordinal_numeral_en_m2[z].value); r ++) { msg_elem[msg_elem_size - 1][r] = ordinal_numeral_en_m2[z].value[r]; }
                                msg_elem[msg_elem_size - 1][strlen(ordinal_numeral_en_m2[z].value)] = '\0';

                                msg_size += 1;
                                msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
                                msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
                                }
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                                        msg[msg_size - 1][s][t] = msg_elem[s][t];
                                    }
                                    msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
                                }
                                msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
                                msg_index[msg_size - 1] = msg_elem_size;
                                for (int s = 0; s < msg_elem_size; s ++) {
                                    free(msg_elem[s]);
                                    msg_elem[s] = NULL;
                                }
                                free(msg_elem);
                                msg_elem = NULL;
                                msg_elem_size = 0;

                                label_size += 1;
                                label = (long long *)realloc(label, sizeof(long long) * label_size);
                                label[label_size - 1] = 2;
                                stamp_size += 1;
                                stamp = (long long *)realloc(stamp, sizeof(long long) * stamp_size);
                                stamp[stamp_size - 1] = 3;

                                i += 1;
                                toggle = 1;
                                break;
                            }
                        }
                    } else {
                        toggle = 1;
                        mark = 1;
                    }
                } else {
                    toggle = 1;
                    mark = 1;
                }
            }
        }

        if ( mark == 1 ) { //Each year, foodborne illness strikes 48 million Americans, hospitalizing a hundred thousand and killing thousands.
            if ( !strcmp(msg_elem[msg_elem_size - 1], "and") ) {
                msg_elem_size -= 1;
                msg_elem = (char **)realloc(msg_elem, sizeof(char *) * msg_elem_size);
                label[label_size - 2] = 0;
                label[label_size - 1] = 0;
            }
            msg_size += 1;
            msg = (char ***)realloc(msg, sizeof(char **) * msg_size);
            msg[msg_size - 1] = (char **)malloc(sizeof(char *) * msg_elem_size);
            for (int s = 0; s < msg_elem_size; s ++) {
                msg[msg_size - 1][s] = (char *)malloc(sizeof(char) * strlen(msg_elem[s] + 1));
            }
            for (int s = 0; s < msg_elem_size; s ++) {
                for (int t = 0; t < strlen(msg_elem[s]); t ++) {
                    msg[msg_size - 1][s][t] = msg_elem[s][t];
                }
                msg[msg_size - 1][s][strlen(msg_elem[s])] = '\0';
            }
            msg_index = (int *)realloc(msg_index, sizeof(int) * msg_size);
            msg_index[msg_size - 1] = msg_elem_size;
            for (int s = 0; s < msg_elem_size; s ++) {
                free(msg_elem[s]);
                msg_elem[s] = NULL;
            }
            free(msg_elem);
            msg_elem = NULL;
            msg_elem_size = 0;
        }
    }

    long long vec_k = 0;
    char **str_out = NULL;
    int str_out_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        if ( !strcmp(str[i], "zero") ) strcpy(str[i], "0");

        if ( label[i] == 0 ) {
            if ( !strcmp(str[i], "tenth") ) strcpy(str[i], "10th");
            if ( !strcmp(str[i], "eleventh") ) strcpy(str[i], "11th");
            if ( !strcmp(str[i], "twelfth") ) strcpy(str[i], "12th");
            if ( !strcmp(str[i], "thirteenth") ) strcpy(str[i], "13th");
            if ( !strcmp(str[i], "fourteenth") ) strcpy(str[i], "14th");
            if ( !strcmp(str[i], "fifteenth") ) strcpy(str[i], "15th");
            if ( !strcmp(str[i], "sixteenth") ) strcpy(str[i], "16th");
            if ( !strcmp(str[i], "seventeenth") ) strcpy(str[i], "17th");
            if ( !strcmp(str[i], "eighteenth") ) strcpy(str[i], "18th");
            if ( !strcmp(str[i], "nineteenth") ) strcpy(str[i], "19th");
            if ( !strcmp(str[i], "twentieth") ) strcpy(str[i], "20th");
            if ( !strcmp(str[i], "thirtieth") ) strcpy(str[i], "30th");
            if ( !strcmp(str[i], "fortieth") ) strcpy(str[i], "40th");
            if ( !strcmp(str[i], "fiftieth") ) strcpy(str[i], "50th");
            if ( !strcmp(str[i], "sixtieth") ) strcpy(str[i], "60th");
            if ( !strcmp(str[i], "seventieth") ) strcpy(str[i], "70th");
            if ( !strcmp(str[i], "eightieth") ) strcpy(str[i], "80th");
            if ( !strcmp(str[i], "ninetieth") ) strcpy(str[i], "90th");

            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * strlen(str[i] + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
            str_out[str_out_size - 1][strlen(str[i])] = '\0';
        } else if (label[i] == 1) {
            if ( stamp[i + msg_index[vec_k] - 1] == 3 ) {
                int temp_size = strlen(EnNum_to_ArNum(msg[vec_k], msg_index[vec_k])) + 2;
                char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                for (int r = 0; r < strlen(EnNum_to_ArNum(msg[vec_k], msg_index[vec_k])); r ++) { temp[r] = EnNum_to_ArNum(msg[vec_k], msg_index[vec_k])[r]; }
                temp[strlen(EnNum_to_ArNum(msg[vec_k], msg_index[vec_k]))] = str[i + msg_index[vec_k] - 1][strlen(str[i + msg_index[vec_k] - 1]) - 2];
                temp[strlen(EnNum_to_ArNum(msg[vec_k], msg_index[vec_k])) + 1] = str[i + msg_index[vec_k] - 1][strlen(str[i + msg_index[vec_k] - 1]) - 2 + 1];
                temp[strlen(EnNum_to_ArNum(msg[vec_k], msg_index[vec_k])) + 2] = '\0';

                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                free(temp);
                temp = NULL;
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(EnNum_to_ArNum(msg[vec_k], msg_index[vec_k])) + 1));
                for (int r = 0; r < strlen(EnNum_to_ArNum(msg[vec_k], msg_index[vec_k])); r ++) { str_out[str_out_size - 1][r] = EnNum_to_ArNum(msg[vec_k], msg_index[vec_k])[r]; }
                str_out[str_out_size - 1][strlen(EnNum_to_ArNum(msg[vec_k], msg_index[vec_k]))] = '\0';
            }
            vec_k += 1;
        }
    }

    free(label);
    label = NULL;
    free(stamp);
    stamp = NULL;
    for (int i = 0; i < msg_size; i ++) {
        for (int j = 0; j < msg_index[i]; j ++) {
            free(msg[i][j]);
            msg[i][j] = NULL;
        }
        free(msg[i]);
        msg[i] = NULL;
    }
    free(msg);
    msg = NULL;
    free(msg_index);
    msg_index = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **DecimalNum(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    long long *label = NULL;
    int label_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;

        if ( isNum(str[i], strlen(str[i])) == 1 ) {
            label[label_size - 1] = strlen(str[i]);
        }
    }

    char **str_out = NULL;
    int str_out_size = 0;
    long long convert = 1;
    for ( int i = 0; i < str_size; i ++ ) {
        if ( convert == 1 ) {
            if ( !strcmp(str[i], "o") || label[i] != 0 ) {
                if ( i < str_size - 1 && !strcmp(str[i + 1], "point") ) {
                    if ( i < str_size - 2 && ( label[i + 2] == 1 || !strcmp(str[i + 2], "o") ) ) {
                        if ( !strcmp(str[i], "o") ) strcpy(str[i], "0");
                        if ( !strcmp(str[i + 2], "o") ) strcpy(str[i + 2], "0");

                        int temp_size = strlen(str[i]) + 1 + strlen(str[i + 2]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                        temp[r ++] = '.';
                        for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        convert = 0;
                        if ( i == str_size - 3 || ( i < str_size - 3 && label[i + 3] != 1 && strcmp(str[i + 3], "o") ) ) { convert = 1; }
                        i += 2;
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                        str_out[str_out_size - 1][strlen(str[i])] = '\0';
                    }
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else if ( !strcmp(str[i], "point") ) {
                if ( i < str_size - 1 && ( label[i + 1] == 1 || !strcmp(str[i + 1], "o") ) ) {
                    if ( !strcmp(str[i + 1], "o") ) strcpy(str[i + 1], "0");

                    int temp_size = 1 + strlen(str[i + 1]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '.';
                    for (int t = 0; t < strlen(str[i + 1]); t ++) { temp[r ++] = str[i + 1][t]; }
                    temp[r] = '\0';

                    if ( str_out_size > 0 && strlen(str_out[str_out_size - 1]) == 1 && isNum(str_out[str_out_size - 1], strlen(str_out[str_out_size - 1])) == 1 ) {
                        int cur_str_out_index_size = strlen(str_out[str_out_size - 1]);
                        str_out[str_out_size - 1] = (char *)realloc(str_out[str_out_size - 1], sizeof(char) * (cur_str_out_index_size + temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][cur_str_out_index_size + r] = temp[r]; }
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }
                    }

                    free(temp);
                    temp = NULL;

                    convert = 0;
                    if ( i == str_size - 2 || ( i < str_size - 2 && label[i + 2] != 1 && strcmp(str[i + 2], "o") ) ) { convert = 1; }
                    i += 1;
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else {
            if ( !strcmp(str[i], "o") ) strcpy(str[i], "0");
            if ( i < str_size - 1 && !strcmp(str[i + 1], "point") ) {
                if ( i < str_size - 2 && ( label[i + 2] == 1 || !strcmp(str[i + 2], "o") ) ) {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';

                    convert = 1;
                    continue;
                }
            }
            int cur_str_out_index_size = strlen(str_out[str_out_size - 1]);
            str_out[str_out_size - 1] = (char *)realloc(str_out[str_out_size - 1], sizeof(char) * (cur_str_out_index_size + strlen(str[i]) + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][cur_str_out_index_size + r] = str[i][r]; }
            str_out[str_out_size - 1][cur_str_out_index_size + strlen(str[i])] = '\0';

            if ( i == str_size - 1 || ( i < str_size - 1 && label[i + 1] != 1 && strcmp(str[i + 1], "o") ) ) { convert = 1; }
        }
    }

    free(label);
    label = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **PercentageNum(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    long long *label = NULL;
    int label_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;

        if ( isNum(str[i], strlen(str[i])) == 1 ) {
            label[label_size - 1] = 1;
        }
    }

    char **str_out = NULL;
    int str_out_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        if ( label[i] == 1 ) {
            if ( i < str_size - 1 && !strcmp(str[i + 1], "per") ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "cent") ) {
                    int temp_size = strlen(str[i]) + 1;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '%';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;

                    i += 2;
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "percent") ) {
                int temp_size = strlen(str[i]) + 1;
                char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                int r = 0;
                for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                temp[r ++] = '%';
                temp[r] = '\0';

                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                free(temp);
                temp = NULL;

                i += 1;
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else {
            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
            str_out[str_out_size - 1][strlen(str[i])] = '\0';
        }
    }

    free(label);
    label = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **FractionNum(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    STR_STR_Pair ordinal_numeral_en_denominator[ordinal_numeral_en_denominator_max];

    strcpy(ordinal_numeral_en_denominator[0].key, "third");     strcpy(ordinal_numeral_en_denominator[0].value, "3");
    strcpy(ordinal_numeral_en_denominator[1].key, "fourth");    strcpy(ordinal_numeral_en_denominator[1].value, "4");
    strcpy(ordinal_numeral_en_denominator[2].key, "fifth");     strcpy(ordinal_numeral_en_denominator[2].value, "5");
    strcpy(ordinal_numeral_en_denominator[3].key, "sixth");     strcpy(ordinal_numeral_en_denominator[3].value, "6");
    strcpy(ordinal_numeral_en_denominator[4].key, "seventh");   strcpy(ordinal_numeral_en_denominator[4].value, "7");
    strcpy(ordinal_numeral_en_denominator[5].key, "eighth");    strcpy(ordinal_numeral_en_denominator[5].value, "8");
    strcpy(ordinal_numeral_en_denominator[6].key, "ninth");     strcpy(ordinal_numeral_en_denominator[6].value, "9");
    strcpy(ordinal_numeral_en_denominator[7].key, "10th");      strcpy(ordinal_numeral_en_denominator[7].value, "10");

    char *cardinal_numeral_ar_numerator[cardinal_numeral_ar_numerator_max] = {
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9"
    };

    STR_STR_Pair ordinal_numeral_en_denominators[ordinal_numeral_en_denominators_max];

    strcpy(ordinal_numeral_en_denominators[0].key, "thirds");   strcpy(ordinal_numeral_en_denominators[0].value, "3");
    strcpy(ordinal_numeral_en_denominators[1].key, "fourths");  strcpy(ordinal_numeral_en_denominators[1].value, "4");
    strcpy(ordinal_numeral_en_denominators[2].key, "fifths");   strcpy(ordinal_numeral_en_denominators[2].value, "5");
    strcpy(ordinal_numeral_en_denominators[3].key, "sixths");   strcpy(ordinal_numeral_en_denominators[3].value, "6");
    strcpy(ordinal_numeral_en_denominators[4].key, "sevenths"); strcpy(ordinal_numeral_en_denominators[4].value, "7");
    strcpy(ordinal_numeral_en_denominators[5].key, "eighths");  strcpy(ordinal_numeral_en_denominators[5].value, "8");
    strcpy(ordinal_numeral_en_denominators[6].key, "ninths");   strcpy(ordinal_numeral_en_denominators[6].value, "9");
    strcpy(ordinal_numeral_en_denominators[7].key, "tenths");   strcpy(ordinal_numeral_en_denominators[7].value, "10");

    long long *label = NULL;
    int label_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;

        for ( int x = 0; x < ordinal_numeral_en_denominator_max; x ++ ) {
            if ( !strcmp(str[i], ordinal_numeral_en_denominator[x].key) ) {
                label[label_size - 1] = 1;
                break;
            }
        }
        for ( int j = 0; j < cardinal_numeral_ar_numerator_max; j ++ ) {
            if ( !strcmp(str[i], cardinal_numeral_ar_numerator[j]) ) {
                label[label_size - 1] = 2;
                break;
            }
        }
        for ( int y = 0; y < ordinal_numeral_en_denominators_max; y ++ ) {
            if ( !strcmp(str[i], ordinal_numeral_en_denominators[y].key) ) {
                label[label_size - 1] = 3;
                break;
            }
        }
    }

    char **str_out = NULL;
    int str_out_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        if ( i < str_size - 1 && !strcmp(str[i], "1") && label[i + 1] == 1 ) {
            for ( int x = 0; x < ordinal_numeral_en_denominator_max; x ++ ) {
                if ( !strcmp(str[i + 1], ordinal_numeral_en_denominator[x].key) ) {
                    int temp_size = 2 + strlen(ordinal_numeral_en_denominator[x].value);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '1';
                    temp[r ++] = '/';
                    for (int t = 0; t < strlen(ordinal_numeral_en_denominator[x].value); t ++) { temp[r ++] = ordinal_numeral_en_denominator[x].value[t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;

                    i += 1;
                    break;
                }
            }
        } else if ( i < str_size - 1 && label[i] == 2 && label[i + 1] == 3 ) {
            for ( int y = 0; y < ordinal_numeral_en_denominators_max; y ++ ) {
                if ( !strcmp(str[i + 1], ordinal_numeral_en_denominators[y].key) ) {
                    int temp_size = strlen(str[i]) + 1 + strlen(ordinal_numeral_en_denominators[y].value);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '/';
                    for (int t = 0; t < strlen(ordinal_numeral_en_denominators[y].value); t ++) { temp[r ++] = ordinal_numeral_en_denominators[y].value[t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;

                    i += 1;
                    break;
                }
            }
        } else if ( i < str_size - 1 && !strcmp(str[i], "1") && !strcmp(str[i + 1], "half") ) {
            int temp_size = 3;
            char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
            int r = 0;
            temp[r ++] = '1';
            temp[r ++] = '/';
            temp[r ++] = '2';
            temp[r] = '\0';

            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
            for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

            free(temp);
            temp = NULL;
            
            i += 1;
        } else if ( i < str_size - 1 && !strcmp(str[i], "1") && !strcmp(str[i + 1], "quarter") ) {
            int temp_size = 3;
            char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
            int r = 0;
            temp[r ++] = '1';
            temp[r ++] = '/';
            temp[r ++] = '4';
            temp[r] = '\0';

            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
            for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

            free(temp);
            temp = NULL;
            
            i += 1;
        } else if ( i < str_size - 1 && !strcmp(str[i], "3") && !strcmp(str[i + 1], "quarters") ) {
            int temp_size = 3;
            char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
            int r = 0;
            temp[r ++] = '3';
            temp[r ++] = '/';
            temp[r ++] = '4';
            temp[r] = '\0';

            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
            for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

            free(temp);
            temp = NULL;
            
            i += 1;
        } else {
            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
            str_out[str_out_size - 1][strlen(str[i])] = '\0';
        }
    }

    free(label);
    label = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **CenturyNotationAndDecadeNotation(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    // non-zero one-digit double-digit number + ...
    STR_STR_Pair cardinal_numeral_plural_en[cardinal_numeral_plural_en_max];

    strcpy(cardinal_numeral_plural_en[0].key, "tens");          strcpy(cardinal_numeral_plural_en[0].value, "10s");
    strcpy(cardinal_numeral_plural_en[1].key, "twenties");      strcpy(cardinal_numeral_plural_en[1].value, "20s");
    strcpy(cardinal_numeral_plural_en[2].key, "thirties");      strcpy(cardinal_numeral_plural_en[2].value, "30s");
    strcpy(cardinal_numeral_plural_en[3].key, "fourties");      strcpy(cardinal_numeral_plural_en[3].value, "40s");
    strcpy(cardinal_numeral_plural_en[4].key, "fifties");       strcpy(cardinal_numeral_plural_en[4].value, "50s");
    strcpy(cardinal_numeral_plural_en[5].key, "sixties");       strcpy(cardinal_numeral_plural_en[5].value, "60s");
    strcpy(cardinal_numeral_plural_en[6].key, "seventies");     strcpy(cardinal_numeral_plural_en[6].value, "70s");
    strcpy(cardinal_numeral_plural_en[7].key, "eighties");      strcpy(cardinal_numeral_plural_en[7].value, "80s");
    strcpy(cardinal_numeral_plural_en[8].key, "nineties");      strcpy(cardinal_numeral_plural_en[8].value, "90s");

    char *cardinal_numeral_singular_ar[cardinal_numeral_singular_ar_max] = {
        "10",
        "20",
        "30",
        "40",
        "50",
        "60",
        "70",
        "80",
        "90"
    };

    long long *label = NULL;
    int label_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;

        for ( int z = 0; z < cardinal_numeral_plural_en_max; z ++ ) {
            if ( !strcmp(str[i], cardinal_numeral_plural_en[z].key) ) {
                strcpy(str[i], cardinal_numeral_plural_en[z].value);
                break;
            }
        }

        if ( isNum(str[i], strlen(str[i])) == 1 ) {
            label[label_size - 1] = strlen(str[i]);
        }
    }

    char **str_out = NULL;
    int str_out_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        long long token = 0;
        long long ticket = 0;

        if ( label[i] == 1 && strcmp(str[i], "0") && i < str_size - 1 ) {
            for ( int z = 0; z < cardinal_numeral_plural_en_max; z ++ ) {
                if ( !strcmp(str[i + 1], cardinal_numeral_plural_en[z].value) ) {
                    int temp_size = strlen(str[i]) + strlen(cardinal_numeral_plural_en[z].value);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    for (int t = 0; t < strlen(cardinal_numeral_plural_en[z].value); t ++) { temp[r ++] = cardinal_numeral_plural_en[z].value[t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    token = 1;
                    break;
                }
            }

            if ( token == 0 ) {
                if ( !strcmp(str[i + 1], "thousands") ) {
                    int temp_size = strlen(str[i]) + strlen("000s");
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '0';
                    temp[r ++] = '0';
                    temp[r ++] = '0';
                    temp[r ++] = 's';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                } else {
                    int temp_size1 = strlen(str[i]);
                    char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                    int r1 = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                    temp1[r1] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                    for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                    free(temp1);
                    temp1 = NULL;

                    int temp_size2 = strlen(str[i + 1]);
                    char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                    int r2 = 0;
                    for (int s = 0; s < strlen(str[i + 1]); s ++) { temp2[r2 ++] = str[i + 1][s]; }
                    temp2[r2] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                    for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                    free(temp2);
                    temp2 = NULL;
                }
            }

            i += 1;
        } else if ( label[i] == 2 && i < str_size - 1 ) {
            for ( int z = 0; z < cardinal_numeral_plural_en_max; z ++ ) {
                if ( !strcmp(str[i + 1], cardinal_numeral_plural_en[z].value) ) {
                    int temp_size = strlen(str[i]) + strlen(cardinal_numeral_plural_en[z].value);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    for (int t = 0; t < strlen(cardinal_numeral_plural_en[z].value); t ++) { temp[r ++] = cardinal_numeral_plural_en[z].value[t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    token = 1;
                    break;
                }
            }

            if ( token == 0 ) {
                for ( int j = 0; j < cardinal_numeral_singular_ar_max; j ++ ) {
                    if ( !strcmp(str[i], cardinal_numeral_singular_ar[j]) ) {
                        ticket = 1;
                        break;
                    }
                }

                if ( ticket == 0 ) {
                    if ( !strcmp(str[i + 1], "hundreds") ) {
                        int temp_size = strlen(str[i]) + strlen("00s");
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                        temp[r ++] = '0';
                        temp[r ++] = '0';
                        temp[r ++] = 's';
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                    } else {
                        int temp_size1 = strlen(str[i]);
                        char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        int r1 = 0;
                        for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                        temp1[r1] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                        free(temp1);
                        temp1 = NULL;

                        int temp_size2 = strlen(str[i + 1]);
                        char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        int r2 = 0;
                        for (int s = 0; s < strlen(str[i + 1]); s ++) { temp2[r2 ++] = str[i + 1][s]; }
                        temp2[r2] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                        free(temp2);
                        temp2 = NULL;
                    }
                } else {
                    int temp_size1 = strlen(str[i]);
                    char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                    int r1 = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                    temp1[r1] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                    for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                    free(temp1);
                    temp1 = NULL;

                    int temp_size2 = strlen(str[i + 1]);
                    char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                    int r2 = 0;
                    for (int s = 0; s < strlen(str[i + 1]); s ++) { temp2[r2 ++] = str[i + 1][s]; }
                    temp2[r2] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                    for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                    free(temp2);
                    temp2 = NULL;
                }
            }

            i += 1;
        } else {
            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
            str_out[str_out_size - 1][strlen(str[i])] = '\0';
        }
    }

    free(label);
    label = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **ClockTimeNotation(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    char *cardinal_numeral_hour_ar[cardinal_numeral_hour_ar_max] = {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"
    };

    char *cardinal_numeral_minute_ar[cardinal_numeral_minute_ar_max] = {
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", 
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", 
        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", 
        "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", 
        "50", "51", "52", "53", "54", "55", "56", "57", "58", "59"
    };

    long long *label = NULL;
    int label_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;

        if ( isNum(str[i], strlen(str[i])) == 1 ) {
            label[label_size - 1] = strlen(str[i]);
        }
    }

    char **str_out = NULL;
    int str_out_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        long long marker = 0;
        long long sentinel = 0;

        for ( int j = 0; j < cardinal_numeral_hour_ar_max; j ++ ) {
            if ( !strcmp(str[i], cardinal_numeral_hour_ar[j]) ) {
                marker = 1;
                break;
            }
        }
        for ( int j = 0; j < cardinal_numeral_minute_ar_max; j ++ ) {
            if ( i < str_size - 1 && !strcmp(str[i + 1], cardinal_numeral_minute_ar[j]) ) {
                sentinel = 1;
                break;
            }
        }

        if ( marker == 1 ) {
            if ( i < str_size - 1 && ( !strcmp(str[i + 1], "o") || !strcmp(str[i + 1], "0") ) ) {
                if ( i < str_size - 2 && label[i + 2] == 1 && strcmp(str[i + 2], "0") ) {
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "a") ) {
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "m") ) {
                            int temp_size1 = strlen(str[i]) + 2 + strlen(str[i + 2]);
                            char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                            int r1 = 0;
                            for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                            temp1[r1 ++] = ':';
                            temp1[r1 ++] = '0';
                            for (int t = 0; t < strlen(str[i + 2]); t ++) { temp1[r1 ++] = str[i + 2][t]; }
                            temp1[r1] = '\0';

                            str_out_size += 1;
                            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                            for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                            free(temp1);
                            temp1 = NULL;

                            int temp_size2 = strlen("a.m.");
                            char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                            int r2 = 0;
                            temp2[r2 ++] = 'a';
                            temp2[r2 ++] = '.';
                            temp2[r2 ++] = 'm';
                            temp2[r2 ++] = '.';
                            temp2[r2] = '\0';

                            str_out_size += 1;
                            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                            for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                            free(temp2);
                            temp2 = NULL;
                            
                            i += 4;
                        } else {
                            str_out_size += 1;
                            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                            str_out[str_out_size - 1][strlen(str[i])] = '\0';
                        }
                    } else if ( i < str_size - 3 && !strcmp(str[i + 3], "p") ) {
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "m") ) {
                            int temp_size1 = strlen(str[i]) + 2 + strlen(str[i + 2]);
                            char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                            int r1 = 0;
                            for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                            temp1[r1 ++] = ':';
                            temp1[r1 ++] = '0';
                            for (int t = 0; t < strlen(str[i + 2]); t ++) { temp1[r1 ++] = str[i + 2][t]; }
                            temp1[r1] = '\0';

                            str_out_size += 1;
                            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                            for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                            free(temp1);
                            temp1 = NULL;

                            int temp_size2 = strlen("p.m.");
                            char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                            int r2 = 0;
                            temp2[r2 ++] = 'p';
                            temp2[r2 ++] = '.';
                            temp2[r2 ++] = 'm';
                            temp2[r2 ++] = '.';
                            temp2[r2] = '\0';

                            str_out_size += 1;
                            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                            for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                            free(temp2);
                            temp2 = NULL;
                            
                            i += 4;
                        } else {
                            str_out_size += 1;
                            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                            str_out[str_out_size - 1][strlen(str[i])] = '\0';
                        }
                    } else if ( i < str_size - 3 && !strcmp(str[i + 3], "am") ) {
                        int temp_size1 = strlen(str[i]) + 2 + strlen(str[i + 2]);
                        char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        int r1 = 0;
                        for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                        temp1[r1 ++] = ':';
                        temp1[r1 ++] = '0';
                        for (int t = 0; t < strlen(str[i + 2]); t ++) { temp1[r1 ++] = str[i + 2][t]; }
                        temp1[r1] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                        free(temp1);
                        temp1 = NULL;

                        int temp_size2 = strlen("a.m.");
                        char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        int r2 = 0;
                        temp2[r2 ++] = 'a';
                        temp2[r2 ++] = '.';
                        temp2[r2 ++] = 'm';
                        temp2[r2 ++] = '.';
                        temp2[r2] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                        free(temp2);
                        temp2 = NULL;
                        
                        i += 3;
                    } else if ( i < str_size - 3 && !strcmp(str[i + 3], "pm") ) {
                        int temp_size1 = strlen(str[i]) + 2 + strlen(str[i + 2]);
                        char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        int r1 = 0;
                        for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                        temp1[r1 ++] = ':';
                        temp1[r1 ++] = '0';
                        for (int t = 0; t < strlen(str[i + 2]); t ++) { temp1[r1 ++] = str[i + 2][t]; }
                        temp1[r1] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                        free(temp1);
                        temp1 = NULL;

                        int temp_size2 = strlen("p.m.");
                        char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        int r2 = 0;
                        temp2[r2 ++] = 'p';
                        temp2[r2 ++] = '.';
                        temp2[r2 ++] = 'm';
                        temp2[r2 ++] = '.';
                        temp2[r2] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                        free(temp2);
                        temp2 = NULL;
                        
                        i += 3;
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                        str_out[str_out_size - 1][strlen(str[i])] = '\0';
                    }
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else if ( sentinel == 1 ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "a") ) {
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "m") ) {
                        int temp_size1 = strlen(str[i]) + 1 + strlen(str[i + 1]);
                        char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        int r1 = 0;
                        for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                        temp1[r1 ++] = ':';
                        for (int t = 0; t < strlen(str[i + 1]); t ++) { temp1[r1 ++] = str[i + 1][t]; }
                        temp1[r1] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                        free(temp1);
                        temp1 = NULL;

                        int temp_size2 = strlen("a.m.");
                        char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        int r2 = 0;
                        temp2[r2 ++] = 'a';
                        temp2[r2 ++] = '.';
                        temp2[r2 ++] = 'm';
                        temp2[r2 ++] = '.';
                        temp2[r2] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                        free(temp2);
                        temp2 = NULL;
                        
                        i += 3;
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                        str_out[str_out_size - 1][strlen(str[i])] = '\0';
                    }
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "p") ) {
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "m") ) {
                        int temp_size1 = strlen(str[i]) + 1 + strlen(str[i + 1]);
                        char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        int r1 = 0;
                        for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                        temp1[r1 ++] = ':';
                        for (int t = 0; t < strlen(str[i + 1]); t ++) { temp1[r1 ++] = str[i + 1][t]; }
                        temp1[r1] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                        free(temp1);
                        temp1 = NULL;

                        int temp_size2 = strlen("p.m.");
                        char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        int r2 = 0;
                        temp2[r2 ++] = 'p';
                        temp2[r2 ++] = '.';
                        temp2[r2 ++] = 'm';
                        temp2[r2 ++] = '.';
                        temp2[r2] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                        free(temp2);
                        temp2 = NULL;
                        
                        i += 3;
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                        str_out[str_out_size - 1][strlen(str[i])] = '\0';
                    }
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "am") ) {
                    int temp_size1 = strlen(str[i]) + 1 + strlen(str[i + 1]);
                    char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                    int r1 = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                    temp1[r1 ++] = ':';
                    for (int t = 0; t < strlen(str[i + 1]); t ++) { temp1[r1 ++] = str[i + 1][t]; }
                    temp1[r1] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                    for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                    free(temp1);
                    temp1 = NULL;

                    int temp_size2 = strlen("a.m.");
                    char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                    int r2 = 0;
                    temp2[r2 ++] = 'a';
                    temp2[r2 ++] = '.';
                    temp2[r2 ++] = 'm';
                    temp2[r2 ++] = '.';
                    temp2[r2] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                    for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                    free(temp2);
                    temp2 = NULL;
                    
                    i += 2;
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "pm") ) {
                    int temp_size1 = strlen(str[i]) + 1 + strlen(str[i + 1]);
                    char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                    int r1 = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                    temp1[r1 ++] = ':';
                    for (int t = 0; t < strlen(str[i + 1]); t ++) { temp1[r1 ++] = str[i + 1][t]; }
                    temp1[r1] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                    for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                    free(temp1);
                    temp1 = NULL;

                    int temp_size2 = strlen("p.m.");
                    char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                    int r2 = 0;
                    temp2[r2 ++] = 'p';
                    temp2[r2 ++] = '.';
                    temp2[r2 ++] = 'm';
                    temp2[r2 ++] = '.';
                    temp2[r2] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                    for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                    free(temp2);
                    temp2 = NULL;
                    
                    i += 2;
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else {
            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
            str_out[str_out_size - 1][strlen(str[i])] = '\0';
        }
    }

    free(label);
    label = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **CurrencyNotation(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    char *pound_m = "£";
    char *euro_m = "€";

    long long *label = NULL;
    int label_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;

        if ( isNum(str[i], strlen(str[i])) == 1 && strcmp(str[i], "0") && strcmp(str[i], "1") ) {
            label[label_size - 1] = strlen(str[i]);
        }
    }

    char **str_out = NULL;
    int str_out_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        if ( !strcmp(str[i], "1") ) {
            if ( i < str_size - 1 && !strcmp(str[i + 1], "pound") ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "and") ) {
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "1") ) {
                        int temp_size = 6;
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                        temp[r ++] = '1';
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        temp[r ++] = '1';
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "penny") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 1 ) {
                        int temp_size = 5 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                        temp[r ++] = '1';
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "pence") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 2 ) {
                        int temp_size = 4 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                        temp[r ++] = '1';
                        temp[r ++] = '.';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "pence") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                        str_out[str_out_size - 1][strlen(str[i])] = '\0';
                    }
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "1") ) {
                    int temp_size = 6;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                    temp[r ++] = '1';
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    temp[r ++] = '1';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "penny") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 1 ) {
                    int temp_size = 5 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                    temp[r ++] = '1';
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "pence") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 2 ) {
                    int temp_size = 4 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                    temp[r ++] = '1';
                    temp[r ++] = '.';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "pence") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "euro") ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "and") ) {
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "1") ) {
                        int temp_size = 7;
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        temp[r ++] = '1';
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        temp[r ++] = '1';
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cent") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 1 ) {
                        int temp_size = 6 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        temp[r ++] = '1';
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cents") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 2 ) {
                        int temp_size = 5 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        temp[r ++] = '1';
                        temp[r ++] = '.';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cents") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else {
                        int temp_size = 4;
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        temp[r ++] = '1';
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        i += 1;
                    }
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "1") ) {
                    int temp_size = 7;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                    temp[r ++] = '1';
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    temp[r ++] = '1';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cent") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 1 ) {
                    int temp_size = 6 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                    temp[r ++] = '1';
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cents") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 2 ) {
                    int temp_size = 5 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                    temp[r ++] = '1';
                    temp[r ++] = '.';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cents") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "cent") ) {
                    int temp_size = 7;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                    temp[r ++] = '0';
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    temp[r ++] = '1';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 2;
                } else {
                    int temp_size = 4;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                    temp[r ++] = '1';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 1;
                }
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "dollar") ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "and") ) {
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "1") ) {
                        int temp_size = 5;
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        temp[r ++] = '$';
                        temp[r ++] = '1';
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        temp[r ++] = '1';
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cent") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 1 ) {
                        int temp_size = 4 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        temp[r ++] = '$';
                        temp[r ++] = '1';
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cents") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 2 ) {
                        int temp_size = 3 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        temp[r ++] = '$';
                        temp[r ++] = '1';
                        temp[r ++] = '.';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cents") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else {
                        int temp_size = 2;
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        temp[r ++] = '$';
                        temp[r ++] = '1';
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        i += 1;
                    }
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "1") ) {
                    int temp_size = 5;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    temp[r ++] = '1';
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    temp[r ++] = '1';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cent") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 1 ) {
                    int temp_size = 4 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    temp[r ++] = '1';
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cents") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 2 ) {
                    int temp_size = 3 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    temp[r ++] = '1';
                    temp[r ++] = '.';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cents") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else {
                    int temp_size = 2;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    temp[r ++] = '1';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 1;
                }
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "penny") ) {
                int temp_size = 6;
                char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                int r = 0;
                for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                temp[r ++] = '0';
                temp[r ++] = '.';
                temp[r ++] = '0';
                temp[r ++] = '1';
                temp[r] = '\0';

                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                free(temp);
                temp = NULL;
                
                i += 1;
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "cent") ) {
                int temp_size = 5;
                char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                int r = 0;
                temp[r ++] = '$';
                temp[r ++] = '0';
                temp[r ++] = '.';
                temp[r ++] = '0';
                temp[r ++] = '1';
                temp[r] = '\0';

                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                free(temp);
                temp = NULL;
                
                i += 1;
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else if ( label[i] != 0 ) {
            if ( i < str_size - 1 && !strcmp(str[i + 1], "pounds") ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "and") ) {
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "1") ) {
                        int temp_size = 2 + strlen(str[i]) + 3;
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                        for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        temp[r ++] = '1';
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "penny") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 1 ) {
                        int temp_size = 2 + strlen(str[i]) + 2 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                        for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "pence") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 2 ) {
                        int temp_size = 2 + strlen(str[i]) + 1 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                        for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                        temp[r ++] = '.';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "pence") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                        str_out[str_out_size - 1][strlen(str[i])] = '\0';
                    }
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "1") ) {
                    int temp_size = 2 + strlen(str[i]) + 3;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                    for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    temp[r ++] = '1';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "penny") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 1 ) {
                    int temp_size = 2 + strlen(str[i]) + 2 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "pence") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 2 ) {
                    int temp_size = 2 + strlen(str[i]) + 1 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '.';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "pence") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "euros") ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "and") ) {
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "1") ) {
                        int temp_size = 3 + strlen(str[i]) + 3;
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        temp[r ++] = '1';
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cent") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 1 ) {
                        int temp_size = 3 + strlen(str[i]) + 2 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cents") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 2 ) {
                        int temp_size = 3 + strlen(str[i]) + 1 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                        temp[r ++] = '.';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cents") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else {
                        int temp_size = 3 + strlen(str[i]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        i += 1;
                    }
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "1") ) {
                    int temp_size = 3 + strlen(str[i]) + 3;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                    for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    temp[r ++] = '1';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cent") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 1 ) {
                    int temp_size = 3 + strlen(str[i]) + 2 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cents") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 2 ) {
                    int temp_size = 3 + strlen(str[i]) + 1 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '.';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cents") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else {
                    int temp_size = 3 + strlen(str[i]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                    for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 1;
                }
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "dollars") ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "and") ) {
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "1") ) {
                        int temp_size = 1 + strlen(str[i]) + 3;
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        temp[r ++] = '$';
                        for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        temp[r ++] = '1';
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cent") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 1 ) {
                        int temp_size = 1 + strlen(str[i]) + 2 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        temp[r ++] = '$';
                        for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cents") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else if ( i < str_size - 3 && label[i + 3] == 2 ) {
                        int temp_size = 1 + strlen(str[i]) + 1 + strlen(str[i + 3]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        temp[r ++] = '$';
                        for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                        temp[r ++] = '.';
                        for (int t = 0; t < strlen(str[i + 3]); t ++) { temp[r ++] = str[i + 3][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        if ( i < str_size - 4 && !strcmp(str[i + 4], "cents") ) {
                            i += 4;
                        } else {
                            i += 3;
                        }
                    } else {
                        int temp_size = 1 + strlen(str[i]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        temp[r ++] = '$';
                        for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        i += 1;
                    }
                } else if ( i < str_size - 2 && !strcmp(str[i + 2], "1") ) {
                    int temp_size = 1 + strlen(str[i]) + 3;
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    temp[r ++] = '1';
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cent") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 1 ) {
                    int temp_size = 1 + strlen(str[i]) + 2 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cents") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else if ( i < str_size - 2 && label[i + 2] == 2 ) {
                    int temp_size = 1 + strlen(str[i]) + 1 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '.';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    if ( i < str_size - 3 && !strcmp(str[i + 3], "cents") ) {
                        i += 3;
                    } else {
                        i += 2;
                    }
                } else {
                    int temp_size = 1 + strlen(str[i]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 1;
                }
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "pence") ) {
                if ( label[i] == 1 ) {
                    int temp_size = 5 + strlen(str[i]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                    temp[r ++] = '0';
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 1;
                } else if ( label[i] == 2 ) {
                    int temp_size = 4 + strlen(str[i]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int w = 0; w < strlen(pound_m); w ++) { temp[r ++] = pound_m[w]; }
                    temp[r ++] = '0';
                    temp[r ++] = '.';
                    for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 1;
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "euro") ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "cents") ) {
                    if ( label[i] == 1 ) {
                        int temp_size = 6 + strlen(str[i]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        temp[r ++] = '0';
                        temp[r ++] = '.';
                        temp[r ++] = '0';
                        for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        i += 2;
                    } else if ( label[i] == 2 ) {
                        int temp_size = 5 + strlen(str[i]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int w = 0; w < strlen(euro_m); w ++) { temp[r ++] = euro_m[w]; }
                        temp[r ++] = '0';
                        temp[r ++] = '.';
                        for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        i += 2;
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                        str_out[str_out_size - 1][strlen(str[i])] = '\0';
                    }
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else if ( i < str_size - 1 && !strcmp(str[i + 1], "cents") ) {
                if ( label[i] == 1 ) {
                    int temp_size = 4 + strlen(str[i]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    temp[r ++] = '0';
                    temp[r ++] = '.';
                    temp[r ++] = '0';
                    for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 1;
                } else if ( label[i] == 2 ) {
                    int temp_size = 3 + strlen(str[i]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    temp[r ++] = '$';
                    temp[r ++] = '0';
                    temp[r ++] = '.';
                    for (int t = 0; t < strlen(str[i]); t ++) { temp[r ++] = str[i][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 1;
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else {
            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
            str_out[str_out_size - 1][strlen(str[i])] = '\0';
        }
    }

    free(label);
    label = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **YearNotation(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    long long *label = NULL;
    int label_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;

        if ( isNum(str[i], strlen(str[i])) == 1 && strcmp(str[i], "0") ) {
            label[label_size - 1] = strlen(str[i]);
        }
    }

    char **str_out = NULL;
    int str_out_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        if ( label[i] == 1 ) {
            if ( i < str_size - 1 && label[i + 1] == 2 ) {
                int temp_size = strlen(str[i]) + strlen(str[i + 1]);
                char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                int r = 0;
                for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                for (int t = 0; t < strlen(str[i + 1]); t ++) { temp[r ++] = str[i + 1][t]; }
                temp[r] = '\0';

                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                free(temp);
                temp = NULL;
                
                i += 1;
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else if ( label[i] == 2 ) {
            if ( i < str_size - 1 && ( !strcmp(str[i + 1], "0") || !strcmp(str[i + 1], "o") ) ) {
                if ( i < str_size - 2 && label[i + 2] == 1 ) {
                    int temp_size = strlen(str[i]) + 1 + strlen(str[i + 2]);
                    char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                    int r = 0;
                    for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                    temp[r ++] = '0';
                    for (int t = 0; t < strlen(str[i + 2]); t ++) { temp[r ++] = str[i + 2][t]; }
                    temp[r] = '\0';

                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                    for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                    free(temp);
                    temp = NULL;
                    
                    i += 2;
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else if ( i < str_size - 1 && label[i + 1] == 2 ) {
                int temp_size = strlen(str[i]) + strlen(str[i + 1]);
                char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                int r = 0;
                for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                for (int t = 0; t < strlen(str[i + 1]); t ++) { temp[r ++] = str[i + 1][t]; }
                temp[r] = '\0';

                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                free(temp);
                temp = NULL;
                
                i += 1;
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else {
            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
            str_out[str_out_size - 1][strlen(str[i])] = '\0';
        }
    }

    free(label);
    label = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **MonthDayNotation(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    STR_STR_Pair month_name_en_abbr[month_name_en_abbr_max];

    strcpy(month_name_en_abbr[0].key, "January");           strcpy(month_name_en_abbr[0].value, "Jan.");
    strcpy(month_name_en_abbr[1].key, "February");          strcpy(month_name_en_abbr[1].value, "Feb.");
    strcpy(month_name_en_abbr[2].key, "March");             strcpy(month_name_en_abbr[2].value, "Mar.");
    strcpy(month_name_en_abbr[3].key, "April");             strcpy(month_name_en_abbr[3].value, "Apr.");
    strcpy(month_name_en_abbr[4].key, "May");               strcpy(month_name_en_abbr[4].value, "May");
    strcpy(month_name_en_abbr[5].key, "June");              strcpy(month_name_en_abbr[5].value, "June");
    strcpy(month_name_en_abbr[6].key, "July");              strcpy(month_name_en_abbr[6].value, "July");
    strcpy(month_name_en_abbr[7].key, "August");            strcpy(month_name_en_abbr[7].value, "Aug.");
    strcpy(month_name_en_abbr[8].key, "September");         strcpy(month_name_en_abbr[8].value, "Sept.");
    strcpy(month_name_en_abbr[9].key, "October");           strcpy(month_name_en_abbr[9].value, "Oct.");
    strcpy(month_name_en_abbr[10].key, "November");         strcpy(month_name_en_abbr[10].value, "Nov.");
    strcpy(month_name_en_abbr[11].key, "December");         strcpy(month_name_en_abbr[11].value, "Dec.");

    STR_STR_Pair ordinal_numeral_en_day[ordinal_numeral_en_day_max];

    strcpy(ordinal_numeral_en_day[0].key, "first");         strcpy(ordinal_numeral_en_day[0].value, "1");
    strcpy(ordinal_numeral_en_day[1].key, "second");        strcpy(ordinal_numeral_en_day[1].value, "2");
    strcpy(ordinal_numeral_en_day[2].key, "third");         strcpy(ordinal_numeral_en_day[2].value, "3");
    strcpy(ordinal_numeral_en_day[3].key, "fourth");        strcpy(ordinal_numeral_en_day[3].value, "4");
    strcpy(ordinal_numeral_en_day[4].key, "fifth");         strcpy(ordinal_numeral_en_day[4].value, "5");
    strcpy(ordinal_numeral_en_day[5].key, "sixth");         strcpy(ordinal_numeral_en_day[5].value, "6");
    strcpy(ordinal_numeral_en_day[6].key, "seventh");       strcpy(ordinal_numeral_en_day[6].value, "7");
    strcpy(ordinal_numeral_en_day[7].key, "eighth");        strcpy(ordinal_numeral_en_day[7].value, "8");
    strcpy(ordinal_numeral_en_day[8].key, "ninth");         strcpy(ordinal_numeral_en_day[8].value, "9");
    strcpy(ordinal_numeral_en_day[9].key, "10th");          strcpy(ordinal_numeral_en_day[9].value, "10");
    strcpy(ordinal_numeral_en_day[10].key, "11th");         strcpy(ordinal_numeral_en_day[10].value, "11");
    strcpy(ordinal_numeral_en_day[11].key, "12th");         strcpy(ordinal_numeral_en_day[11].value, "12");
    strcpy(ordinal_numeral_en_day[12].key, "13th");         strcpy(ordinal_numeral_en_day[12].value, "13");
    strcpy(ordinal_numeral_en_day[13].key, "14th");         strcpy(ordinal_numeral_en_day[13].value, "14");
    strcpy(ordinal_numeral_en_day[14].key, "15th");         strcpy(ordinal_numeral_en_day[14].value, "15");
    strcpy(ordinal_numeral_en_day[15].key, "16th");         strcpy(ordinal_numeral_en_day[15].value, "16");
    strcpy(ordinal_numeral_en_day[16].key, "17th");         strcpy(ordinal_numeral_en_day[16].value, "17");
    strcpy(ordinal_numeral_en_day[17].key, "18th");         strcpy(ordinal_numeral_en_day[17].value, "18");
    strcpy(ordinal_numeral_en_day[18].key, "19th");         strcpy(ordinal_numeral_en_day[18].value, "19");
    strcpy(ordinal_numeral_en_day[19].key, "20th");         strcpy(ordinal_numeral_en_day[19].value, "20");
    strcpy(ordinal_numeral_en_day[20].key, "21st");         strcpy(ordinal_numeral_en_day[20].value, "21");
    strcpy(ordinal_numeral_en_day[21].key, "22nd");         strcpy(ordinal_numeral_en_day[21].value, "22");
    strcpy(ordinal_numeral_en_day[22].key, "23rd");         strcpy(ordinal_numeral_en_day[22].value, "23");
    strcpy(ordinal_numeral_en_day[23].key, "24th");         strcpy(ordinal_numeral_en_day[23].value, "24");
    strcpy(ordinal_numeral_en_day[24].key, "25th");         strcpy(ordinal_numeral_en_day[24].value, "25");
    strcpy(ordinal_numeral_en_day[25].key, "26th");         strcpy(ordinal_numeral_en_day[25].value, "26");
    strcpy(ordinal_numeral_en_day[26].key, "27th");         strcpy(ordinal_numeral_en_day[26].value, "27");
    strcpy(ordinal_numeral_en_day[27].key, "28th");         strcpy(ordinal_numeral_en_day[27].value, "28");
    strcpy(ordinal_numeral_en_day[28].key, "29th");         strcpy(ordinal_numeral_en_day[28].value, "29");
    strcpy(ordinal_numeral_en_day[29].key, "30th");         strcpy(ordinal_numeral_en_day[29].value, "30");
    strcpy(ordinal_numeral_en_day[30].key, "31st");         strcpy(ordinal_numeral_en_day[30].value, "31");

    long long *label = NULL;
    int label_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;

        for ( int x = 0; x < month_name_en_abbr_max; x ++ ) {
            if ( !strcmp(str[i], month_name_en_abbr[x].key) ) {
                label[label_size - 1] = 1;
                break;
            }
        }

        for ( int y = 0; y < ordinal_numeral_en_day_max; y ++ ) {
            if ( !strcmp(str[i], ordinal_numeral_en_day[y].key) ) {
                label[label_size - 1] = 2;
                break;
            }
        }
    }

    char **str_out = NULL;
    int str_out_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        if ( !strcmp(str[i], "the") ) {
            if ( i < str_size - 1 && label[i + 1] == 2 ) {
                if ( i < str_size - 2 && !strcmp(str[i + 2], "of") ) {
                    if ( i < str_size - 3 && label[i + 3] == 1 ) {
                        for ( int y = 0; y < ordinal_numeral_en_day_max; y ++ ) {
                            if ( !strcmp(str[i + 1], ordinal_numeral_en_day[y].key) ) {
                                strcpy(str[i + 1], ordinal_numeral_en_day[y].value);
                                break;
                            }
                        }
                        for ( int x = 0; x < month_name_en_abbr_max; x ++ ) {
                            if ( !strcmp(str[i + 3], month_name_en_abbr[x].key) ) {
                                strcpy(str[i + 3], month_name_en_abbr[x].value);
                                break;
                            }
                        }

                        int temp_size1 = strlen(str[i + 3]);
                        char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        int r1 = 0;
                        for (int s = 0; s < strlen(str[i + 3]); s ++) { temp1[r1 ++] = str[i + 3][s]; }
                        temp1[r1] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                        for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                        free(temp1);
                        temp1 = NULL;

                        int temp_size2 = strlen(str[i + 1]);
                        char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        int r2 = 0;
                        for (int s = 0; s < strlen(str[i + 1]); s ++) { temp2[r2 ++] = str[i + 1][s]; }
                        temp2[r2] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                        for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                        free(temp2);
                        temp2 = NULL;
                        
                        i += 3;
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                        str_out[str_out_size - 1][strlen(str[i])] = '\0';
                    }
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else if ( label[i] == 1 ) {
            if ( i < str_size - 1 && label[i + 1] == 2 ) {
                for ( int x = 0; x < month_name_en_abbr_max; x ++ ) {
                    if ( !strcmp(str[i], month_name_en_abbr[x].key) ) {
                        strcpy(str[i], month_name_en_abbr[x].value);
                        break;
                    }
                }
                for ( int y = 0; y < ordinal_numeral_en_day_max; y ++ ) {
                    if ( !strcmp(str[i + 1], ordinal_numeral_en_day[y].key) ) {
                        strcpy(str[i + 1], ordinal_numeral_en_day[y].value);
                        break;
                    }
                }

                int temp_size1 = strlen(str[i]);
                char *temp1 = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                int r1 = 0;
                for (int s = 0; s < strlen(str[i]); s ++) { temp1[r1 ++] = str[i][s]; }
                temp1[r1] = '\0';

                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size1 + 1));
                for (int r = 0; r < temp_size1 + 1; r ++) { str_out[str_out_size - 1][r] = temp1[r]; }

                free(temp1);
                temp1 = NULL;

                int temp_size2 = strlen(str[i + 1]);
                char *temp2 = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                int r2 = 0;
                for (int s = 0; s < strlen(str[i + 1]); s ++) { temp2[r2 ++] = str[i + 1][s]; }
                temp2[r2] = '\0';

                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size2 + 1));
                for (int r = 0; r < temp_size2 + 1; r ++) { str_out[str_out_size - 1][r] = temp2[r]; }

                free(temp2);
                temp2 = NULL;
                
                i += 1;
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else {
            str_out_size += 1;
            str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
            str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
            str_out[str_out_size - 1][strlen(str[i])] = '\0';
        }
    }

    free(label);
    label = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **ContinuousOneDigitNum(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    char *cardinal_numeral_ar_s[cardinal_numeral_ar_s_max] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
    };

    long long *label = NULL;
    int label_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        label_size += 1;
        label = (long long *)realloc(label, sizeof(long long) * label_size);
        label[label_size - 1] = 0;

        for ( int j = 0; j < cardinal_numeral_ar_s_max; j ++ ) {
            if ( !strcmp(str[i], cardinal_numeral_ar_s[j]) || !strcmp(str[i], "o") ) {
                label[label_size - 1] = 1;
                break;
            }
        }
    }

    char **str_out = NULL;
    int str_out_size = 0;
    long long transform = 1;
    for ( int i = 0; i < str_size; i ++ ) {
        if ( transform == 1 ) {
            if ( label[i] == 1 ) {
                if ( i < str_size - 1 && label[i + 1] == 1 ) {
                    if ( i < str_size - 2 && label[i + 2] == 1 ) {
                        if ( !strcmp(str[i], "o") ) strcpy(str[i], "0");
                        if ( !strcmp(str[i + 1], "o") ) strcpy(str[i + 1], "0");
                        if ( !strcmp(str[i + 2], "o") ) strcpy(str[i + 2], "0");

                        int temp_size = strlen(str[i]) + strlen(str[i + 1]) + strlen(str[i + 2]);
                        char *temp = (char *)malloc(sizeof(char) * (temp_size + 1));
                        int r = 0;
                        for (int s = 0; s < strlen(str[i]); s ++) { temp[r ++] = str[i][s]; }
                        for (int t = 0; t < strlen(str[i + 1]); t ++) { temp[r ++] = str[i + 1][t]; }
                        for (int w = 0; w < strlen(str[i + 2]); w ++) { temp[r ++] = str[i + 2][w]; }
                        temp[r] = '\0';

                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (temp_size + 1));
                        for (int r = 0; r < temp_size + 1; r ++) { str_out[str_out_size - 1][r] = temp[r]; }

                        free(temp);
                        temp = NULL;
                        
                        transform = 0;
                        if ( i == str_size - 3 || ( i < str_size - 3 && label[i + 3] != 1 ) ) { transform = 1; }
                        i += 2;
                    } else {
                        str_out_size += 1;
                        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                        str_out[str_out_size - 1][strlen(str[i])] = '\0';
                    }
                } else {
                    str_out_size += 1;
                    str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                    str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                    for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                    str_out[str_out_size - 1][strlen(str[i])] = '\0';
                }
            } else {
                str_out_size += 1;
                str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
                str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
                for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
                str_out[str_out_size - 1][strlen(str[i])] = '\0';
            }
        } else {
            if ( !strcmp(str[i], "o") ) strcpy(str[i], "0");
            int cur_str_out_index_size = strlen(str_out[str_out_size - 1]);
            str_out[str_out_size - 1] = (char *)realloc(str_out[str_out_size - 1], sizeof(char) * (cur_str_out_index_size + strlen(str[i]) + 1));
            for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][cur_str_out_index_size + r] = str[i][r]; }
            str_out[str_out_size - 1][cur_str_out_index_size + strlen(str[i])] = '\0';

            if ( i == str_size - 1 || ( i < str_size - 1 && label[i + 1] != 1 ) ) { transform = 1; }
        }
    }

    free(label);
    label = NULL;

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **OneDigitNumTextNormalization(char **src_str, int str_size, int *res_size) {
    char **str = (char **)malloc(sizeof(char *) * str_size);
    for ( int i = 0; i < str_size; i ++ ) {
        str[i] = (char *)malloc(sizeof(char) * (strlen(src_str[i]) + 1));
    }

    for ( int i = 0; i < str_size; i ++ ) {
        for ( int j = 0; j < strlen(src_str[i]); j ++ ) {
            str[i][j] = src_str[i][j];
        }
        str[i][strlen(src_str[i])] = '\0';
    }

    char *month_name_abbr_en[month_name_abbr_en_max] = {
        "Jan.", "Feb.", "Mar.", "Apr.", "May", "June", "July", "Aug.", "Sept.", "Oct.", "Nov.", "Dec."
    };

    STR_STR_Pair cardinal_numeral_ar_en[cardinal_numeral_ar_en_max];

    strcpy(cardinal_numeral_ar_en[0].key, "0");         strcpy(cardinal_numeral_ar_en[0].value, "zero");
    strcpy(cardinal_numeral_ar_en[1].key, "1");         strcpy(cardinal_numeral_ar_en[1].value, "one");
    strcpy(cardinal_numeral_ar_en[2].key, "2");         strcpy(cardinal_numeral_ar_en[2].value, "two");
    strcpy(cardinal_numeral_ar_en[3].key, "3");         strcpy(cardinal_numeral_ar_en[3].value, "three");
    strcpy(cardinal_numeral_ar_en[4].key, "4");         strcpy(cardinal_numeral_ar_en[4].value, "four");
    strcpy(cardinal_numeral_ar_en[5].key, "5");         strcpy(cardinal_numeral_ar_en[5].value, "five");
    strcpy(cardinal_numeral_ar_en[6].key, "6");         strcpy(cardinal_numeral_ar_en[6].value, "six");
    strcpy(cardinal_numeral_ar_en[7].key, "7");         strcpy(cardinal_numeral_ar_en[7].value, "seven");
    strcpy(cardinal_numeral_ar_en[8].key, "8");         strcpy(cardinal_numeral_ar_en[8].value, "eight");
    strcpy(cardinal_numeral_ar_en[9].key, "9");         strcpy(cardinal_numeral_ar_en[9].value, "nine");

    char **str_out = NULL;
    int str_out_size = 0;
    for ( int i = 0; i < str_size; i ++ ) {
        long long piquet = 1;
        
        if ( i > 0 ) {
            for ( int j = 0; j < month_name_abbr_en_max; j ++ ) {
                if ( !strcmp(str[i - 1], month_name_abbr_en[j]) ) {
                    piquet = 0;
                    break;
                }
            }
        }

        if ( piquet == 1 ) {
            for ( int z = 0; z < cardinal_numeral_ar_en_max; z ++ ) {
                if ( !strcmp(str[i], cardinal_numeral_ar_en[z].key) ) {
                    strcpy(str[i], cardinal_numeral_ar_en[z].value);
                    break;
                }
            }
        }

        str_out_size += 1;
        str_out = (char **)realloc(str_out, sizeof(char *) * str_out_size);
        str_out[str_out_size - 1] = (char *)malloc(sizeof(char) * (strlen(str[i]) + 1));
        for (int r = 0; r < strlen(str[i]); r ++) { str_out[str_out_size - 1][r] = str[i][r]; }
        str_out[str_out_size - 1][strlen(str[i])] = '\0';
    }

    for ( int i = 0; i < str_size; i ++ ) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;

    res_size[0] = str_out_size;

    return str_out;
}


char **InverseTextNormalization(char **src_str, int str_size, int *res_size) {
    int *res_size1 = (int *)malloc(sizeof(int));
    char **str_out1 = CardinalNumAndOrdinalNum(src_str, str_size, res_size1);

    int *res_size2 = (int *)malloc(sizeof(int));
    char **str_out2 = DecimalNum(str_out1, res_size1[0], res_size2);

    int *res_size3 = (int *)malloc(sizeof(int));
    char **str_out3 = PercentageNum(str_out2, res_size2[0], res_size3);

    int *res_size4 = (int *)malloc(sizeof(int));
    char **str_out4 = FractionNum(str_out3, res_size3[0], res_size4);

    int *res_size5 = (int *)malloc(sizeof(int));
    char **str_out5 = CenturyNotationAndDecadeNotation(str_out4, res_size4[0], res_size5);

    int *res_size6 = (int *)malloc(sizeof(int));
    char **str_out6 = ClockTimeNotation(str_out5, res_size5[0], res_size6);

    int *res_size7 = (int *)malloc(sizeof(int));
    char **str_out7 = CurrencyNotation(str_out6, res_size6[0], res_size7);

    int *res_size8 = (int *)malloc(sizeof(int));
    char **str_out8 = YearNotation(str_out7, res_size7[0], res_size8);

    int *res_size9 = (int *)malloc(sizeof(int));
    char **str_out9 = MonthDayNotation(str_out8, res_size8[0], res_size9);

    int *res_size10 = (int *)malloc(sizeof(int));
    char **str_out10 = ContinuousOneDigitNum(str_out9, res_size9[0], res_size10);

    int *res_size11 = (int *)malloc(sizeof(int));
    char **str_out11 = OneDigitNumTextNormalization(str_out10, res_size10[0], res_size11);

    res_size[0] = res_size11[0];
    char **str_out = (char **)malloc(sizeof(char *) * res_size[0]);
    for ( int i = 0; i < res_size[0]; i ++ ) {
        str_out[i] = (char *)malloc(sizeof(char) * (strlen(str_out11[i]) + 1));
    }

    for ( int i = 0; i < res_size[0]; i ++ ) {
        for ( int j = 0; j < strlen(str_out11[i]); j ++ ) {
            str_out[i][j] = str_out11[i][j];
        }
        str_out[i][strlen(str_out11[i])] = '\0';
    }

    for (int i = 0; i < res_size1[0]; i ++) {
        free(str_out1[i]);
        str_out1[i] = NULL;
    }
    free(str_out1);
    str_out1 = NULL;
    free(res_size1);
    res_size1 = NULL;

    for (int i = 0; i < res_size2[0]; i ++) {
        free(str_out2[i]);
        str_out2[i] = NULL;
    }
    free(str_out2);
    str_out2 = NULL;
    free(res_size2);
    res_size2 = NULL;

    for (int i = 0; i < res_size3[0]; i ++) {
        free(str_out3[i]);
        str_out3[i] = NULL;
    }
    free(str_out3);
    str_out3 = NULL;
    free(res_size3);
    res_size3 = NULL;

    for (int i = 0; i < res_size4[0]; i ++) {
        free(str_out4[i]);
        str_out4[i] = NULL;
    }
    free(str_out4);
    str_out4 = NULL;
    free(res_size4);
    res_size4 = NULL;

    for (int i = 0; i < res_size5[0]; i ++) {
        free(str_out5[i]);
        str_out5[i] = NULL;
    }
    free(str_out5);
    str_out5 = NULL;
    free(res_size5);
    res_size5 = NULL;

    for (int i = 0; i < res_size6[0]; i ++) {
        free(str_out6[i]);
        str_out6[i] = NULL;
    }
    free(str_out6);
    str_out6 = NULL;
    free(res_size6);
    res_size6 = NULL;

    for (int i = 0; i < res_size7[0]; i ++) {
        free(str_out7[i]);
        str_out7[i] = NULL;
    }
    free(str_out7);
    str_out7 = NULL;
    free(res_size7);
    res_size7 = NULL;

    for (int i = 0; i < res_size8[0]; i ++) {
        free(str_out8[i]);
        str_out8[i] = NULL;
    }
    free(str_out8);
    str_out8 = NULL;
    free(res_size8);
    res_size8 = NULL;

    for (int i = 0; i < res_size9[0]; i ++) {
        free(str_out9[i]);
        str_out9[i] = NULL;
    }
    free(str_out9);
    str_out9 = NULL;
    free(res_size9);
    res_size9 = NULL;

    for (int i = 0; i < res_size10[0]; i ++) {
        free(str_out10[i]);
        str_out10[i] = NULL;
    }
    free(str_out10);
    str_out10 = NULL;
    free(res_size10);
    res_size10 = NULL;

    for (int i = 0; i < res_size11[0]; i ++) {
        free(str_out11[i]);
        str_out11[i] = NULL;
    }
    free(str_out11);
    str_out11 = NULL;
    free(res_size11);
    res_size11 = NULL;

    return str_out;
}

