#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setWindowTitle("开始界面");
    //单机游戏PVP
    connect(ui->SingleGame,&QPushButton::clicked,this,&StartWindow::SingleGame);

}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::SingleGame()
{
    this->hide();
    MainWindow *m=new MainWindow;
    m->game->closed=0;
    m->show();

    connect(m,&MainWindow::ReturnStart,this,&StartWindow::show);
}



