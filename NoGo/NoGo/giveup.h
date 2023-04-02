#ifndef GIVEUP_H
#define GIVEUP_H

#include <QWidget>

namespace Ui {
class giveup;
}

class giveup : public QWidget
{
    Q_OBJECT

public:
    explicit giveup(QWidget *parent = nullptr);
    ~giveup();

public slots:
    void on_label_linkActivated(const QString &link);

private slots:
    void on_pushButton_clicked();

private:
    Ui::giveup *ui;
};

#endif // GIVEUP_H
