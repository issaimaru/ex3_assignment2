#include "GameManager.h"
#include "ui/GameScreen.h"

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>

 // 標準ターミナルの割り込み
void enableNonBlockingInput() {
    struct termios ttystate;

    // 現在のターミナル設定を取得
    tcgetattr(STDIN_FILENO, &ttystate);

    // 非カノニカルモード、エコーオフ
    ttystate.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    // stdinをノンブロッキングに設定
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void disableNonBlockingInput() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

int main() {
    // initializing gamemanager
    GameManager gm;
    initGame(&gm);

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

    Unit* player1 = createUnit(10, 5, 8, 3, shape1);
    Unit* player2 = createUnit(10, 9, 8, 3, shape2);
    Unit* wall_left = createUnit(0, 0, 1, gm.screen.height, wall);
    Unit* wall_right = createUnit(gm.screen.width - 1, 0, 1, gm.screen.height, wall);
    Unit* cell_up = createUnit(0, gm.screen.height - 1, gm.screen.width, 1, cell);
    Unit* cell_down = createUnit(0, 0, gm.screen.width, 1, cell);

    addUnit(&gm, player1);
    addUnit(&gm, player2);
    addUnit(&gm, wall_right);
    addUnit(&gm, wall_left);
    addUnit(&gm, cell_up);
    addUnit(&gm, cell_down);

    //

    enableNonBlockingInput(); // 割り込み禁止
    char key = 0;

    while (1) {

        const int ch = getchar();
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
            if (isTouching(player1, wall_left)) player1->x++;
            if (isTouching(player1, wall_right)) player1->x--;
            if (isTouching(player1, cell_down)) {
                printf("-----GameOver-----");
                break;
            };
            if (isTouching(player1, player2)) removeUnit(&gm, player2);
            if (isTouching(player1, cell_up)) player1->y--;
            update(&gm, key); // プレイヤーの位置が変わって時点で更新: 適宜更新するタイミングに挿入する
        }

        usleep(50000);// 0.05秒
    }

    disableNonBlockingInput();
    cleanupGame(&gm); //メモリクリア
    return 0;
}