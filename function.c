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
#include <math.h>

void blockinit(Block *block){
    //ブロックの初期化
    for (int j=0; j < BLOCK_ROWS; j++) {
        for (int i = 0; i < BLOCK_NUM / BLOCK_ROWS; i++) {
            block[i + j * (BLOCK_NUM / BLOCK_ROWS)].x = i * BLOCK_WIDTH;
            block[i + j * (BLOCK_NUM / BLOCK_ROWS)].y = (SCREEN_HEIGHT - BLOCK_HEIGHT * BLOCK_ROWS) + j * BLOCK_HEIGHT;
            block[i + j * (BLOCK_NUM / BLOCK_ROWS)].width = BLOCK_WIDTH;
            block[i + j * (BLOCK_NUM / BLOCK_ROWS)].height = BLOCK_HEIGHT;
            block[i + j * (BLOCK_NUM / BLOCK_ROWS)].isDestroyed = false;
        }
    }
}

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
    if ((*ball).x < 0 || (*ball).x + (*ball).width > SCREEN_WIDTH) {
        (*ball).x = (*ball).x < 0 ? 0 : SCREEN_WIDTH - (*ball).width;
        (*ball).dx = -(*ball).dx;
    }
    if ((*ball).y < 0 || (*ball).y + (*ball).height > SCREEN_HEIGHT) {
        (*ball).y = (*ball).y < 0 ? 0 : SCREEN_HEIGHT - (*ball).height;
        (*ball).dy = -(*ball).dy;
    }
}


//矩形と矩形の衝突アルゴリズムを利用したブロックとボールの衝突判定関数
void isCollideBlock(Ball *ball, Block *block){
    // ボールとブロックの衝突判定
    for (int i = 0; i < BLOCK_NUM; i++) {
        if (!block[i].isDestroyed) {
            // 中心座標を求める
            float x_m_block = block[i].x + (float)block[i].width/2;
            float y_m_block = block[i].y + (float)block[i].height/2;

            float x_m_ball = (*ball).x + (float)(*ball).width/2;
            float y_m_ball = (*ball).y + (float)(*ball).height/2;

            float x_d_all = (float)((*ball).width + block[i].width) / 2.0f; //2つの矩形が重なった時の中心x座標の差
            float y_d_all = (float)((*ball).height + block[i].height) / 2.0f; //2つの矩形が重なった時の中心y座標の差

            if (fabs(x_m_block - x_m_ball) < x_d_all && fabs(y_m_block - y_m_ball) < y_d_all) {
                block[i].isDestroyed = true;
                //ボールの速度を反転

                float x_overlap = x_d_all - fabs(x_m_block - x_m_ball);
                float y_overlap = y_d_all - fabs(y_m_block - y_m_ball);

                // 前フレームの中心座標を計算
                float prev_x_m_ball = x_m_ball - (*ball).dx;
                float prev_y_m_ball = y_m_ball - (*ball).dy;
                float prev_x_diff = fabs(x_m_block - prev_x_m_ball);
                float prev_y_diff = fabs(y_m_block - prev_y_m_ball);

                int was_x_separated = (prev_x_diff >= x_d_all);
                int was_y_separated = (prev_y_diff >= y_d_all);

                if (was_x_separated && !was_y_separated) {
                    // 前フレームはx軸で離れていた→x軸で衝突
                    (*ball).dx = -(*ball).dx;
                } else if (!was_x_separated && was_y_separated) {
                    // 前フレームはy軸で離れていた→y軸で衝突
                    (*ball).dy = -(*ball).dy;
                } else {
                    // 斜め進入や両方重なりの場合は最小overlapで判定
                    float x_overlap = x_d_all - fabs(x_m_block - x_m_ball);
                    float y_overlap = y_d_all - fabs(y_m_block - y_m_ball);
                    if (x_overlap < y_overlap) {
                        (*ball).dx = -(*ball).dx;
                    } else {
                        (*ball).dy = -(*ball).dy;
                    }
                }

            }
        }
    }
}