//
// Created by Asahi Isshiki on 25/05/08.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "ui/GameScreen.h"

#define MAX_UNITS 100

typedef struct {
    GameScreen screen;
    Unit* unitList[MAX_UNITS];
    int unitCount;
} GameManager;

void initGame(GameManager* gm);
void addUnit(GameManager* gm, Unit* unit);
void removeUnit(GameManager* gm, const Unit* unit);
void update(GameManager* gm, char key);
void cleanupGame(GameManager* gm);


#endif //GAMEMANAGER_H
