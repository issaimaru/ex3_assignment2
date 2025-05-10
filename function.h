/*  
    * This file contains the function declarations for the game.
    *
    * created by issei takeuchi on 2025/05/08
*/

#ifndef FUNCTION_H
#define FUNCTION_H

#include "structs.h"

void moveBar(Bar *bar, Ball *ball,char key);
void isCollideBlock(Ball *ball, Block *block);

#endif // FUNCTION_H