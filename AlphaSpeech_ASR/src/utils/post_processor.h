// Copyright (c) 2022 Personal (Bingbing Feng)

#ifndef __POST_PROCESSOR_H__
#define __POST_PROCESSOR_H__

void process_underscore(char *str);
void ltrim(char *str);
void rtrim(char *str);
void process_spaces(char *str);
void sentence_level_post_process(char *str);
void word_level_post_process(char *str);

#endif
