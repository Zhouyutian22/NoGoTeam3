#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "mainwindow.h"
#include "netwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StartWindow; }
QT_END_NAMESPACE

class StartWindow : public QMainWindow
{
    Q_OBJECT

signals:

public slots:
    void SingleGame();
    void NetGame();
public:

    StartWindow(QWidget *parent = nullptr);
    ~StartWindow();



private:
    Ui::StartWindow *ui;
};
#endif // STARTWINDOW_H
