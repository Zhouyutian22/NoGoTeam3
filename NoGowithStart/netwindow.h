#ifndef NETWINDOW_H
#define NETWINDOW_H

#include <QMainWindow>
#include "networkdata.h"
#include "networkserver.h"
#include "networksocket.h"

namespace Ui {
class NetWindow;
}

class NetWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NetWindow(QWidget *parent = nullptr);
    ~NetWindow();
    // 服务端
    NetworkServer* server;
    // 客户端
    NetworkSocket* socket;

    //姓名、IP和端口
    QString MyName;
    QString HisName;
    QString IP;
    int PORT;
    //我方持棋颜色
    int MyColor;
    //关闭行为
    void closeEvent(QCloseEvent *event);

public slots:
    //服务端信息处理函数
    void receiveDataFromSocket(QTcpSocket* client, NetworkData data);
    //客户端信息处理函数
    void receiveDataFromServer(NetworkData data);
    //重设姓名、IP和端口
    void reset();
    //重启服务端
    void reStart();
    //重启客户端
    void reConnect();
signals:
    //窗口被关闭时，唤出主窗口
    void ReturnStart();
private:
    Ui::NetWindow *ui;
};

#endif // NETWINDOW_H
