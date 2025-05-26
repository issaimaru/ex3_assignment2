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
void isCollideBar(Ball *ball, Bar *bar);
void isCollideBlock(Ball *ball, Block *block, int *counter);

int mod(int value, int mod_value);


#endif // FUNCTION_H