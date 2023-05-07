#include "netwindow.h"
#include "ui_netwindow.h"
#include <QMessageBox>
NetWindow::NetWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetWindow)
{
    ui->setupUi(this);

    //设置IP和PORT
    IP="127.0.0.1";
    PORT=97531;

    this->ui->IPEdit->setText(IP);
    this->ui->PORTEdit->setText(QString::number(PORT));

    //创建服务端
    this->server=new NetworkServer(this);
    //服务端监听的信号槽
    connect(this->server,&NetworkServer::receive,this,&NetWindow::receiveDataFromSocket);
    //创建客户端
    this->socket = new NetworkSocket(new QTcpSocket(),this);
    //客户端收到信息
    connect(this->socket,&NetworkSocket::receive,this,&NetWindow::receiveDataFromServer);
    //客户端连接失败
    connect(socket->base(), &QAbstractSocket::disconnected, [=]() {
        QMessageBox::critical(this, tr("连接失败"), tr("连接被关闭了"));
    });

    //确认连接对方
    connect(ui->ResetButton,&QPushButton::clicked,this,&NetWindow::reset);
}

NetWindow::~NetWindow()
{
    delete ui;
}

//服务端的信息处理函数
void NetWindow::receiveDataFromSocket(QTcpSocket* client, NetworkData data)
{

}

//客户端的信息处理函数
void NetWindow::receiveDataFromServer(NetworkData data)
{

}

void NetWindow::reStart()
{
    qDebug()<<"restart the server.";

    disconnect(this->server,&NetworkServer::receive,this,&NetWindow::receiveDataFromSocket);
    delete this->server;
    this->server = new NetworkServer(this);

    this->server->listen(QHostAddress::Any,PORT);

    connect(this->server,&NetworkServer::receive,this,&NetWindow::receiveDataFromSocket);
}

void NetWindow::reConnect()
{
    qDebug()<<"client reconnect to the server.";

    this->socket->bye();
    this->socket->hello(IP,PORT);
    if(!this->socket->base()->waitForConnected(3000)){
        qDebug()<<"reconnect fail";
    }
}
//重设姓名、IP和端口并连接
void NetWindow::reset()
{
    MyName=ui->NameEdit->text();
    IP=ui->IPEdit->text();
    PORT=ui->PORTEdit->text().toInt();
    reStart();
    reConnect();
}

//关闭本窗口的事件
void NetWindow::closeEvent(QCloseEvent *event)
{
    emit ReturnStart();
    delete this;
}
