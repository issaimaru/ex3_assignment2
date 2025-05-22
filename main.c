#include <stdio.h>
#include "unit.h"
#include "define.h"
#include "io.h"


char buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
unit* units[UNIT_MAX];
int unitCount = 0;
unit* player;
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

void deploy_unit(unit* target) {
    if (!target || !target->shape) return;
    for (int dy = 0; dy < target->height; dy++) {
        for (int dx = 0; dx < target->width; dx++) {
            const int sx = target->x + dx;
            const int sy = target->y + dy;

            if (sx >= 0 && sx < SCREEN_WIDTH && sy >= 0 && sy < SCREEN_HEIGHT) {
                buffer[sy][sx] = target->shape[dy][dx];
            }
        }
    }
}

void add_unit(unit* unit) {
    if (unitCount < UNIT_MAX) {
        units[unitCount++] = unit;
    }
}

void remove_unit(unit* unit) {
    for (int i = 0; i < unitCount; i++) {
        if (units[i] == unit) {
            for (int k = i; k < unitCount - 1; k++) {
                units[k] = units[k+1];
            }
            unitCount--;
            break;
        }
    }
}

void update_buffer() {
    init_buffer();
     for (int i = 0; i < unitCount; i++) {
        unit* unit = units[i];
        deploy_unit(unit);
     }
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

    const char* shape1[] = {
        "-----"
    };

    const char* wall[] = {
        "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|"
    };

    const char* cell[] = {
        "================================================================================"
    };

    player = create_unit(3,3,5,1,shape1);
    unit* wall_left = create_unit(0, 0, 1, SCREEN_HEIGHT, wall);
    unit* wall_right = create_unit(SCREEN_WIDTH - 1, 0, 1, SCREEN_HEIGHT, wall);


    add_unit(player);
    add_unit(wall_left);
    add_unit(wall_right);

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
                player->x--;
            } else {
                player->x++;
            }
            update_buffer();
        }
     }

    destroy_unit(player);

    return 0;
#endif
    return 0;
}