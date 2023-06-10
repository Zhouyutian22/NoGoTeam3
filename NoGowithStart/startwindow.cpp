#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setWindowTitle("开始界面");

    connect(ui->NetGame,&QPushButton::clicked,this,&StartWindow::NetGame);

}

StartWindow::~StartWindow()
{
    delete ui;
}


//创建新的联机游戏
void StartWindow::NetGame()
{
    this->hide();
    NetWindow *n=new NetWindow;
    n->show();

    connect(n,&NetWindow::ReturnStart,this,&StartWindow::show);

}

//创建新的复盘模式
void StartWindow::on_RecordPattern_clicked()
{
    this->hide();
    RegoWindow *r = new RegoWindow;
    r->show();

    connect(r,&RegoWindow::ReturnStart,this,&StartWindow::show);
}

//创建新的单机游戏
void StartWindow::on_SingleGame_clicked()
{
    this->hide();
    ChooseRoadWindow *c = new ChooseRoadWindow;
    c->show();

    connect(c,&ChooseRoadWindow::ReturnStart,this,&StartWindow::show);
}

