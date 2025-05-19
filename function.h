/*  
    * This file contains the function declarations for the game.
    *
    * created by issei takeuchi on 2025/05/08
*/

#ifndef FUNCTION_H
#define FUNCTION_H

void moveBar(struct Bar *bar, struct Ball *ball,char key);
void isCollideBlock(struct Ball *ball, struct Block *block);

#endif // FUNCTION_H