#include <stdio.h>
#include "define.h"
#include "structs.h"
#include "io.h"


char buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
int end = 0;

void init_buffer() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            buffer[y][x] = ' ';
        }
    }
}

#if !defined(NATIVE_MODE)
void print_buffer() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            io_putch(buffer[y][x]);
        }
        io_putch('\n');
    }
}
#endif


// deploy block, deploy wall, deploy bar

//


void update_buffer() {
    init_buffer();
    // deploy wall, ball, bar
}

#if !defined(NATIVE_MODE)
void timer_hook() {
    for (int i = 0; i < 30 - SCREEN_HEIGHT; i++) {
        io_putch('\n');
    } 
    print_buffer();
}
#endif

int main() {
#if defined(NATIVE_MODE)
    printf("do nothing...");
#else
    init_buffer();
    print_buffer();

    // ここにball, wall, barの宣言

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
             unsigned mask = 0x80;
            asm volatile("csrc mie, %0" :: "r"(mask));
            sysctrl->mtimecmp = (unsigned long long)-1;
        } else if (ch == 'a' || ch == 'd'){
            if (ch == 'a') {
                // movebar
            } else {
                // movebar
            }
            update_buffer();
        }
     }

    return 0;
#endif
    return 0;
}