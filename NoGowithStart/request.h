#ifndef REQUEST_H
#define REQUEST_H

#include <QDialog>

namespace Ui {
class Request;
}

class Request : public QDialog
{
    Q_OBJECT

public:
    explicit Request(QWidget *parent = nullptr);
    Request(QString ,QString );
    ~Request();
    Ui::Request *ui;
signals:
    void Ac();
    void Re(QString Reason);

public slots:
    void accpet();
    void reject();
private:

};

#endif // REQUEST_H
