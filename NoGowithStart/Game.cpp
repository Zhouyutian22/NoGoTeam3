#include "Game.h"
#include "resultwidget.h"
#include <QDebug>
#include <ctime>
#include <QTimer>
/*
    @file Game.cpp
    @brief  这里定义了Game类，
            包含了游戏逻辑等。
    @time: 2023.03.21
*/

Game::Game(QObject *parent) : QObject(parent),f("localgames.txt")
{
    //初始化游戏
    memset(Board,0,sizeof(Board));
    memset(helper,0,sizeof(helper));
    PlayerBlack=1;
    PlayerWhite=0;
    StepCount=0;
    game_init();
    //检测到Timeout信号时，触发一次judgeTime函数。
    connect(Timer, &QTimer::timeout, this, &Game::judgeTime);
    //分出胜负时，展示结果界面
    connect(this,&Game::ResultDisplaySignal,this,&Game::ResultDisplay);

}

//切换下棋者
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
//再来一局
void Game::resetGame()
{
    game_init();
    memset(Board,0,sizeof(Board));
    memset(helper,0,sizeof(helper));
    PlayerBlack=1;
    PlayerWhite=0;
    StepCount=0;
    emit resetGo();
}
//退出游戏
void Game::exitGame()
{
    emit exitGo();
}
//弹出结果窗口
void Game::ResultDisplay(QString text)
{
    //停掉定时器
    Timer->stop();
    //弹出获胜信息窗口
    r=new resultwidget(text,nullptr);
    //设置再来一次的信号与槽
    connect(r,&resultwidget::StartNewGame,this,&Game::resetGame);
    //设置退出游戏的信号与槽
    connect(r,&resultwidget::ExitGame,this,&Game::exitGame);
    r->show();
    emit StopGo();

}
//设置时限
void Game::setTimeLimit(int Second)
{
    TimeLimit=Second;
}
//合法落子后判断输赢
void Game::judge()
{
    StepCount++;
    if(PlayerBlack)
    {
        go_write(1,CurrentPositionX,CurrentPositionY,StepCount);
    }
    if(PlayerWhite)
    {
        go_write(0,CurrentPositionX,CurrentPositionY,StepCount);
    }
    QString step=QString::number(StepCount,10);
    Timer->stop();
    Timer->start(200);
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
        Assistant();
        break;

    case 1:
        if(PlayerBlack)
        {
            winner = 0;
            game_over();
            emit ResultDisplaySignal("白棋方赢啦！    步数："+step);
        }
        if(PlayerWhite)
        {
            winner = 1;
            game_over();
            emit ResultDisplaySignal("黑棋方赢啦！    步数："+step);
        }
        //qDebug() << "case 1 emit";
        break;
    case 2:
        if(PlayerBlack)
        {
            winner = 5;
            game_over();
            emit ResultDisplaySignal("黑棋方自杀了。  步数：" + step + "\n白棋方赢啦！");
        }
        if(PlayerWhite)
        {
            winner = 4;
            game_over();
            emit ResultDisplaySignal("白棋方自杀了。  步数：" + step + "\n黑棋方赢啦！");
        }
        //qDebug() << "case 2 emit";
        break;
    default:
        break;
    }



}
//judge函数的辅助函数来判断是否围住了邻近对手棋子的气。
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
    //吃对方棋，无论是否自杀
    if(flagKillHim == 1)
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
//judge函数的辅助函数
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
//判断是否超时
void Game::judgeTime()
{
    //关闭窗口时不再计时
    if(closed) Timer->stop();
    //超时情况
    //qDebug() << "judgeTime " << (int)(0.001*(-1*clock()+StartTime+TimeLimit*1000)) ;
    emit updateTime((int)(0.001*(-1*clock()+StartTime+TimeLimit*1000)));
    if(clock()-StartTime > TimeLimit*1000)
    {
        Timer->stop();
        if(PlayerBlack)
        {
            winner = 7;
            game_over();
            emit ResultDisplaySignal("黑棋方超时了……\n白棋方赢啦！！！");
        }
        if(PlayerWhite)
        {
            winner = 6;
            game_over();
            emit ResultDisplaySignal("白棋方超时了……\n黑棋方赢啦！！！");
        }
    }

}
//可落子判断
void Game::Assistant()
{
    memset(helper,0,sizeof(helper));
    for(int i=1;i<10;i++)
        for(int j=1;j<10;j++)
        {
            if(Board[i][j] == 0)
            {
                Board[i][j]=PlayerBlack?1:-1;
                if(LibertyCheck(i,j)) helper[i][j]=1;
                Board[i][j]=0;
            }
        }
    qDebug() << StepCount;
    for(int i=1;i<10;i++)
    {
        qDebug() << helper[i][1] << " " << helper[i][2] << " " << helper[i][2] << " " << helper[i][2] << " " << helper[i][5] << " " << helper[i][6] << " " << helper[i][7] << " " << helper[i][8] << " " << helper[i][9];
    }
    qDebug() <<"----------------------";
}

void Game::game_init(void)
{
    if(!f.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        qDebug()<<"打开文件失败";
    }
    QTextStream out(&f);
    QString str = "Game at ";
    QTime time = QTime::currentTime();
    out<<'-'<<'\n';
    out<<str<<time.toString("hh:mm:ss t")<<'\n';

    f.close();
}

void Game::game_over(void)
{
    if(!f.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        qDebug()<<"打开文件失败";
    }
    QTextStream out(&f);
    QString str = "game is over";
    out<<str<<'\n';
    out<<"The result is below:"<<'\n';
    out<<"Total time: "<<TotalTime<<' '<<"Total steps: "<<StepCount<<'\n';
    out<<"Winner: ";

    if(winner == 0)
    {
        out<<"Playerwhite"<<'\n';
    }
    if(winner == 1)
    {
        out<<"Playerblack"<<'\n';
    }
    if(winner == 2)
    {
        out<<"Playerbalck G"<<'\n';
    }
    if(winner == 3)
    {
        out<<"Playerwhite G"<<'\n';
    }
    if(winner == 4)
    {
        out<<"Playerblack S"<<'\n';
    }
    if(winner == 5)
    {
        out<<"Playerwhite S"<<'\n';
    }
    if(winner == 6)
    {
        out<<"Playerblack T"<<'\n';
    }
    if(winner == 7)
    {
        out<<"Playerwhite T"<<'\n';
    }

    f.close();
}
//下每一步棋子的时候同时记录函数，第一个布尔值为当前颜色,黑1，中间xy值是当前落子处坐标，最后一个是当前步数
void Game::go_write(bool player,int cpx,int cpy,int cstep)
{
    if(!f.open(QIODevice::ReadWrite | QIODevice::Append))
    {
        qDebug()<<"打开文件失败";
    }
    QTextStream out(&f);
    if(player)
    {
        out<<cstep<<": "<<"Playerblack"<<' '<<(char)(cpx+64)<<cpy<<'\n';
    }
    else
    {
        out<<cstep<<": "<<"Playerwhite"<<' '<<(char)(cpx+64)<<cpy<<'\n';
    }
    f.close();
}

