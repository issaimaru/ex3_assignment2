//
// Created by Asahi Isshiki on 25/05/08.
//

#ifndef UNIT_H
#define UNIT_H

typedef struct {
    int x;
    int y;
    int width;
    int height;
    char** shape;
} Unit;

Unit* createUnit(int x, int y, int width, int height, const char* pattern[]);
void destroyUnit(Unit* unit);
int isTouching(const Unit* unit1, const Unit* unit2);


#endif //UNIT_H
