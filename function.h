/*  
    * This file contains the function declarations for the game.
    *
    * created by issei takeuchi on 2025/05/08
*/

#ifndef FUNCTION_H
#define FUNCTION_H

#include "structs.h"

void blockinit(Block *block);
void moveBar(Bar *bar, Ball *ball,char key);
void moveBall(Ball *ball);
void isCollideBlock(Ball *ball, Block *block);

#endif // FUNCTION_H