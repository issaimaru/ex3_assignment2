#include "gamemanager.h"
#include "gamescreen.h"

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>


int main() {
    // initializing gamemanager
    game_manager gm;
    init_game(&gm);

    // Unitの生成

    const char* shape1[] = {
        "  -  -  ",
        "[@]  [@]",
        "  WWWW  "
    };

    const char* shape2[] = {
        "  -  -  ",
        "[X]  [X]",
        "  |__|  "
    };
    const char* wall[] = {
        "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|", "|"
    };

    const char* cell[] = {
        "================================================================================"
    };

    unit* player1 = create_unit(10, 5, 8, 3, shape1);
    unit* player2 = create_unit(10, 9, 8, 3, shape2);
    unit* wall_left = create_unit(0, 0, 1, gm.screen.height, wall);
    unit* wall_right = create_unit(gm.screen.width - 1, 0, 1, gm.screen.height, wall);
    unit* cell_up = create_unit(0, gm.screen.height - 1, gm.screen.width, 1, cell);
    unit* cell_down = create_unit(0, 0, gm.screen.width, 1, cell);

    add_unit(&gm, player1);
    add_unit(&gm, player2);
    add_unit(&gm, wall_right);
    add_unit(&gm, wall_left);
    add_unit(&gm, cell_up);
    add_unit(&gm, cell_down);

    //

    char key = 0;

    while (1) {

        const int ch = io_getch();
        if (ch == 27) {
            break;
        }
        if (ch != EOF) {
            key = ch;
            // 操作
            if (ch == 'w') player1->y++;
            if (ch == 's') player1->y--;
            if (ch == 'a') player1->x--;
            if (ch == 'd') player1->x++;
            if (ch == 'q') break;  // 'q'で終了

            // player1が壁に埋まったら元の位置にずらす。（擬似的な当たり判定）
            if (is_touching(player1, wall_left)) player1->x++;
            if (is_touching(player1, wall_right)) player1->x--;
            if (is_touching(player1, cell_down)) {
                printf("-----GameOver-----");
                break;
            };
            if (is_touching(player1, player2)) remove_unit(&gm, player2);
            if (is_touching(player1, cell_up)) player1->y--;
            update(&gm, key); // プレイヤーの位置が変わって時点で更新: 適宜更新するタイミングに挿入する
        }
    }

    cleanup_game(&gm); //メモリクリア

    return 0;
}