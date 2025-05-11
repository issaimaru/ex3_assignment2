#include <stdio.h>
#include "function.h"
#include "structs.h"

Bar bar = {100,0,200,300,10};
Ball ball = {100,50,3,1,1};
Block block[10] = {100, 100, 0, 0, false};

int main(void) {
    printf("before: %d %d\n", bar.x,bar.speed);
    char key = getchar();
    moveBar(&bar, &ball, key);
    printf("after: %d %d\n", bar.x,bar.speed);
    return 0;
}
//
