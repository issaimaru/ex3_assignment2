#include <stdio.h>
#include <stdlib.h>
#include "define.h"
#include "function.h"
#include "structs.h"

Bar bar = {100,0,300,5};
Ball ball = {100,50,3,1,1};
Block block[10] = {100, 100, 0, 0, false};

int main(void) {
    printf("before: %d %d\n", bar.x,bar.speed);
    while(1){
        char key = getchar();
        moveBar(&bar, &ball, key);
        printf("After: %d %d\n", bar.x,bar.speed);
        //printf("x+radious:%d,screen_width:%d\n", (ball).x + (ball).radius, SCREEN_WIDTH);
        //printf("y+radious:%d,screen_height:%d\n", (ball).y + (ball).radius,SCREEN_HEIGHT);
    }
    return 0;
}
//