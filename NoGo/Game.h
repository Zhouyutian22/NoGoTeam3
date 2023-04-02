#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <vector>
#include <QTimer>
#include "resultwidget.h"
/*
    @file Game.h
    @brief  这里定义了Game类，
            包含了游戏逻辑等。
    @time: 2023.03.21
*/


class Game : public QObject
{
    Q_OBJECT
public:
    //构造函数
    explicit Game(QObject *parent = nullptr);

signals:
    //认输，结束游戏
    void EndGame();
    //展示提醒 1黑棋赢，2白棋赢 3黑超时 4白超时 5黑棋自杀 6黑棋自杀，提醒界面要有再来一局。
    void ResultDisplaySignal(QString text);

//信号槽
public slots:
    //判断游戏胜负
    void judge();
    //判断是否超时
    void judgeTime();
    //设置时限
    void setTimeLimit(int Second);
    //...
    //游戏结果展示
    void ResultDisplay(QString text);
//一些游戏信息
public:
    //时间信号
    QTimer* Timer=new QTimer(this);
    //当前棋盘局面**
    int Board[10][10];
    //黑白玩家
    int PlayerBlack;
    int PlayerWhite;
    //倒计时时限 默认为30秒
    int TimeLimit=30;
    //当前一步的开始时间
    int StartTime;
    //当前的步数
    int StepCount;
    //当前的落子位置
    int CurrentPositionX;
    int CurrentPositionY;
private:

    //更改下棋方
    void ChangePlayer();
    //判断是否提子的函数
    int LibertyCheck(int ,int );
    //判断是否提子的函数
    void dfs(int ,int ,int );
    //判断该点是否在棋盘内
    bool InBoard(int ,int );
    //辅助的变量
    int checked[10][10];
    //辅助的变量
    int DirectionX[4]={0,1,0,-1};
    int DirectionY[4]={-1,0,1,0};
    //辅助的变量
    int flag;
    //结果窗口
    resultwidget* r;

};

#endif // GAME_H
