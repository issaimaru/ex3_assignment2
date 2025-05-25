#include <stdio.h>
#include "function.h"
#include "structs.h"
#include "define.h"

// 二次元配列bufferを空白文字で初期化する関数
void init_buffer(char (*buffer)[SCREEN_WIDTH]) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            buffer[y][x] = ' ';
        }
    }
}

void draw(int x, int y, int width, int height, char (*buffer)[SCREEN_WIDTH]) {
    if (x < 0 || y < 0 || x + width > SCREEN_WIDTH || y + height > SCREEN_HEIGHT) return;
    for(int i = x; i < x + width; i++) {
        buffer[y + height - 1][i] = '-';
        buffer[y][i] = '-';
    }
    for(int i = y + 1; i < y + height - 1; i++) {
        buffer[i][x] = '|';
        buffer[i][x + width - 1] = '|';
    }
}

void draw_block(int x, int y, int width, int height, char (*buffer)[SCREEN_WIDTH]) {
    for (int i = x; i < x + width; i++) {
        buffer[y][i] = '-';
        buffer[y + height - 1][i] = '-';
    }
    for (int i = y + 1; i < y + height - 1; i++) {
        buffer[i][x] = '|';
        buffer[i][x + width - 1] = '|';
    }
}

void update_buffer(struct Ball *ball, struct Block *blocks, struct Bar *bar, char (*buffer)[SCREEN_WIDTH]) {
    moveBall(ball); // ボールの移動
    isCollideBar(ball, bar); // ボールとバーの衝突判定
    isCollideBlock(ball, blocks); // ボールとブロックの衝突判定
    init_buffer(buffer);
    //棒の描画
    draw((*bar).x, (*bar).y, (*bar).width, 1, buffer);

    //ボールの描画
    draw((*ball).x, (*ball).y, (*ball).width, (*ball).height, buffer);

    //ブロックの描画
    for (int i = 0; i < BLOCK_NUM; i++) {
        if (blocks[i].isNotDestroyed == 1) {
            draw_block(blocks[i].x, blocks[i].y, blocks[i].width, blocks[i].height, buffer);
        }else{
            //printf("x:%d,y:%d,isNotDestroyed: %d\n", blocks[i].x, blocks[i].y, blocks[i].isNotDestroyed);
        }
    }

}