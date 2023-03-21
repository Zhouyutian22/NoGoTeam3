#include "Game.h"
#include <ctime>
#include <QTimer>


/*
    @file Game.h
    @brief  这里定义了Game类，
            包含了游戏逻辑等。
    @time: 2023.03.21
*/

Game::Game(QObject *parent) : QObject(parent)
{

}

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
void Game::EndGame()
{

}