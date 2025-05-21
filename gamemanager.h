//
// Created by Asahi Isshiki on 25/05/08.
//

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "gamescreen.h"

#define MAX_UNITS 100

typedef struct {
    game_screen screen;
    unit* unitList[MAX_UNITS];
    int unitCount;
} game_manager;

void init_game(game_manager* gm);
void add_unit(game_manager* gm, unit* unit);
void remove_unit(game_manager* gm, const unit* unit);
void update(game_manager* gm, char key);
void cleanup_game(game_manager* gm);


#endif //GAMEMANAGER_H
