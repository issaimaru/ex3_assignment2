//
// Created by Asahi Isshiki on 25/05/08.
//

#include "gamemanager.h"

#include <stdio.h>

void init_game(game_manager* gm) {
    init_screen(&gm->screen);
    gm->unitCount = 0;
}

void add_unit(game_manager* gm, unit* unit) {
    if (gm->unitCount < MAX_UNITS) {
        gm->unitList[gm->unitCount++] = unit;
    }
}

void remove_unit(game_manager* gm, const unit* unit) {
    for (int i = 0; i < gm->unitCount; i++) {
        if (gm->unitList[i] == unit) {
            for (int k = i; k < gm->unitCount; k++) {
                gm->unitList[k] = gm->unitList[k+1];
            }
            gm->unitCount--;
            break;
        }
    }
}

void update(game_manager* gm, const char key) {

    printf("\n\nYou are pressing a key: %c\n\n\n", key);
    draw_screen(&gm->screen, gm->unitList, gm->unitCount);
    putchar('\n');
}

void cleanup_game(game_manager* gm) {
    for (int i = 0; i < gm->unitCount; i++) {
        destroy_unit(gm->unitList[i]);
    }
}