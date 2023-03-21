#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <vector>
#include <QTimer>

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
    //展示提醒 1-输赢 2-超时 ...
    void HintDisplay(int );

//信号槽
public slots:
    //判断游戏胜负
    void judge();
    //判断是否超时
    void judgeTime();
    //设置时限
    void setTimeLimit(int Second);
    //...

//一些游戏信息
public:
    //时间信号
    QTimer* Timer=new QTimer(this);
    //当前棋盘局面**
    int Board[9][9];
    //黑白玩家
    int PlayerBlack;
    int PlayerWhite;
    //倒计时秒数
    double TimeLimit;
    //当前一步的开始时间
    double StartTime;
    //当前的步数
    int StepCount;
private:
    //更改下棋方
    void ChangePlayer();
};

#endif // GAME_H
