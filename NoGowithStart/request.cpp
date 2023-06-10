#include "request.h"
#include "ui_request.h"

Request::Request(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Request)
{
    ui->setupUi(this);
}

Request::Request(QString name,QString color):
    QDialog(),
    ui(new Ui::Request)
{

    ui->setupUi(this);
    ui->PlayerInformation->append(" " + name+ " 想要执" + color +"棋与你对弈。");
    connect(ui->YesButton,&QPushButton::clicked,this,&Request::accpet);
    connect(ui->NoButton,&QPushButton::clicked,this,&Request::reject);
}

void Request::accpet()
{
    emit Ac();
}

void Request::reject()
{
    QString reason=ui->RejectEdit->text();
    emit Re(reason);
}
Request::~Request()
{
    delete ui;
}
