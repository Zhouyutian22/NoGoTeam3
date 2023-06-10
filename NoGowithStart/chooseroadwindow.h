#ifndef CHOOSEROADWINDOW_H
#define CHOOSEROADWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
namespace Ui {
class ChooseRoadWindow;
}

class ChooseRoadWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChooseRoadWindow(QWidget *parent = nullptr);
    ~ChooseRoadWindow();
    void closeEvent(QCloseEvent *);
signals:
    void ReturnStart();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::ChooseRoadWindow *ui;
};

#endif // CHOOSEROADWINDOW_H
