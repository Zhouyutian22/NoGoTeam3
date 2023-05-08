#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QDialog>
#include <QTextBrowser>
//#include "ui_resultwidget.h"
namespace Ui {
class resultwidget;
}

class resultwidget : public QDialog
{
    Q_OBJECT

signals:
    void StartNewGame();//向Game发出重置游戏的信号
    void ExitGame();
public slots:
    void EmitSignal();     //接收本窗口的信号。
    void Exit();
public:
    explicit resultwidget(QString ,QWidget *parent = nullptr);

    ~resultwidget();

private:
    Ui::resultwidget *ui;
};

#endif // RESULTWIDGET_H
