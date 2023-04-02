#include "Game.h"
#include "resultwidget.h"
#include <QDebug>
#include <ctime>
#include <QTimer>


//#include "Resultwidget.h" //结算界面 **

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



    //检测到Timeout信号时，触发一次judgeTime函数。
    connect(Timer, &QTimer::timeout, this, &Game::judgeTime);
    //分出胜负时，展示结果界面
    connect(this,&Game::ResultDisplaySignal,this,&Game::ResultDisplay);

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

void Game::resetGame()
{
    memset(Board,0,sizeof(Board));
    PlayerBlack=1;
    PlayerWhite=0;
    StepCount=0;
    emit resetGo();
}

void Game::ResultDisplay(QString text)
{
    //停掉定时器
    Timer->stop();
    //弹出获胜信息窗口
    r=new resultwidget(text,nullptr);
    //设置再来一次的信号与槽
    connect(r,&resultwidget::StartNewGame,this,&Game::resetGame);
    r->show();
    emit StopGo();

}

void Game::setTimeLimit(int Second)
{
    TimeLimit=Second;
}

//合法落子后判断输赢
void Game::judge()
{
    Timer->stop();
    Timer->start(1000);
    int x=CurrentPositionX;
    int y=CurrentPositionY;

    if(PlayerBlack) Board[x][y] = 1;
    if(PlayerWhite) Board[x][y] =-1;

    //0表示游戏继续，1表示落棋方吃子而输，2表示落棋方自杀而输
    switch (LibertyCheck(x,y)) {
    case 0:
        ChangePlayer();
        StartTime=clock();
        //qDebug() << "case 0 emit";
        break;

    case 1:
        if(PlayerBlack) emit ResultDisplaySignal("白棋方赢啦！！！");
        if(PlayerWhite) emit ResultDisplaySignal("黑棋方赢啦！！！");
        //qDebug() << "case 1 emit";
        break;
    case 2:
        if(PlayerBlack) emit ResultDisplaySignal("黑棋方自杀了……\n白棋方赢啦！！！");
        if(PlayerWhite) emit ResultDisplaySignal("白棋方自杀了……\n黑棋方赢啦！！！");
        //qDebug() << "case 2 emit";
        break;
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



    for(int i=0;i<4;i++)
    {
        flag=0;
        int nx=x+DirectionX[i];
        int ny=y+DirectionY[i];
        //判断(x,y)的四周是否在棋盘内
        if(InBoard(nx,ny))
        {
            //qDebug() << nx << " " << ny << "checked";
            //重置checked数组和flag来准备做dfs，flag=0表示没有气，即被吃；flag=1表示有气，即没被吃。
            memset(checked,0,sizeof(checked));

            //如果该位置为空，则不再继续判断这个位置
            if(Board[nx][ny] == 0) continue;

            //如果该位置是敌方，判断是否有吃掉敌方
            if(Board[nx][ny] != MyColor)
            {
                dfs(nx,ny,Board[nx][ny]);
                if(flagKillHim == 0) flagKillHim=!flag;
            }
            //如果该位置是自方，判断是否有吃掉己方
            if(Board[nx][ny] == MyColor)
            {
                dfs(nx,ny,Board[nx][ny]);
                if(flagKillMe == 0) flagKillMe=!flag;
            }
        }
    }
    //单独判断落子位置的气
    int singlecheck=0;
    for(int i=0;i<4;i++)
    {
        int nx=x+DirectionX[i];
        int ny=y+DirectionY[i];
        if(InBoard(nx,ny))
        {
            if(Board[nx][ny] != (-1)*MyColor) singlecheck=1;
        }
    }
    if(singlecheck == 0) flagKillMe=1;
    //没有吃棋
    if(flagKillHim == 0 && flagKillMe == 0)
    {
        //qDebug() << "case 0";
        return 0;
    }
    //吃对方棋
    if(flagKillHim == 1 && flagKillMe == 0)
    {
        //qDebug() << "case 1";
        return 1;
    }
    //自杀且只自杀
    if(flagKillHim == 0 && flagKillMe == 1)
    {
        //qDebug() << "case 2";
        return 2;
    }

    else return 0;
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
            if(Board[nx][ny] != color && Board[nx][ny] != 0) continue;
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
    //qDebug() << "judgeTime " << (int)(0.001*(-1*clock()+StartTime+TimeLimit*1000)) ;
    if(clock()-StartTime > TimeLimit*1000)
    {
        Timer->stop();
        if(PlayerBlack) emit ResultDisplaySignal("黑棋方超时了……\n白棋方赢啦！！！");
        if(PlayerWhite) emit ResultDisplaySignal("白棋方超时了……\n黑棋方赢啦！！！");
    }

}
