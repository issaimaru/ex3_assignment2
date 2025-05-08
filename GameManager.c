//
// Created by Asahi Isshiki on 25/05/08.
//

#include "GameManager.h"

#include <stdio.h>

void initGame(GameManager* gm) {
    initScreen(&gm->screen);
    gm->unitCount = 0;
}

void addUnit(GameManager* gm, Unit* unit) {
    if (gm->unitCount < MAX_UNITS) {
        gm->unitList[gm->unitCount++] = unit;
    }
}

void update(GameManager* gm, const char key) {

    printf("\n\nYou are pressing a key: %c\n\n\n", key);
    drawScreen(&gm->screen, gm->unitList, gm->unitCount);
    putchar('\n');
}

void cleanupGame(GameManager* gm) {
    for (int i = 0; i < gm->unitCount; i++) {
        destroyUnit(gm->unitList[i]);
    }
}