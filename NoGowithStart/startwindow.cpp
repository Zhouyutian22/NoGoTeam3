#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setWindowTitle("开始界面");
    //单机游戏PVP
    //connect(ui->SingleGame,&QPushButton::clicked,this,&StartWindow::SingleGame);
    connect(ui->NetGame,&QPushButton::clicked,this,&StartWindow::NetGame);

}

StartWindow::~StartWindow()
{
    delete ui;
}

/*void StartWindow::SingleGame()
{
    this->hide();
    MainWindow *m=new MainWindow;
    m->game->closed=0;
    m->show();

    connect(m,&MainWindow::ReturnStart,this,&StartWindow::show);
}*/


void StartWindow::NetGame()
{
    this->hide();
    NetWindow *n=new NetWindow;
    n->show();

    connect(n,&NetWindow::ReturnStart,this,&StartWindow::show);

}


void StartWindow::on_RecordPattern_clicked()
{
    this->hide();
    RegoWindow *r = new RegoWindow;
    r->show();

    connect(r,&RegoWindow::ReturnStart,this,&StartWindow::show);
}


void StartWindow::on_SingleGame_clicked()
{
    this->hide();
    ChooseRoadWindow *c = new ChooseRoadWindow;
    c->show();

    connect(c,&ChooseRoadWindow::ReturnStart,this,&StartWindow::show);
}

