// Copyright (c) 2022 Personal (Bingbing Feng)

#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../src/utils/post_processor.h"

int main() {
    
    char str[] = "▁THERE▁IS▁ACCORDING▁TO▁LEGEND▁BOILING▁POT▁OF▁GOLD▁AT▁ONE▁END";

    sentence_level_post_process(str);

    printf("%s\n", str);

    return 0;
}

