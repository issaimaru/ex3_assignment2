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
} unit;

unit* create_unit(int x, int y, int width, int height, const char* pattern[]);
void destroy_unit(unit* unit);
int is_touching(const unit* unit1, const unit* unit2);


#endif //UNIT_H