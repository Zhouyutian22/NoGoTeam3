#include "giveup.h"
#include "ui_giveup.h"

giveup::giveup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::giveup)
{
    ui->setupUi(this);
}

giveup::~giveup()
{
    delete ui;
}

void giveup::on_label_linkActivated(const QString &link)
{
    ui->label->setText(link);
}


void giveup::on_pushButton_clicked()
{
    this->close();
}
