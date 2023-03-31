//这个文件里声明了棋子类
#ifndef GOCHESS_H
#define GOCHESS_H


#include <QPoint>

class goChess
{
public:
    goChess();          //无参构造
    goChess(QPoint point,bool color);           //含参构造,下棋坐标和颜色（1黑0白）
    QPoint c_point;             //本棋子的坐标
    bool w_color;              //这一子是哪一方下的，黑or白
    bool judge(const goChess &c)        //判定某一子c是否与这一子完全相同
    {
        return (w_color==c.w_color && c_point==c.c_point);
    }
};

#endif // GOCHESS_H
