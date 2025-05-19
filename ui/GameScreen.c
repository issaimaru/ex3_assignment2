//
// Created by Asahi Isshiki on 25/05/08.
//

#include "GameScreen.h"

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void initScreen(GameScreen* screen) {
    screen->height = SCREEN_HEIGHT;
    screen->width = SCREEN_WIDTH;
}

void drawScreen(const GameScreen* screen, const Unit** units, const int unitsize) {
    char buffer[screen->height][screen->width];

    for (int y = 0; y < screen->height; y++) {
        for (int x = 0; x < screen->width; x++) {
            buffer[y][x] = ' ';
        }
    }
    for (int i = 0; i < unitsize; i++) {
        const Unit* unit = units[i];
        for (int dy = 0; dy < unit->height; dy++) {
            for (int dx = 0; dx < unit->width; dx++) {
                const int sx = unit->x + dx;
                const int sy = unit->y + dy;

                if (sx >= 0 && sx < screen->width && sy >= 0 && sy < screen->height) {
                    buffer[screen->height - sy - 1][sx] = unit->shape[dy][dx];
                }
            }
        }
    }
    for (int y = 0; y < screen->height; y++) {
        for (int x = 0; x < screen->width; x++) {
            putchar(buffer[y][x]);
        }
        putchar('\n');
    }
}

