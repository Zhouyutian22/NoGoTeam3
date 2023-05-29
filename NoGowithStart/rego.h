#ifndef REGO_H
#define REGO_H

#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QTime>
#include <QTextEdit>
#include <QTimer>
#include <QRegularExpressionValidator>
#include "mainwindow.h"
namespace Ui {
class ReGo;
}

class ReGo : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReGo(QWidget *parent = nullptr);
    ~ReGo();
    void closeEvent(QCloseEvent *);
    //当前储存的总局数
    int sum;
    //复盘路数
    int road;
    //画棋盘
    void drawboard();
    //画棋子
    void drawchess();
    //画具
    void paintEvent(QPaintEvent *);
    //棋子坐标
    int ptx,pty;
    //模拟棋盘
    int board[13*13+3][2];
    //初始化
    void rego_init();
    //总步数
    int cnt;
    //当前进行的步数
    int step;
    //胜者
    bool winner;
    //播放模式开启
    bool show_modeon;
    //计时器
    QTimer timer;
    //判断变量
    bool flag;
    //到某步
    int to_which_step;
    void stopshow();
    //局面转移变量
    bool move;
signals:
    void ReturnRego();
    void Redraw();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    void continueshow();

    void on_lineEdit_editingFinished();

    void on_lineEdit_returnPressed();
    void on_pushButton_5_clicked();

public slots:
    //void del();

private:
    Ui::ReGo *ui;
};

#endif // REGO_H
