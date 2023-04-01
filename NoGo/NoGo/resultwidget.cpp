#include "resultwidget.h"
#include "ui_resultwidget.h"

resultwidget::resultwidget(QString t,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resultwidget)
{
    ui->setupUi(this);
    ui->TextDisplay->setText(t);
    ui->TextDisplay->setStyleSheet("background-color: ;");

    QTextCursor cursor = ui->TextDisplay->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.setCharFormat(QTextCharFormat());  // 清除格式
    QTextCharFormat format;
    format.setForeground(Qt::white);
    format.setFontPointSize(20);
    cursor.setCharFormat(format);

}

resultwidget::~resultwidget()
{
    delete ui;
}


