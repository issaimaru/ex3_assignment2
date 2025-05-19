#include <stdio.h>
#include <stdlib.h>
#include "define.h"
#include "function.h"
#include "structs.h"

/*
テストケース1(左から衝突)

Ball ball = {60, 110, 1, 1, 7, 0}; // x, y, width, height, dx, dy
Block blocks[2] = {
    {110, 110, 20, 20, false}, // 衝突する位置
    {200, 110, 20, 20, false}  // 衝突しない位置
};

結果：クリア

テストケース2(右から衝突後、左から衝突)

Ball ball = {60, 110, 1, 1, -7, 0}; // x, y, width, height, dx, dy
Block blocks[2] = {
    {0, 110, 20, 20, false}, // 衝突する位置
    {200, 110, 20, 20, false}  // 衝突しない位置
};

結果：クリア

テストケース3(上から衝突後、下から衝突)
Ball ball = {200, 110, 1, 1, 0 , -5}; // x, y, width, height, dx, dy
Block blocks[2] = {
    {200, 200, 20, 20, false}, // 衝突する位置
    {200, 50, 20, 20, false}  // 衝突しない位置
};

結果：クリア

テストケース4(縁の部分に衝突）
Ball ball = {0, 0, 1, 1, 5 , 5}; // x, y, width, height, dx, dy
Block blocks[2] = {
    {200, 200, 20, 20, false}, // 衝突する位置
    {300, 300, 20, 20, false}  // 衝突しない位置
};

結果：クリア



*/

int main(void) {
    // テスト用のボールとブロックを用意
    Ball ball = {0, 0, 1, 1, 5 , 5}; // x, y, width, height, dx, dy
    Block blocks[2] = {
        {190, 200, 20, 20, false}, // 衝突する位置
        {290, 110, 20, 20, false}  // 衝突しない位置
    };

    printf("Before collision:\n");
    printf("Ball: x=%d, y=%d, dx=%d, dy=%d\n", ball.x, ball.y, ball.dx, ball.dy);
    for (int i = 0; i < 2; i++) {
        printf("Block %d: x=%d, y=%d, destroyed=%d\n", i, blocks[i].x, blocks[i].y, blocks[i].isDestroyed);
    }

    printf("Press any key to simulate ball movement and collision detection...\n");

    int num = 0;
    while(1) {
        char key = getchar();
        if (key == 'q') {
            break; // 'q'を押すと終了
        }
        moveBall(&ball); // ボールを移動
        isCollideBlock(&ball, blocks, 2); // 衝突判定を実行
        printf("\nAfter collision[%d回目]:\n", num++);
        printf("Ball: x=%d, y=%d, dx=%d, dy=%d\n", ball.x, ball.y, ball.dx, ball.dy);

        for (int i = 0; i < 2; i++) {
            printf("Block %d: x=%d, y=%d, destroyed=%d\n", i, blocks[i].x, blocks[i].y, blocks[i].isDestroyed);
        }
    }

    return 0;
}