#include "regowindow.h"
#include "ui_regowindow.h"
#include "readrecords.h"
#include "rego.h"

RegoWindow::RegoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegoWindow)
{
    ui->setupUi(this);
}

RegoWindow::~RegoWindow()
{
    delete ui;
}

void RegoWindow::closeEvent(QCloseEvent *event)
{
    emit ReturnStart();
    delete this;
}

void RegoWindow::on_pushButton_clicked()
{
    this->hide();
    Readrecords *read = new Readrecords;
    read->show();

    connect(read,&Readrecords::Returnrego,this,&RegoWindow::show);
}


void RegoWindow::on_pushButton_2_clicked()
{
    this->hide();
    ReGo *re = new ReGo;
    re->show();

    connect(re,&ReGo::ReturnRego,this,&RegoWindow::show);
}

