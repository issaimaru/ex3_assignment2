//
// Created by Asahi Isshiki on 25/05/08.
//

#include "gamemanager.h"

#include <stdio.h>

void init_game(game_manager* gm) {
    init_screen(&gm->screen);
    gm->unit_count = 0;
}

void add_unit(game_manager* gm, unit* unit) {
    if (gm->unit_count < MAX_UNITS) {
        gm->unit_list[gm->unit_count++] = unit;
    }
}

void remove_unit(game_manager* gm, const unit* unit) {
    for (int i = 0; i < gm->unit_count; i++) {
        if (gm->unit_list[i] == unit) {
            for (int k = i; k < gm->unit_count; k++) {
                gm->unit_list[k] = gm->unit_list[k+1];
            }
            gm->unit_count--;
            break;
        }
    }
}

void update(game_manager* gm, const char key) {

    printf("\n\nYou are pressing a key: %c\n\n\n", key);
    draw_screen(&gm->screen, gm->unit_list, gm->unit_count);
    putchar('\n');
}

void cleanup_game(game_manager* gm) {
    for (int i = 0; i < gm->unit_count; i++) {
        destroy_unit(gm->unit_list[i]);
    }
}