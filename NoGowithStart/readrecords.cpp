#include "readrecords.h"
#include "ui_readrecords.h"

Readrecords::Readrecords(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Readrecords)
{
    ui->setupUi(this);
}

Readrecords::~Readrecords()
{
    delete ui;
}

void Readrecords::closeEvent(QCloseEvent *e)
{
    emit Returnrego();
    delete this;
}

void Readrecords::on_pushButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,"open","mygames","TXT(*.txt)");
    if(!filepath.isEmpty())
    {
        QFile file(filepath);
        bool isopen = file.open(QIODevice::ReadOnly);
        if(isopen)
        {
            QByteArray array=file.readAll();
            ui->textEdit->setText(array);
        }
        else
        {
            qDebug()<<"open file failed";
        }
    }
    else
    {
        qDebug()<<"record file has not build yet";
    }
}


void Readrecords::on_pushButton_2_clicked()
{
    QFile f("mygames.txt");
    f.open(QIODevice::WriteOnly);
    f.close();
}

