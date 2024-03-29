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
    current = 1;            //执黑先行
    Going=true;             //正在下棋
    chesses.clear();        //清空落子记录
    NetMode=0;              //默认为单机游戏
    DisplayTime(game->TimeLimit);
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

    //AI相关
    connect(game,&Game::AIPosition,this,&MainWindow::PutChessOn);
    connect(game,&Game::AIGiveup,this,&MainWindow::on_pushButton_clicked);
    connect(ui->BlackBot,&QCheckBox::clicked,this,[&](){
        //托管开关-黑
        if(ui->BlackBot->isChecked()) game->BlackBot=1;
        else game->BlackBot=0;
    });
    connect(ui->WhiteBot,&QCheckBox::clicked,this,[&](){
        //托管开关-白
        if(ui->WhiteBot->isChecked()) game->WhiteBot=1;
        else game->WhiteBot=0;
    });
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
    for(int r=1;r<=(game->road)-1;r++)
        for(int c=1;c<=(game->road)-1;c++)
            painter.drawRect((r+0.5)*WIDTH,(c+0.5)*HEIGHT,WIDTH,HEIGHT);
    //画棋盘使用drawRect方法
}
//画棋子
void MainWindow::drawchess()
{
    QPainter painter(this);
    QPen pencil(Qt::transparent);
    painter.setPen(pencil);
    //遍历绘制棋子
    for(int i=0;i<chesses.size();i++)
    {
        goChess now = chesses[i];
        if(now.w_color == 1) painter.setBrush(Qt::black);
        else painter.setBrush(Qt::white);
        QPoint center((now.c_point.x()+0.5)*WIDTH,(now.c_point.y()+0.5)*HEIGHT);
        //高亮最近二步
        if(i < chesses.size() -2 ) painter.drawEllipse(center,WIDTH/2,HEIGHT/2);
        else painter.drawEllipse(center,WIDTH*3/5,HEIGHT*3/5);
        //在认输框右侧显示现在的落子方
        if(now.w_color == 1) painter.setBrush(Qt::white);
        else painter.setBrush(Qt::black);
        QPoint HintPlayer(14.5*WIDTH,1.85*HEIGHT);
        painter.drawEllipse(HintPlayer,WIDTH/2,HEIGHT/2);
        //显示步数
        ui->StepDisplay->display(game->StepCount);
    }
}
//该函数同时处理鼠标点击落子、MOVE_OP落子、AI落子。
void MainWindow::PutChessOn(int x,int y)
{
    //设置点
    QPoint point;
    point.setX(x);
    point.setY(y);
    //插入点
    goChess present(point,current);
    chesses.append(present);
    //向game类传递落子位置
    game->CurrentPositionX=x;
    game->CurrentPositionY=y;
    //如果是联机己方下棋的情况则发送MOVE_OP信号
    if(Going && NetMode &&((MyColor == 1 && current == 1) || (MyColor == -1 && current == 0))) emit Move(x,y);
    //启动判断胜负
    emit StartJudge();
    qDebug() << "落子判断完毕，位置：" << x << " " << y;
    DisplayTime(game->TimeLimit);
    current = !current;
}

