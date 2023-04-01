#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QDialog>
#include <QTextBrowser>
#include "ui_resultwidget.h"
namespace Ui {
class resultwidget;
}

class resultwidget : public QDialog
{
    Q_OBJECT

public:
    explicit resultwidget(QString ,QWidget *parent = nullptr);
    ~resultwidget();
    void Display(QString );

private:
    Ui::resultwidget *ui;
};

#endif // RESULTWIDGET_H
