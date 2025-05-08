#include "GameManager.h"
#include "ui/GameScreen.h"

#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdio.h>

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

void clear_screen() {
    printf("\033[2J");    // ANSIエスケープコードで画面クリア
}


int main() {
    GameManager gm;
    initGame(&gm);

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
    Unit* wall_left = createUnit(0, 0, 1, 25, wall);
    Unit* wall_right = createUnit(79, 0, 1, 25, wall);
    Unit* cell_up = createUnit(0, 24, 80, 1, cell);
    Unit* cell_down = createUnit(0, 0, 80, 1, cell);

    addUnit(&gm, player1);
    addUnit(&gm, player2);
    addUnit(&gm, wall_right);
    addUnit(&gm, wall_left);
    addUnit(&gm, cell_up);
    addUnit(&gm, cell_down);

    enableNonBlockingInput();
    char key = 0;

    while (1) {

        const int ch = getchar();
        if (ch == 27) {
            break;
        }

        if (ch != EOF) {
            key = ch;
            if (ch == 'a') player1->x--;
            if (ch == 'd') player1->x++;
            if (ch == 'q') break;  // 'q'で終了
            if (isTouching(player1, wall_left)) player1->x++;
            if (isTouching(player1, wall_right)) player1->x--;
            if (isTouching(player1, cell_down)) player1->y++;
            if (isTouching(player1, cell_up)) player1->y--;
            update(&gm, key);
        }

        usleep(50000);
    }

    disableNonBlockingInput();
    cleanupGame(&gm);
    return 0;
}