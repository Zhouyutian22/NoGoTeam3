#ifndef REGOWINDOW_H
#define REGOWINDOW_H

#include <QMainWindow>

namespace Ui {
class RegoWindow;
}

class RegoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegoWindow(QWidget *parent = nullptr);
    ~RegoWindow();
    //关闭行为
    void closeEvent(QCloseEvent *event);

signals:
    //窗口被关闭时，唤出主窗口
    void ReturnStart();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::RegoWindow *ui;
};

#endif // REGOWINDOW_H
