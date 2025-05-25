#include <stdio.h>
#include "function.h"
#include "define.h"
#include "structs.h"
#include "io.h"

#if defined(NATIVE_MODE)
#define io_putch putchar
#endif

char buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
int end = 0;

// 二次元配列bufferを空白文字で初期化する関数
void init_buffer() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            buffer[y][x] = ' ';
        }
    }
}

void draw(int x, int y, int width, int height){
    for(int i = x; i < x + width; i++) {
        buffer[y + height][i] = '-';
        buffer[y][i] = '-';
    }
    for(int i = y + 1; i < y + height - 1; i++) {
        buffer[i][x] = '|';
        buffer[i][x + width] = '|';
    }
}

void update_buffer(struct Ball *ball, struct Block *blocks, struct Bar *bar) {
    moveBall(ball); // ボールの移動
    isCollideBlock(ball, blocks); // ボールとブロックの衝突判定
    init_buffer();
    //棒の描画
    draw((*bar).x, (*bar).y, (*bar).width, 1);

    //ボールの描画
    draw((*ball).x, (*ball).y, (*ball).width, (*ball).height);

    //ブロックの描画
    for (int i = 0; i < BLOCK_NUM; i++) {
        if (!blocks[i].isDestroyed) {
            draw(blocks[i].x, blocks[i].y, blocks[i].width, blocks[i].height);
        }
    }
}

// 画面にbufferの内容を出力する関数
void print_buffer() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            io_putch(buffer[y][x]);
        }
        io_putch('\n');
    }
}

// タイマー割り込みで画面を更新
void timer_hook() {
    for (int i = 0; i < 30 - SCREEN_HEIGHT; i++) {
        io_putch('\n');
    } 
    print_buffer();
}

int main() {
#if defined(NATIVE_MODE)
    printf("do nothing...");
#else
    init_buffer();
    print_buffer();

    // ball, wall, barの宣言
    Bar bar = {0, 0, 10, 1};
    Ball ball = {0, 0, 1, 1, 0, 0};
    Block blocks[BLOCK_NUM];
    
    blockinit(blocks);

    set_mtimer_interval(300);
    timer_interrupt_hook = timer_hook;
    enable_timer_interrupt();
     while (1) {
        if (end) {
            break;
        }
        char ch = io_getch();
        if (ch == TERMINATE_CODE) {
            end = 1;
            printf("\nCtrl-Z detected...\n");

             //タイマー割り込みの無効化
            unsigned mask = 0x80;
            asm volatile("csrc mie, %0" :: "r"(mask));
            sysctrl->mtimecmp = (unsigned long long)-1;
        } else{
            moveBar(&bar, &ball, ch); // 棒の移動
            update_buffer(&ball, blocks, &bar);
        }
     }

    return 0;
#endif
    return 0;
}