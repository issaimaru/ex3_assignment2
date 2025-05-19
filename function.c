/*  
    * This file contains the function declarations for the game.
    *
    * created by issei takeuchi on 2025/05/08
*/

#include "define.h"
#include "structs.h"
#include "function.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//棒を動かす関数
//デバック完了、エラーなし(2025/05/12)
void moveBar(Bar *bar, Ball *ball,char key){
    switch(key) {
        case LEFT_KEY:
            if((*bar).x > 0){
                (*bar).x -= (*bar).speed;
            }
            break;

        case RIGHT_KEY:
            if((*bar).x + (*bar).width < SCREEN_WIDTH){
                (*bar).x += (*bar).speed;
            }
            break;

        case UP_KEY:
            if ((*bar).speed < MAX_BAR_SPEED){
                (*bar).speed += BAR_SPEED_INCREMENT;
            }
            break;
        
        case DOWN_KEY:
            if ((*bar).speed > MIN_BAR_SPEED){
                (*bar).speed -= BAR_SPEED_DECREMENT;
            }
            break;

        default:
            break;
    }
}

//デバック完了、エラーなし(2025/05/12)
void moveBall(Ball *ball){
    //ボールの移動
    (*ball).x += (*ball).dx;
    (*ball).y += (*ball).dy;

    //画面の端に当たった場合、ボールの速度を反転させる
    if ((*ball).x < 0 || (*ball).x + (*ball).radius > SCREEN_WIDTH) {
        (*ball).x = (*ball).x < 0 ? 0 : SCREEN_WIDTH - (*ball).radius;
        (*ball).dx = -(*ball).dx;
    }
    if ((*ball).y < 0 || (*ball).y + (*ball).radius > SCREEN_HEIGHT) {
        (*ball).y = (*ball).y < 0 ? 0 : SCREEN_WIDTH - (*ball).radius;
        (*ball).dy = -(*ball).dy;
    }
}


//矩形と矩形の衝突アルゴリズムを利用したブロックとボールの衝突判定関数
void isCollideBlock(Ball *ball, Block *block, int BlockCount){
    // ボールとブロックの衝突判定
    for (int i = 0; i < BlockCount; i++) {
        if (!block[i].isDestroyed) {
            float x_m_block = block[i].x + (float)block[i].width/2;
            float y_m_block = block[i].y + (float)block[i].height/2;

            float x_m_ball = ball.x + (float)ball.radius/2;
            float y_m_ball = ball.y + (float)ball.radius/2;

            if (abs(x_m_block - x_m_ball) < block[i].width + ball.radius && abs(y_m_block - y_m_ball) < block[i].height + ball.radius) {
                //TODO:ここに衝突時の処理を記述
                block[i].isDestroyed = true;
                //ボールの速度を反転
                if((*ball).dx > 0){
                    (*ball).dx = -(*ball).dx
                }else{
                    
                }
            }
        }
    }
}