/*  
    * This file contains the function declarations for the game.
    *
    * created by issei takeuchi on 2025/05/08
*/

#include "define.h"
#include "structs.h"
#include <stdbool.h>

//棒を動かす関数
void moveBar(struct Bar *bar, struct Ball *ball,char key){
    // TODO : スクリーン超過判定の検討
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
    }
}

/*
This is ASCII art of a block.
block has a width and height.

                --------  <- (x+width,y+height)
                |      |  
     (x,y) ->   --------  <- (x+width,y)
*/

void AABBJudge(struct Ball *ball, struct Block *block){
    //直線的と仮定すると，t0 = (P1 - P2)/vの時に衝突する．

    //左右に衝突する場合
    if ((*ball).dx > 0){
        double t_0 = ((*block).x - ((*ball).x + (*ball).radious)) / (*ball).dx;
    }else if ((*ball).dx < 0){
        double t_0 = ((*block).x + (*block).width - ((*ball).x - (*ball).radious)) / (*ball).dx;
    } 

    if(t_0 > 0 && t_0 < 1) {
        if(((*ball).y + (*ball).radious) + (*ball).dy * t_0 > (*block).y && ((*ball).y - (*ball).radious) + (*ball).dy * t_0 < (*block).y + (*block).height){
            //衝突した場合，ボールの速度を反転させる
            (*ball).dx = -(*ball).dx;
            //ブロックを壊す
            (*block).isDestroyed = true;
        }
    }

    //上下に衝突する場合
    if ((*ball).dy > 0){
        double t_0 = ((*block).y - ((*ball).y + (*ball).radious)) / (*ball).dy;
    }else if ((*ball).dy < 0){
        double t_0 = ((*block).y + (*block).height - ((*ball).y - (*ball).radious)) / (*ball).dy;
    }

    if(t_0 > 0 && t_0 < 1) {
        if(((*ball).x + (*ball).radious) + (*ball).dx * t_0 > (*block).x && ((*ball).x - (*ball).radious) + (*ball).dx * t_0 < (*block).x + (*block).width){
            //衝突した場合，ボールの速度を反転させる
            (*ball).dy = -(*ball).dy;
            //ブロックを壊す
            (*block).isDestroyed = true;
        }
    }
} 

//AABB衝突判定を利用したブロックとボールの衝突判定関数
void isCollideBlock(struct Ball *ball, struct Block *block, int BlockCount){
    // ボールとブロックの衝突判定
    for (int i = 0; i < BlockCount; i++) {
        if (!block[i].isDestroyed) {
            AABBJudge(ball, &block[i]);
        }
    }
}