#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct Bar{
    int x;          // バーのX座標
    int y;          // バーのY座標
    int width;      // バーの幅
    int speed;      // バーの移動速度
}Bar;

typedef struct Ball{
    int x;          // ボールのX座標
    int y;          // ボールのY座標
    int width;      // ボールの幅 
    int height;     // ボールの高さ （備考：ボールを円形とすることはTeraterm上でできない）
    int dx;         // ボールのX方向の速度
    int dy;         // ボールのY方向の速度
    int isGameOver; // ゲームオーバーかどうか
}Ball;

typedef struct Block{
    int x;          // ブロックのX座標
    int y;          // ブロックのY座標
    int width;      // ブロックの幅
    int height;     // ブロックの高さ
    int isNotDestroyed; // ブロックが壊れているかどうか
}Block;


#endif // STRUCTS_H