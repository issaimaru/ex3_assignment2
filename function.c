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

void AABBJudge(Ball *ball, Block *block){
    //直線的と仮定すると，t0 = (P1 - P2)/vの時に衝突する．

    double t_0;
    //左右に衝突する場合
    if ((*ball).dx > 0){
        t_0 = ((*block).x - ((*ball).x + (*ball).radius)) / (*ball).dx;
    }else{
        t_0 = ((*block).x + (*block).width - ((*ball).x - (*ball).radius)) / (*ball).dx;
    } 

    if(t_0 > 0 && t_0 < 1) {
        if(((*ball).y + (*ball).radius) + (*ball).dy * t_0 > (*block).y && ((*ball).y - (*ball).radius) + (*ball).dy * t_0 < (*block).y + (*block).height){
            //衝突した場合，ボールの速度を反転させる
            (*ball).dx = -(*ball).dx;
            //ブロックを壊す
            (*block).isDestroyed = true;
        }
    }

    //上下に衝突する場合
    if ((*ball).dy > 0){
        t_0 = ((*block).y - ((*ball).y + (*ball).radius)) / (*ball).dy;
    }else if ((*ball).dy < 0){
        t_0 = ((*block).y + (*block).height - ((*ball).y - (*ball).radius)) / (*ball).dy;
    }

    if(t_0 > 0 && t_0 < 1) {
        if(((*ball).x + (*ball).radius) + (*ball).dx * t_0 > (*block).x && ((*ball).x - (*ball).radius) + (*ball).dx * t_0 < (*block).x + (*block).width){
            //衝突した場合，ボールの速度を反転させる
            (*ball).dy = -(*ball).dy;
            //ブロックを壊す
            (*block).isDestroyed = true;
        }
    }
} 

//AABB衝突判定を利用したブロックとボールの衝突判定関数
void isCollideBlock(Ball *ball, Block *block, int BlockCount){
    // ボールとブロックの衝突判定
    for (int i = 0; i < BlockCount; i++) {
        if (!block[i].isDestroyed) {
            AABBJudge(ball, &block[i]);
        }
    }
}