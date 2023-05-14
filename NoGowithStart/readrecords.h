#ifndef READRECORDS_H
#define READRECORDS_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QTextEdit>
#include <QDebug>
namespace Ui {
class Readrecords;
}

class Readrecords : public QWidget
{
    Q_OBJECT

public:
    explicit Readrecords(QWidget *parent = nullptr);
    ~Readrecords();
    void closeEvent(QCloseEvent *);//返回上一层窗口
signals:
    void Returnrego();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Readrecords *ui;
};

#endif // READRECORDS_H
