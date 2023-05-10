#ifndef NETWINDOW_H
#define NETWINDOW_H

#include <QMainWindow>
#include "networkdata.h"
#include "networkserver.h"
#include "networksocket.h"
#include "mainwindow.h"

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
    //目前是服务端还是客户端:服务端为1，客户端为2；
    int ServerorSocket;
    //作为服务端时，已连接机器的IP
    QTcpSocket* ConnectedOne;
    //关闭行为
    void closeEvent(QCloseEvent *event);
    //胜利和失败弹窗
    void win(int );
    void lose(int );

    //编码与棋盘转换
    void CodetoNumber(QString code);
public slots:
    //服务端信息处理函数
    void receiveDataFromSocket(QTcpSocket* client, NetworkData data);

    //客户端信息处理函数
    void receiveDataFromServer(NetworkData data);

    //重设姓名、IP和端口
    void reset();

    //重设姓名
    void resetName();

    //重启服务端
    void reStart();

    //重启客户端
    void reConnect();

    //发送聊天信息
    void SendChat();

    //发送对局请求
    void SendRequest();
    //同意对局并开始
    void StartGame();

    //拒绝对局
    void RejectGame(QString );

    //发送落子位置
    void Move(int ,int );

    //自杀结束
    void Suicide();
    //认输结束
    void GiveUp();
    //超时结束
    void Timeout();
    //退出
    void Left();
signals:
    //窗口被关闭时，唤出主窗口
    void ReturnStart();
private:
    //READY处理函数
    void GameRequest(QString ,QString );
    void Leave();
    //是否在对局
    bool onGame;
    //是否已发送投降、自杀信号
    bool SuicideSent;
    bool GiveUpSent;
    bool TimeoutSent;
    void init();
    //游戏窗口
    MainWindow * m;
    Ui::NetWindow *ui;
};

#endif // NETWINDOW_H
