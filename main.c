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
Ball ball;
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

int seed = 2; // 乱数のシード値
int terminated = 0; // プログラム終了フラグ

int main() {
    #if !defined(NATIVE_MODE)
    while(!terminated){
        end = 0; // 終了フラグをリセット
        int seed = 0;
        printf("Press enter...");
        while (1) {
            if (io_getch() == '\n') {
                break;
            }
        }
        ball.width = 3;
        ball.height = 3;
        ball.x = u_rand(seed) % (SCREEN_WIDTH - ball.width) + 1; // ボールの初期位置をランダムに設定
        ball.y = u_rand(seed) % (SCREEN_HEIGHT - ball.height - BLOCK_HEIGHT * BLOCK_ROWS) + 1; // ボールの初期位置をランダムに設定
        ball.dx = u_rand(seed) % 2 + 1;
        ball.dy = u_rand(seed) % 2 + 1;
        ball.isGameOver = 0;
        printf("\nBall initial position: x=%d, y=%d, dx=%d, dy=%d\n", ball.x, ball.y, ball.dx, ball.dy);


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
    }
#endif
    return 0;
}