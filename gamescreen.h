//
// Created by Asahi Isshiki on 25/05/08.
//

#ifndef UI_H
#define UI_H

#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80
#include "unit.h"

typedef struct {
    int width;
    int height;
} game_screen;


void init_screen(game_screen* screen);

void draw_screen(const game_screen* screen, const unit** units, const int unitsize);

#endif //UI_H
