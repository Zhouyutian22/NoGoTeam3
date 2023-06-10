#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <vector>
#include <QTimer>
#include "resultwidget.h"
#include <QTime>
#include <QDebug>
#include <QFile>
#include <QString>
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
    //结果展示的时候停止绘制棋盘
    void StopGo();
    void resetGo();
    void exitGo();
    void updateTime(int );
    //网络信号
    void Suicide(); //自杀信号
    void RivalTimeout(); //超时信号
    //AI信号
    void AIPosition(int ,int );
    void AIGiveup();
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
        //重置游戏
        void resetGame();
        //退出游戏
        void exitGame();

        //AI相关的函数

//一些游戏信息
public:
    //时间信号
    QTimer* Timer=new QTimer(this);
    //当前棋盘局面**
    int Board[14][14];
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
    //结果窗口
    resultwidget* r;
    //不可下棋处的探测
    void Assistant();
    //不可下棋处
    int helper[14][14];
    //判断窗口是否被关闭
    bool closed;
    //游戏总时长
    int TotalTime;
    int BeginTime;
    //是否要记录对局
    bool recmode;
    //联网判断
    bool online;
    int MyColor;
    /*
     *  游戏胜者
     *  0 白方正常赢
     *  1 黑方正常赢
     *  2 白方认输，黑方赢
     *  3 黑方认输，白方赢
     *  4 白方自杀，黑方赢
     *  5 黑方自杀，白方赢
     *  6 白方超时，黑方赢
     *  7 黑方超时，白方赢
     */
    int winner;
    //行棋中记录
    void go_write(bool,int,int,int);
    //对局开始时记录
    void game_init(void);
    //对局结束时记录
    void game_over(void);
    //本地游戏路数
    int road;


    //AI相关的函数等
    void AIGo();
    int AIPosx;
    int AIPosy;
    bool BlackBot;
    bool WhiteBot;
    int colorcheck(int ,int ,int);
    int evaluate(int );
    void greedy(int color);
    int value[14][14];
private:
    //文件记录棋谱以便复盘
    QFile f;
    //更改下棋方
    void ChangePlayer();
        //判断是否提子的函数
        int LibertyCheck(int ,int );
        //判断是否提子的函数
        void dfs(int ,int ,int );
        //判断该点是否在棋盘内
        bool InBoard(int ,int );
        //判断该点四周是否有子
        bool NeedCheck(int ,int );
        //辅助的变量
        int checked[14][14];
    //辅助的变量
    int DirectionX[4]={0,1,0,-1};
    int DirectionY[4]={-1,0,1,0};
    //辅助的变量
    int flag;


};

#endif // GAME_H
