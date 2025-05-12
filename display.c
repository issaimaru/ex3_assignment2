/*  
    * デバック用エージェントを表示させるプログラム
    *
    * created by issei takeuchi on 2025/05/12
*/
#include <display.h>
#include <define.h>

void cleardisplay(){
    printf("\033[1J\033[1;1H");//画面左上からカーソル位置までクリアし、カーソルを1行1桁目に移動
}

void displayBar(Bar *bar) {

}