#ifndef DEFINE_H
#define DEFINE_H

// input key definitions
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define UP_KEY 'w'
#define DOWN_KEY 's'
#define ESCAPE_KEY 0x1b // EscapeキーのASCIIコード

// bar speed definitions
#define MAX_BAR_SPEED 10
#define MIN_BAR_SPEED 1
#define BAR_SPEED_INCREMENT 1
#define BAR_SPEED_DECREMENT 1

// ball speed definitions
#define MAX_BALL_SPEED 3

// screen size
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 20

// block
#define BLOCK_NUM 4
#define BLOCK_WIDTH 10 //ブロックの幅
#define BLOCK_HEIGHT 3 //ブロックの高さ
#define BLOCK_ROWS 1 //ブロックの段数

//monitor mode
#define INIT_MODE 0
#define GAME_MODE 1
#define END_MODE 2
#define ESCAPE_MODE 3
#define COMPLETE_MODE 4

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