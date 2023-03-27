
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "gochess.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent *event);    //绘画工具
    void drawboard(void);        //绘制棋盘函数
    void drawchess(void);       //绘制棋子函数
    void mousePressEvent (QMouseEvent *event);      //鼠标点击事件：落子
    QVector<goChess>chesses;    //      动态数组记录落子
    bool current;                   //当前落子颜色
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H