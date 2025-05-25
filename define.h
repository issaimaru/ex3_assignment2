#ifndef DEFINE_H
#define DEFINE_H

// input key definitions
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define UP_KEY 'w'
#define DOWN_KEY 's'

// bar speed definitions
#define MAX_BAR_SPEED 10
#define MIN_BAR_SPEED 1
#define BAR_SPEED_INCREMENT 1
#define BAR_SPEED_DECREMENT 1

// screen size
#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 30

// block
#define BLOCK_NUM 30
#define BLOCK_WIDTH 2 //ブロックの幅
#define BLOCK_HEIGHT 3 //ブロックの高さ
#define BLOCK_ROWS 2 //ブロックの段数

#endif // DEFINE_H

/*
This is ASCII art of the game screen.
block has a width and height.

         --------------------------------------------------------------------------------------- <- (SCREEN_WIDTH,SCREEN_HEIGHT)
         |                --------  <- (x+width,y+height)                                      |
         |                |      |                                                             |
         |     (x,y) ->   --------  <- (x+width,y)                                             |   
         |                                                                                     |
         |                                                                                     |
         |                                                                                     |
         |                                                                                     |
         |                                                                                     |
         |                                                                                     |
         |                                                                                     |
         |                                                                                     |
         |                                                                                     |
         |                                                                                     |
         |                                                                                     |
         |                    (x,y) ->   --------  <- (x+width,y)                              |
(0,0) -> --------------------------------------------------------------------------------------- <- (SCLEEN_WIDTH,0)
*/