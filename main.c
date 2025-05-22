#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "define.h"
#include "function.h"
#include "structs.h"

int main() {
#if defined(NATIVE_MODE)
    printf("do nothing...\n");
#else
    enable_UART_interrupt();
    gpio->data = 0;
    int terminated = 0;
    unsigned gpio_data = 0;
    int i;
    for (i = 0; i < 2; ++i) {
        set_uart_ID(i);
    }
    while (!terminated) {
        UARTBuf* ub_rx;
        for (i = 0; i < 2; ++i) {
            ub_rx = &UART_BUF_RX[i];
            if (ub_rx->num_new_lines || ub_rx->terminate_code || ub_rx->full) {
                break; /// UART[i] RX is not empty
            }
        }
        if (i == 2) { continue; }
        set_uart_ID(i);
        char ch;
        char mem[10] = {0};
        int disp = 0;
        int j = 0;
        if (ub_rx->num_new_lines == 0) {
            io_putch('\n');
        }
        // テスト用のボールとブロックを用意
        Ball ball = {0, 0, 1, 1, 5 , 5}; // x, y, width, height, dx, dy
        Block blocks[2] = {
            {200, 200, 20, 20, false}, // 衝突する位置
            {300, 300, 20, 20, false}  // 衝突しない位置
        };

        printf("Before collision:\n");
        printf("Ball: x=%d, y=%d, dx=%d, dy=%d\n", ball.x, ball.y, ball.dx, ball.dy);
        for (int i = 0; i < 2; i++) {
            printf("Block %d: x=%d, y=%d, destroyed=%d\n", i, blocks[i].x, blocks[i].y, blocks[i].isDestroyed);
        }

        printf("Press any key to simulate ball movement and collision detection...\n");

        int num = 0;
        while(1) {
            char key = io_getch();
            if (key == 'q') {
                break; // 'q'を押すと終了
            }else if(key == '\n'){
                moveBall(&ball); // ボールを移動
                isCollideBlock(&ball, blocks, 2); // 衝突判定を実行
                printf("\nAfter collision[%d回目]:\n", num++);
                printf("Ball: x=%d, y=%d, dx=%d, dy=%d\n", ball.x, ball.y, ball.dx, ball.dy);

                for (int i = 0; i < 2; i++) {
                    printf("Block %d: x=%d, y=%d, destroyed=%d\n", i, blocks[i].x, blocks[i].y, blocks[i].isDestroyed);
                }
            }
        }
    }
    set_uart_ID(0);
    #endif
    return 0;
}