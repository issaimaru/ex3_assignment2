#include "gamemanager.h"
#include "ui/gamescreen.h"

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>



int main() {
    // initializing gamemanager
    game_manager gm;
    init_game(&gm);

    // Unitの生成


    const char* ballshape[] = {
        "@"
    };

    const char* barshape[] = {
        "^^^^"
    };

    const char* wall[] = {
        "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|"
    };

    const char* cell[] = {
        "================================================================================"
    };

    unit* playerbar = create_unit( gm.screen.width / 2 - 2, 4, 4, 1, barshape);
    unit* ball = create_unit( gm.screen.width / 2, 7, 1, 1, ballshape);
    unit* wall_left = create_unit(0, 0, 1, gm.screen.height, wall);
    unit* wall_right = create_unit(gm.screen.width - 1, 0, 1, gm.screen.height, wall);
    unit* cell_up = create_unit(0, gm.screen.height - 1, gm.screen.width, 1, cell);
    unit* cell_down = create_unit(0, 0, gm.screen.width, 1, cell);

    add_unit(&gm, ball);
    add_unit(&gm, playerbar);
    add_unit(&gm, wall_right);
    add_unit(&gm, wall_left);
    add_unit(&gm, cell_up);
    add_unit(&gm, cell_down);

    char key = 0;

    while (1) {

        const int ch = io_getch();
        if (ch == 27) {
            break;
        }
        if (ch != EOF) {
            key = ch;
            // 操作
            if (ch == 'w') playerbar->y++;
            if (ch == 's') playerbar->y--;
            if (ch == 'a') playerbar->x--;
            if (ch == 'd') playerbar->x++;
            if (ch == 'q') break;  // 'q'で終了

            // player1が壁に埋まったら元の位置にずらす。（擬似的な当たり判定）
            if (is_touching(playerbar, wall_left)) playerbar->x++;
            if (is_touching(playerbar, wall_right)) playerbar->x--;
            if (is_touching(playerbar, cell_down)) {
                printf("-----GameOver-----");
                break;
            };
            if (is_touching(playerbar, ball)) remove_unit(&gm, ball);
            if (is_touching(playerbar, cell_up)) playerbar->y--;
            update(&gm, key); // プレイヤーの位置が変わって時点で更新: 適宜更新するタイミングに挿入する
        }

        usleep(50000);// 0.05秒
    }

    cleanup_game(&gm); //メモリクリア
    return 0;
}