//鼠标点击落子
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(!Going) return;
    //获取该位置
    QPoint point;
    int ptx = event->pos().x();
    int pty = event->pos().y();  //找到鼠标点击点的x，y坐标
    //判断点击位置是否在棋盘内
    if(ptx >= 1.5*WIDTH && ptx <= 1.5*WIDTH+((game->road)-1)*WIDTH && pty >= 1.5*HEIGHT && pty <= 1.5*HEIGHT+((game->road)-1)*HEIGHT)
    {
        point.setX(ptx/WIDTH);
        point.setY(pty/HEIGHT);     //设置点的坐标为点击处的网格点
    }
    else
    {
        return ;
    }
    //判断是否已经有棋
    for(int i=0;i<chesses.size();i++)
    {
        goChess m = chesses[i];
        if(m.c_point == point)
            return ;
    }
    if(!NetMode && ((current == 1 && game->BlackBot == 0) ||(current == 0 && game->WhiteBot == 0))) //单机模式的手动下棋
    {

        PutChessOn(ptx/WIDTH,pty/HEIGHT);

    }
    else if(NetMode && game->BlackBot == 0 && game->WhiteBot == 0) //联网模式的手动下棋
    {
        if((MyColor == 1 && current == 1) || (MyColor == -1 && current == 0))
        {
            //己方放棋
            PutChessOn(ptx/WIDTH,pty/HEIGHT);
        }
    }
}
//绘制棋盘和棋子的动作
void MainWindow::paintEvent(QPaintEvent *)
{
    if(game->recmode == 1)
    {
        drawboard();
        drawchess();
        drawHint();
        update();
    }
    else
    {
        if(flag)
        {
            QPainter painter(this);
            QPen pencil(Qt::transparent);
            painter.setPen(pencil);
            bool color = 1;
            for(int t = 0;t < chesses.size();t++)
            {
                if(!color)
                    painter.setBrush(Qt::white);
                else
                    painter.setBrush(Qt::black);

                QPoint center((chesses[t].c_point.x()+0.5)*WIDTH,(chesses[t].c_point.y()+0.5)*HEIGHT);
                painter.drawEllipse(center,WIDTH/2,HEIGHT/2);

                color = !color;
            }
            update();
            flag = 0;
        }
        else
        {
            drawboard();
            drawchess();
            drawHint();
            update();
        }
    }
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
    DisplayTime(game->TimeLimit);
    ui->StepDisplay->display(0);
    game->Assistant();

}
//认输按钮触发
void MainWindow::on_pushButton_clicked()
{
    if(!NetMode)
    {
        if(game->StepCount == 0)
        {
            QMessageBox::critical(this, "提示", "游戏还没有开始~");
            return;
        }
        if(Going)
        {
            if(current==1)
            {
                if(game->BlackBot == 1) return;
                game->winner = 3;
                game->game_over();
                emit GiveupSignal("黑棋方认输。   步数：" +QString::number(game->StepCount,10) +"\n白棋方赢啦！！！");
            }
            else
            {
                if(game->WhiteBot == 1) return;
                game->winner = 2;
                game->game_over();
                emit GiveupSignal("白棋方认输。   步数：" +QString::number(game->StepCount,10) +"\n黑棋方赢啦！！！");
            }
        }
    }

    if(NetMode)
    {
        if((MyColor == 1 && current) || (MyColor == -1 && !current))
            emit GiveUp();
        game->closed=1;
    }
}
//更新倒计时
void MainWindow::DisplayTime(int Time)
{
    QPalette lcdpat = ui->TimeDisplay->palette();
    lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::black);
    ui->TimeDisplay->setPalette(lcdpat);
    if(Time>=0)
    {
        int minute=Time/60;
        QString Min=QString::number(minute,10);

        Time%=60;
        QString Second=QString::number(Time,10);
        if(Time < 10) Second="0" + Second;
        if(minute != 0 && minute >=10 ) ui->TimeDisplay->display(Min + ":" + Second);
        else if(minute == 0) ui->TimeDisplay->display("00:" + Second);
        else if(minute < 10) ui->TimeDisplay->display("0"+Min+":"+Second);

        if(minute == 0 && Time <=5 )
        {
            lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
            ui->TimeDisplay->setPalette(lcdpat);
        }
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
        if(game->StepCount == 0) DisplayTime(TimeLimit);
        game->setTimeLimit(TimeLimit);
    }
    else
    {
        ui->setTimeLimit->clear();
        ui->setTimeLimit->append(QString::number(game->TimeLimit,10));
        QMessageBox::critical(this, "提示", "输入无效");
    }
}
//禁止落子提示
void MainWindow::drawHint()
{
    QPainter painter(this);
    QPen pencil(Qt::transparent);
    painter.setPen(pencil);
    painter.setBrush(Qt::red);
    for(int i=1;i<=(game->road);i++)
        for(int j=1;j<=(game->road);j++)
        {
            if(game->helper[i][j] == 1)
            {
                QPoint center((i+0.5)*WIDTH,(j+0.5)*HEIGHT);
                painter.drawEllipse(center,WIDTH/4,HEIGHT/4);
            }
        }
}
//关闭游戏界面时触发
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!NetMode)
    {
        emit ReturnChoose();
        game->closed=true;
        delete this;
    }
    else if(NetMode)
    {
        emit CloseByPerson();
    }
}
//设置玩家信息
void MainWindow::setName(QString a,QString b)
{
    ui->BlackName->setText("黑棋："+ a);
    ui->WhiteName->setText("白棋："+ b);
}
//联机模式下，隐藏对方颜色的托管开关
void MainWindow::BotButtonHide()
{
    if(MyColor == 1)        ui->WhiteBot->hide();
    else if(MyColor == -1)  ui->BlackBot->hide();
}
