#pragma once
#include "YandexArt/Yandexart.h"

#include "config.h"
YandexArt gen;
bool next_image_flag = 0;
bool gen_flag = 0;
bool gen_prompt_flag = 0;

void clearFlags() {
    next_image_flag = 0;
    gen_flag = 0;
    gen_prompt_flag = 0;
}

void next_image() {
    clearFlags();
    next_image_flag = 1;
}

void generate() {
    clearFlags();
    gen_flag = 1;
}

void generatePrmt() {
    clearFlags();
    gen_prompt_flag = 1;
}




void gen_tick() {
    gen.tick();

    if (next_image_flag) {
        clearFlags();

        gen.setScale(DISP_SCALE);
        gen.next_image();
    } else if (gen_flag) {
        clearFlags();
        gen.generate();
    }  else if (gen_flag) {
        clearFlags();
        gen.generatePrmt(
            db[kk::gen_query]);
    }
}