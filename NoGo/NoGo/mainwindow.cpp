//窗口类
#include "Game.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
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
    setWindowTitle("不围棋");
    resize(16.99*WIDTH,10.5*HEIGHT);        //窗口大小
    current = 1;            //执黑先行
    Going=true;             //正在下棋
    chesses.clear();        //清空落子记录

    //游戏结束时停止棋盘更新
    connect(game,&Game::StopGo,this,&MainWindow::StopGoing);
    //退出游戏的信号
    connect(game,&Game::exitGo,this,&MainWindow::close);
    //胜负判断的信号
    connect(this,&MainWindow::StartJudge,game,&Game::judge);
    //重置游戏的信号
    connect(game,&Game::resetGo,this,&MainWindow::setNewGame);
    //主动认输的信号
    connect(this,&MainWindow::GiveupSignal,game,&Game::ResultDisplay);
    //修改时限的信号
    connect(ui->setTimeButton,&QPushButton::clicked,this,&MainWindow::setTimeLimit);
    //更新时间展示的信号
    connect(game,&Game::updateTime,this,&MainWindow::DisplayTime);
}
MainWindow::~MainWindow()
{
    delete ui;
}
//画棋盘
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
//画棋子
void MainWindow::drawchess()
{
    QPainter painter(this);
    QPen pencil(Qt::transparent);
    //QBrush brush;       可以为棋盘涂上喜欢的底色，暂时无用
    painter.setPen(pencil);

    for(int i=0;i<chesses.size();i++)
    {
        goChess now = chesses[i];
        if(now.w_color == 1)
            painter.setBrush(Qt::black);
        else
            painter.setBrush(Qt::white);

        QPoint center((now.c_point.x()+0.5)*WIDTH,(now.c_point.y()+0.5)*HEIGHT);
        //高亮最近一步
        if(i != chesses.size() -1 )
            painter.drawEllipse(center,WIDTH/2,HEIGHT/2);
        else
            painter.drawEllipse(center,WIDTH*2/3,HEIGHT*2/3);
        //画棋子

        //在认输框右侧显示现在的落子方
        if(now.w_color == 1)
            painter.setBrush(Qt::white);
        else
            painter.setBrush(Qt::black);
        QPoint HintPlayer(14.5*WIDTH,1.85*HEIGHT);
        painter.drawEllipse(HintPlayer,WIDTH/2,HEIGHT/2);
    }
}
//鼠标点击落子
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(Going) //如果棋局在进行
    {
        QPoint point;
        int ptx = event->pos().x();
        int pty = event->pos().y();  //找到鼠标点击点的x，y坐标
        //判断点击位置是否在棋盘内
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
        //向game类传递落子位置
        game->CurrentPositionX=ptx/WIDTH;
        game->CurrentPositionY=pty/HEIGHT;
        //启动判断胜负
        emit StartJudge();

        current = !current;
    }
}
//绘制棋盘和棋子的动作
void MainWindow::paintEvent(QPaintEvent *)
{
        drawboard();
        drawchess();
        drawHint();
        update();
}
//停止下棋，使得绘制棋子、逻辑判断停止
void MainWindow::StopGoing()
{
    Going=false;
}
//再来一局
void MainWindow::setNewGame()
{
    current = 1;            //执黑先行
    Going=true;             //正在下棋
    chesses.clear();        //清空落子记录
    ui->TimeDisplay->clear();
}
//认输按钮触发
void MainWindow::on_pushButton_clicked()
{
    if(Going)
    {
        if(current==1)
        {
            emit GiveupSignal("黑棋方认输……\n白棋方赢啦！！！");
        }
        else
        {
            emit GiveupSignal("白棋方认输……\n黑棋方赢啦！！！");
        }
    }
}
//更新倒计时
void MainWindow::DisplayTime(int Time)
{
    if(Time>=0)
    {
        QString TimeString=QString::number(Time,10);
        ui->TimeDisplay->setText(TimeString+"s");
    }
}
//修改时限
void MainWindow::setTimeLimit()
{
    QString TimeFigure=ui->setTimeLimit->toPlainText();
    bool Legal;
    int TimeLimit=TimeFigure.toInt(&Legal);
    if(Legal && TimeLimit > 0)
    {
        game->setTimeLimit(TimeLimit);
    }
    else
    {
        ui->setTimeLimit->clear();
        ui->setTimeLimit->append("输入无效");
    }
}

//禁止落子提示
void MainWindow::drawHint()
{
    QPainter painter(this);
    QPen pencil(Qt::transparent);
    painter.setPen(pencil);
    painter.setBrush(Qt::red);
    for(int i=1;i<10;i++)
        for(int j=1;j<10;j++)
        {
            if(game->helper[i][j] == 1)
            {
                QPoint center((i+0.5)*WIDTH,(j+0.5)*HEIGHT);
                painter.drawEllipse(center,WIDTH/4,HEIGHT/4);
            }
        }
}
