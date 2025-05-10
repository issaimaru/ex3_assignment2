#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdbool.h>

typedef struct{
    int x;          // バーのX座標
    int y;          // バーのY座標
    int width;      // バーの幅
    int height;     // バーの高さ
    int speed;      // バーの移動速度
}Bar;

typedef struct{
    int x;          // ボールのX座標
    int y;          // ボールのY座標
    int radius;     // ボールの半径
    int dx;         // ボールのX方向の速度
    int dy;         // ボールのY方向の速度
}Ball;

typedef struct{
    int x;          // ブロックのX座標
    int y;          // ブロックのY座標
    int width;      // ブロックの幅
    int height;     // ブロックの高さ
    bool isDestroyed; // ブロックが壊れているかどうか
}Block;


#endif // STRUCTS_H