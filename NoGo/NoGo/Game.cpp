#include "Game.h"
#include <ctime>
#include <QTimer>
#include "Resultwidget.h" //结算界面 **

/*
    @file Game.cpp
    @brief  这里定义了Game类，
            包含了游戏逻辑等。
    @time: 2023.03.21
*/

Game::Game(QObject *parent) : QObject(parent)
{
    //初始化游戏
    memset(Board,0,sizeof(Board));
    PlayerBlack=1;
    PlayerWhite=0;
    StepCount=0;
    DirectionX[0]=DirectionY[0]=-1;
    DirectionX[1]=DirectionY[1]=0;
    DirectionX[2]=DirectionY[2]=1;
    DirectionX[3]=DirectionY[3]=0;
    //检测到Timeout信号时，触发一次judgeTime函数。
    connect(Timer, &QTimer::timeout, this, &Game::judgeTime);
}

//切换下棋者：在判断这一步棋不决出胜负之后才切换
void Game::ChangePlayer()
{
    if(PlayerBlack == 1)
    {
        PlayerWhite=1;
        PlayerBlack=0;
    }
    else
    {
        PlayerWhite=0;
        PlayerBlack=1;
    }
}
//游戏结束的出口
void Game::EndGame()
{
    ResultDisplaySignal("??");//Hint类型1是获胜提醒
    //还应该实现再来一局 ~w;
}

void Game::setTimeLimit(int Second)
{
    TimeLimit=Second;
}

//合法落子后判断输赢
void Game::judge()
{
    int x=CurrentPositionX;
    int y=CurrentPositionY;

    if(PlayerBlack) Board[x][y] = 1;
    if(PlayerWhite) Board[x][y] =-1;

    //0表示游戏继续，1表示落棋方吃子而输，2表示落棋方自杀而输
    switch (LibertyCheck(x,y)) {
    case 0:
        ChangePlayer();
        StartTime=clock();
        break;
    case 1:
        if(PlayerBlack) emit ResultDisplaySignal("WhiteWin");
        if(PlayerWhite) emit ResultDisplaySignal("BlackWin");
        break;
    case 2:
        if(PlayerBlack) emit ResultDisplaySignal("BlackKillself");
        if(PlayerWhite) emit ResultDisplaySignal("WhiteKillself");
    default:
        break;
    }

}
//辅助函数来判断是否围住了邻近对手棋子的气。
int Game::LibertyCheck(int x,int y)
{
    //0表示没有出事，1表示出事
    int flagKillHim=0;
    int flagKillMe=0;
    int MyColor=PlayerBlack?1:-1;

    flag=0;

    for(int i=0;i<4;i++)
    {
        int nx=x+DirectionX[i];
        int ny=y+DirectionY[i];
        //判断(x,y)的四周是否在棋盘内
        if(InBoard(nx,ny))
        {
            //重置checked数组和flag来准备做dfs，flag=0表示没有气，即被吃；flag=1表示有气，即没被吃。
            memset(checked,0,sizeof(checked));

            //如果该位置为空，则不再继续判断这个位置
            if(Board[nx][ny] == 0) continue;
            //如果该位置是敌方
            if(Board[nx][ny] != MyColor)
            {
                dfs(nx,ny,Board[nx][ny]);
                flagKillHim=!flag;
            }
            //如果该位置是自方
            if(Board[nx][ny] == MyColor)
            {
                dfs(nx,ny,Board[nx][ny]);
                flagKillMe=!flag;
            }
        }
    }
    //没有吃棋
    if(flagKillHim == 0 && flagKillMe == 0)
    {
        return 0;
    }
    if(flagKillHim == 1)
    {
        return 1;
    }
    if(flagKillHim == 0 && flagKillMe == 1)
    {
        return 2;
    }
}

//
void Game::dfs(int x,int y,int color)
{
    //避免重复检查
    checked[x][y]=1;
    for(int i=0;i<4;i++)
    {
        int nx=x+DirectionX[i];
        int ny=y+DirectionY[i];

        if(InBoard(nx,ny))
        {
            //避免重复检查
            if(checked[nx][ny]) continue;
            //如果有敌方棋子，无视
            if(Board[nx][ny] != color) continue;
            //如果有气,函数通过flag返回TRUE
            if(Board[nx][ny] == 0)
            {
                flag=1;
            }
            //如果有己方棋子，继续搜索
            if(Board[nx][ny] == color)
            {
                dfs(nx,ny,color);
            }
        }
    }
}
//判断(x,y)的四周是否在棋盘内
bool Game::InBoard(int x,int y)
{
    return (x>=1 && x<=9 && y>=1 && y<=9);
}

void Game::judgeTime()
{
    //超时情况
    if(clock()-StartTime > TimeLimit*1000)
    {
        if(PlayerBlack) emit ResultDisplaySignal("BlackOverTime");
        if(PlayerWhite) emit ResultDisplaySignal("WhiteOverTime");
    }

}
