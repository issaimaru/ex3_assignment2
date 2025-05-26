/*  
    * This file contains the monitor function for the game.
    *
    * created by issei takeuchi on 2025/05/25
*/

#ifndef MONITOR_H
#define MONITOR_H

#include "structs.h"
#include "define.h"
// 二次元配列bufferを空白文字で初期化する関数
void init_buffer();

void draw(int x, int y, int width, int height, char (*buffer)[SCREEN_WIDTH]);

void draw_block(int x, int y, int width, int height, char (*buffer)[SCREEN_WIDTH]);

void update_buffer(struct Ball *ball, struct Block *blocks, struct Bar *bar, char (*buffer)[SCREEN_WIDTH],int *counter);

void print_gameover();
#endif // MONITOR_H