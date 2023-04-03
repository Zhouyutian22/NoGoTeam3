#include "resultwidget.h"
#include "ui_resultwidget.h"

resultwidget::resultwidget(QString t,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resultwidget)
{
    ui->setupUi(this);
    setWindowTitle("游戏结束~~");
    ui->TextDisplay->setText(t);


    //以下为获胜弹窗的文字格式与背景设定。
    ui->TextDisplay->setStyleSheet("background-color: lightgray;");
    QTextCursor cursor = ui->TextDisplay->textCursor();
    cursor.select(QTextCursor::Document);

    QTextCharFormat format;
    format.setForeground(Qt::white);
    format.setFontPointSize(20);
    cursor.setCharFormat(format);
    connect(ui->NewGame,&QPushButton::clicked,this,&resultwidget::EmitSignal);

}

resultwidget::~resultwidget()
{
    delete ui;
}


void resultwidget::EmitSignal()
{
    emit StartNewGame();
}
