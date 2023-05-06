//goChess棋子类的部分函数的定义
#include "gochess.h"

goChess::goChess()      //无参构造函数
{
    ;
}

goChess::goChess(QPoint point,bool color)   //含参构造函数
{
    c_point = point;
    w_color = color;
}
