//
// Created by Asahi Isshiki on 25/05/08.
//

#ifndef UI_H
#define UI_H

#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80
#include "Unit.h"

typedef struct {
    int width;
    int height;
} GameScreen;


void initScreen(GameScreen* screen);

void drawScreen(const GameScreen* screen, const Unit** units, const int unitsize);

#endif //UI_H
