//窗口类
#include "Game.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

#define ROWS 8
#define COLOMNS  8   //行数列数
#define WIDTH 30
#define HEIGHT   30       //棋盘格长宽
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(500,500);        //窗口大小
    current = 1;            //执黑先行
    chesses.clear();        //清空落子记录
    //连接信号
    connect(this,&MainWindow::StartJudge,game,&Game::judge);
}
void MainWindow::drawboard()
{
    QPainter painter(this);     //调用绘画工具
    QPen pencil(Qt::black);     //黑色画笔画棋盘
    pencil.setWidth(2);         //宽度为2

    painter.setPen(pencil);
    for(int r=1;r<=ROWS;r++)
    {
        for(int c=1;c<=COLOMNS;c++)
        {
            painter.drawRect((r+0.5)*WIDTH,(c+0.5)*HEIGHT,WIDTH,HEIGHT);
        }
    }
    //画棋盘使用drawRect方法
}

void MainWindow::drawchess()
{
    QPainter painter(this);
    QPen pencil(Qt::transparent);
    QBrush brush;
    painter.setPen(pencil);

    for(int i=0;i<chesses.size();i++)
    {
        goChess now = chesses[i];
        if(now.w_color == 1)
            painter.setBrush(Qt::black);
        else
            painter.setBrush(Qt::white);

        QPoint center((now.c_point.x()+0.5)*WIDTH,(now.c_point.y()+0.5)*HEIGHT);
        painter.drawEllipse(center,WIDTH/2,HEIGHT/2);
        //画棋子

    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint point;
    int ptx = event->pos().x();
    int pty = event->pos().y();  //找到鼠标点击点的x，y坐标
    //要判断是否在棋盘内
    if(ptx >= 1.5*WIDTH && ptx <= 1.5*WIDTH+COLOMNS*WIDTH && pty >= 1.5*HEIGHT && pty <= 1.5*HEIGHT+ROWS*HEIGHT)
    {
        point.setX(ptx/WIDTH);
        point.setY(pty/HEIGHT);     //设置点的坐标为点击处的网格点
    }
    else
    {
        return ;
    }
    for(int i=0;i<chesses.size();i++)
    {
        goChess m = chesses[i];
        if(m.c_point == point)
            return ;
    }
    //当前落子处不能已经有子
    goChess present(point,current);
    chesses.append(present);
    game->CurrentPositionX=ptx/WIDTH;
    game->CurrentPositionY=pty/HEIGHT;
    //这里留空，以便可以和棋局类对接，判定胜负以结束棋局
    emit StartJudge();
    //待测试!!
    current = !current;


}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    drawboard();
    drawchess();
    update();
}
