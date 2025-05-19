//
// Created by Asahi Isshiki on 25/05/08.
//

#include "ui/gamescreen.h"

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void init_screen(game_screen* screen) {
    screen->height = SCREEN_HEIGHT;
    screen->width = SCREEN_WIDTH;
}

void draw_screen(const game_screen* screen, const unit** units, const int unitsize) {
    char buffer[screen->height][screen->width];

    for (int y = 0; y < screen->height; y++) {
        for (int x = 0; x < screen->width; x++) {
            buffer[y][x] = ' ';
        }
    }
    for (int i = 0; i < unitsize; i++) {
        const unit* unit = units[i];
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

