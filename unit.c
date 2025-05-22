//
// Created by Asahi Isshiki on 25/05/08.
//

#include "unit.h"
#include "define.h"

#include <stdio.h>

#include <stdlib.h>
#include <string.h>

unit* create_unit(const int x, const int y, int width, const int height, const char* pattern[]) {
    if (width <= 0 || height <= 0 || !pattern) {
        return NULL;
    }

    unit* u = malloc(sizeof(unit));
    if (!u) {
        return NULL;
    }

    u->x = x;
    u->y = SCREEN_HEIGHT - y - height;
    u->width = width;
    u->height = height;

    u->shape = malloc(sizeof(char*) * height);
    if (!u->shape) {
        free(u);
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        if (!pattern[i]) {
            u->shape[i] = NULL;
            continue;
        }

        u->shape[i] = malloc(width + 1);
        if (!u->shape[i]) {
            continue;
        }

        memset(u->shape[i], ' ', width);
        strncpy(u->shape[i], pattern[i], width);
        u->shape[i][width] = '\0';
    }

    return u;
}


void destroy_unit(unit* unit) {
    for (int i = 0; i < unit->height; i++) {
        free(unit->shape[i]);
    }
    free(unit->shape);
    free(unit);
}

int is_touching(const unit* unit1, const unit* unit2) {
    const int x1min = unit1->x;
    const int x1max = unit1->x + unit1->width - 1;
    const int y1min = unit1->y;
    const int y1max = unit1->y + unit1->height - 1;
    const int x2min = unit2->x;
    const int x2max = unit2->x + unit2->width - 1;
    const int y2min = unit2->y;
    const int y2max = unit2->y + unit2->height - 1;
    if (x2max > x1max && x2min > x1max|| x2max < x1min && x2min < x1min) return 0;
    if (y2max > y1max && y2min > y1max|| y2max < y1min && y2min < y1min) return 0;
    return 1;
}