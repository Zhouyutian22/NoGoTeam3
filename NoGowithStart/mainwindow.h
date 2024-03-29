#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Game.h>
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
    void setName(QString ,QString );
    bool NetMode;                 //是否是网络游戏

    int MyColor;                  //1黑-1白

    bool flag;      //复盘的时候使用，只画一次


    //联网时，非本方的AI托管按钮隐藏
    void BotButtonHide();
public slots:
    void PutChessOn(int ,int );   //放置棋子
    void StopGoing();             //游戏结束时停止行棋记录
    void setTimeLimit();          //设置倒计时时长
    void DisplayTime(int );       //展示倒计时

signals:
    //启动判断胜负的逻辑
    void StartJudge();
    //主动认输
    void GiveupSignal(QString );
    //窗口被关闭时，唤出选择路数窗口
    void ReturnChoose();

    //联机相关
    //更新棋盘给NetWindow
    void Move(int ,int );
    void GiveUp();   //认输信号
    //联机游戏窗口被人为关闭，则发送leave信号
    void CloseByPerson();

private slots:
    void on_pushButton_clicked();
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
