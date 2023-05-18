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
signals:
    void ReturnRego();
    void Redraw();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::ReGo *ui;
};

#endif // REGO_H
