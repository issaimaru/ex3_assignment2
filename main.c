#include <stdio.h>
#include <string.h>
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
int counter;

 // ball, wall, barの宣言
Bar bar = {0, 0, 15, 1};
Ball ball;
Block blocks[BLOCK_NUM];

// 画面にbufferの内容を出力する関数
void print_buffer(int mode) {
    if(mode == 0)update_buffer(&ball, blocks, &bar, buffer,&counter);
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

void print_moji(const char *moji, int x, int y, int isRow) {
    if (moji == NULL) return;
    if(!isRow) init_buffer(buffer);
    int len = 0;
    while (moji[len] != '\0' && len < 100) len++;
    if (len <= 0 || len > 100) return;
    for(int i = 0; i < len; i++) {
        int px = x - len/2 + i;
        buffer[y][px] = moji[i];
    }
}

void print_gameover() {
    print_moji("GAME OVER", SCREEN_WIDTH/2, SCREEN_HEIGHT/2,0);
    print_moji("Enter: continue the game", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 1,1);
    print_moji("Ctrl+Z: terminate the game", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 2,1);
}

void print_initgame(){
    print_moji("This is a game of Breakout!", SCREEN_WIDTH/2, SCREEN_HEIGHT/2,0);
    print_moji("please press any key!!", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 1,1);
}

void print_escapemode() {
    print_moji("Escape mode", SCREEN_WIDTH/2, SCREEN_HEIGHT/2,1);
    print_moji("Press any key to return to the game", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 1,1);
}

// タイマー割り込みで画面を更新
void timer_hook() {
    print_buffer(0);
}

void nop() {
    // 何もしない関数
}

int seed = 81; // 乱数のシード値
int monitor_mode = INIT_MODE; // モニターモードの初期値
int terminated = 0; // プログラム終了フラグ

int main() {
    for(int i=0;i<30;i++){
        printf("\n");
    }
    #if !defined(NATIVE_MODE)
    while(terminated == 0){
        end = 0; // 終了フラグをリセット
        counter = 0;
        ball.width = 3;
        ball.height = 3;
        ball.x = mod(seed, SCREEN_WIDTH - ball.width) + 1; // ボールの初期位置をランダムに設定
        ball.y = mod(seed, SCREEN_HEIGHT - ball.height - BLOCK_HEIGHT * BLOCK_ROWS) + 1; // ボールの初期位置をランダムに設定
        ball.dx = mod(seed, 2) + 1;
        ball.dy = mod(seed, 2) + 1;
        ball.isGameOver = 0;

        init_buffer(buffer);
        blockinit(blocks);

        set_mtimer_interval(300);
        timer_interrupt_hook = timer_hook;
        enable_timer_interrupt();
        while (!end) {
            if(ball.isGameOver) {
                monitor_mode = END_MODE;
            }
            // 割り込みバッファにデータがあるか監視
            if(monitor_mode == INIT_MODE) {
                timer_interrupt_hook = nop;
                print_initgame();
                print_buffer(1);
                while (1) {
                    char ch = io_getch(); // 1文字取得
                    if (ch != ' '){
                        monitor_mode = GAME_MODE; // ゲームモードに戻る
                        printf("Game started.\n");
                        end = 1; // ゲーム開始フラグを立てる
                        break;
                    }
                }
            }
            else if(monitor_mode == GAME_MODE) {
                char ch = io_getch(); // 1文字取得
                if (ch == TERMINATE_CODE) {
                    end = 1;
                    timer_interrupt_hook = nop;
                }else if(ch == ESCAPE_KEY){
                    monitor_mode = ESCAPE_MODE; // エスケープモードに移行
                }else if(counter >= BLOCK_NUM) {
                    monitor_mode = COMPLETE_MODE; // ゲームクリアモードに移行
                }else{
                    moveBar(&bar, &ball, ch); // 棒の移動
                }
                // 画面更新はタイマー割り込み(timer_hook)で自動実行
                seed = mod(seed+1, 1000);
            }else if(monitor_mode == END_MODE) {
                timer_interrupt_hook = nop;
                print_gameover();
                print_buffer(1);
                while (1) {
                    char ch = io_getch(); // 1文字取得
                    if (ch == TERMINATE_CODE) {
                        end = 1;
                        terminated = 1; // プログラム終了フラグを立てる
                        printf("Program terminated.\n");
                        init_buffer(buffer); // 画面をクリア
                        print_buffer(1); // 画面を更新
                        break;
                    } else if (ch == '\n' || ch == '\r') {
                        monitor_mode = GAME_MODE; // ゲームモードに戻る
                        end = 1;
                        break;
                    }
                }
            }else if(monitor_mode == ESCAPE_MODE) {
                timer_interrupt_hook = nop;
                print_escapemode();
                print_buffer(1);
                while (1) {
                    char ch = io_getch(); // 1文字取得
                    if (ch != ' ') {
                        timer_interrupt_hook = timer_hook; // タイマー割り込みを再開
                        monitor_mode = GAME_MODE; // ゲームモードに戻る
                        break;
                    }
                }
            }else if(monitor_mode == COMPLETE_MODE) {
                timer_interrupt_hook = nop;
                print_moji("Congratulations! You completed the game!", SCREEN_WIDTH/2, SCREEN_HEIGHT/2,0);
                print_moji("Press any key to continue", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 1,1);
                print_moji("Ctrl+Z: terminate the game", SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 2,1);
                print_buffer(1);
                while (1) {
                    char ch = io_getch(); // 1文字取得
                    if (ch == TERMINATE_CODE) {
                        end = 1;
                        terminated = 1; // プログラム終了フラグを立てる
                        printf("Program terminated.\n");
                        init_buffer(buffer); // 画面をクリア
                        print_buffer(1); // 画面を更新
                        break;
                    }else if (ch != ' ') {
                        monitor_mode = GAME_MODE; // ゲームモードに戻る
                        end = 1;
                        break;
                    }
                }
            }
            
        }
    }
    while(terminated == 1){nop();}
#endif
    return 0;
}