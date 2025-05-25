#include <stdio.h>
#include "function.h"
#include "define.h"
#include "structs.h"
#include "monitor.h"
#include "io.h"

#if defined(NATIVE_MODE)
#define io_putch putchar
#endif

char buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
int end = 0;

 // ball, wall, barの宣言
Bar bar = {0, 0, 10, 1};
Ball ball = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 3, 3, 2, 1};
Block blocks[BLOCK_NUM];

// 画面にbufferの内容を出力する関数
void print_buffer() {
    update_buffer(&ball, blocks, &bar, buffer);
    printf("\033[1;1H"); // カーソルを画面の左上に移動
    // bufferの内容を出力

    for (int y = SCREEN_HEIGHT-1 ; y >= 0; y--) {
        printf("\033[2K"); // 行をクリア
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            io_putch(buffer[y][x]);
        }
        io_putch('\n');
    }
}

// タイマー割り込みで画面を更新
void timer_hook() {
    print_buffer();
}

int main() {
#if defined(NATIVE_MODE)
    init_buffer(buffer);
    
    blockinit(blocks);

    while (1) {
        if (end) {
            break;
        }
        char ch = getchar();
        moveBar(&bar, &ball, ch); // 棒の移動

        print_buffer(); // 画面の更新
     }

#else
    init_buffer(buffer);
    blockinit(blocks);

    set_mtimer_interval(300);
    timer_interrupt_hook = timer_hook;
    enable_timer_interrupt();

    while (1) {
        if (end) break;
        // 割り込みバッファにデータがあるか監視
        char ch = io_getch(); // 1文字取得
        if (ch == TERMINATE_CODE) {
            end = 1;
            printf("\nCtrl-Z detected...\n");
            unsigned mask = 0x80;
            asm volatile("csrc mie, %0" :: "r"(mask));
            sysctrl->mtimecmp = (unsigned long long)-1;
        } else {
            moveBar(&bar, &ball, ch); // 棒の移動
        }
        // 画面更新はタイマー割り込み(timer_hook)で自動実行
    }

    return 0;
#endif
}