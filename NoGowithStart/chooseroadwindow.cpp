#include "chooseroadwindow.h"
#include "ui_chooseroadwindow.h"

ChooseRoadWindow::ChooseRoadWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseRoadWindow)
{
    ui->setupUi(this);
}

ChooseRoadWindow::~ChooseRoadWindow()
{
    delete ui;
}

void ChooseRoadWindow::closeEvent(QCloseEvent *e)
{
    emit ReturnStart();
    delete this;
}

void ChooseRoadWindow::on_pushButton_clicked()      //9路
{
    this->hide();           //隐藏上级窗口
    MainWindow *m=new MainWindow;
    m->game->closed=0;
    m->game->road = 9;
    m->game->game_init();       //  以上几行初始化游戏和游戏窗口
    m->show();                  //显示

    connect(m,&MainWindow::ReturnChoose,this,&ChooseRoadWindow::show);      //槽，用于重新显示上级窗口
}

//方式同9路
void ChooseRoadWindow::on_pushButton_2_clicked()        //11路
{
    this->hide();
    MainWindow *m=new MainWindow;
    m->game->closed=0;
    m->game->road = 11;
    m->game->game_init();
    m->show();

    connect(m,&MainWindow::ReturnChoose,this,&ChooseRoadWindow::show);
}


void ChooseRoadWindow::on_pushButton_3_clicked()        //13路
{
    this->hide();
    MainWindow *m=new MainWindow;
    m->game->closed=0;
    m->game->road = 13;
    m->game->game_init();
    m->show();

    connect(m,&MainWindow::ReturnChoose,this,&ChooseRoadWindow::show);
}

