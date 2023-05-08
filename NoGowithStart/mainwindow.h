#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Game.h"
#include <QMainWindow>
#include <QVector>
#include "gochess.h"
#include <iostream>
#include <fstream>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //游戏逻辑类
    Game *game=new Game;
    //棋局是否在进行
    bool Going;
    void paintEvent(QPaintEvent *event);    //绘画工具
    void drawboard(void);        //绘制棋盘函数
    void drawchess(void);       //绘制棋子函数
    void drawHint(void);            //绘制提示符
    void mousePressEvent (QMouseEvent *event);      //鼠标点击事件：落子
    void closeEvent(QCloseEvent *event);       //关闭事件
    QVector<goChess>chesses;    //      动态数组记录落子
    bool current;                   //当前落子颜色
    void setNewGame();            //再来一局的初始化
public slots:
    void StopGoing();             //游戏结束时停止行棋记录
    void setTimeLimit();          //设置倒计时时长
    void DisplayTime(int );       //展示倒计时

signals:
    //启动判断胜负的逻辑
    void StartJudge();
    //主动认输
    void GiveupSignal(QString );
    //窗口被关闭时，唤出主窗口
    void ReturnStart();
private slots:
    void on_pushButton_clicked();
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H