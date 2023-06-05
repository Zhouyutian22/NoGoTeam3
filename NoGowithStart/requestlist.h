#ifndef REQUESTLIST_H
#define REQUESTLIST_H

#include <QWidget>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include "request.h"
#include <QtNetwork>
#include <Qvector>
namespace Ui {
class RequestList;
}

class RequestList : public QWidget
{
    Q_OBJECT

public:
    explicit RequestList(QWidget *parent = nullptr);
    ~RequestList();

    QStandardItemModel *ItemModel;     //
    QStringList StringList;          //
    void init();
    void ShowRequest(QString );
    QVector<RequestData> r;
    void PutIn(RequestData r);

signals:
    void Rej(QString,QTcpSocket *);
    void Acc(QString,QString,QTcpSocket *);
public slots:
    void ShowClick(QModelIndex);
    void Accepted();
    void Rejected(QString reason);
private:
    int RequestIndex;
    Ui::RequestList *ui;
};

#endif // REQUESTLIST_H
