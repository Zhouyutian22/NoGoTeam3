#include "netwindow.h"
#include "ui_netwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include "request.h"

NetWindow::NetWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NetWindow)
{
    ui->setupUi(this);

    //设置IP和PORT
    IP="127.0.0.1";
    PORT=97531;

    //初始化参数
    onGame=false;
    MyName="me";
    HisName="unknown";
    MyColor=-1;         //初始化为白棋，体现谦让之风
    ServerorSocket=0;
    ConnectedOne=nullptr;
    SuicideSent=0;
    GiveUpSent=0;
    this->ui->IPEdit->setText(IP);
    this->ui->PORTEdit->setText(QString::number(PORT));

    //创建服务端
    this->server=new NetworkServer(this);
    //服务端监听的信号槽
    connect(this->server,&NetworkServer::receive,this,&NetWindow::receiveDataFromSocket);
    //开启监听（服务端模式）
    connect(ui->setPortButton,&QPushButton::clicked,this,&NetWindow::reStart);
    //仅重设姓名
    connect(ui->setNameButton,&QPushButton::clicked,this,&NetWindow::resetName);
    //创建客户端
    this->socket = new NetworkSocket(new QTcpSocket(),this);
    //客户端收到信息
    connect(this->socket,&NetworkSocket::receive,this,&NetWindow::receiveDataFromServer);
    //客户端发送请求
    connect(ui->RequestButton,&QPushButton::clicked,this,&NetWindow::SendRequest);
    //客户端连接失败
    connect(socket->base(), &QAbstractSocket::disconnected, [=]() {
        QMessageBox::critical(this, tr("网络提醒"), tr("连接被关闭了"));
    });
    //发送聊天信息
    connect(ui->ChatButton,&QPushButton::clicked,this,&NetWindow::SendChat);
    //确认连接对方
    connect(ui->ResetButton,&QPushButton::clicked,this,&NetWindow::reset);

    //修改请求执棋的颜色
    connect(ui->ColorButton,&QRadioButton::clicked,[=]() {
        if(ui->ColorButton->isChecked()) ui->ColorButton->setText("执黑棋");
        else ui->ColorButton->setText("执白棋");
    });
}

NetWindow::~NetWindow()
{
    delete ui;
}

//服务端的信息处理函数
void NetWindow::receiveDataFromSocket(QTcpSocket* client, NetworkData data)
{
    //仅连接一台机器
    if(ConnectedOne == nullptr)
    {
        qDebug() << "与" << client->peerAddress().toString()  <<"建立连接";
        ConnectedOne=client;
        //调整为服务端模式
        ServerorSocket=1;
        ui->StatusLabel->setText("Connected as Server");
    }
    else if(onGame)
    {
        this->server->send(client,NetworkData(OPCODE::CHAT_OP,"对不起，正在与另一台机器进行对局",NULL));
    }

    //接收数据
    switch (data.op) {
    case OPCODE::CHAT_OP:
        ui->ChatBox->append(HisName + " " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
        ui->ChatBox->append(data.data1);
        break;

    case OPCODE::READY_OP:
        if(!onGame) GameRequest(data.data1,data.data2);
        break;
    case OPCODE::MOVE_OP:
        CodetoNumber(data.data1);
        break;
    case OPCODE::SUICIDE_END_OP:
        if(SuicideSent)
        {

            win();
        }
        else if(!SuicideSent)
        {
            this->server->send(ConnectedOne,NetworkData(OPCODE::SUICIDE_END_OP,MyName,NULL));
            lose();
        }
        break;
    case OPCODE::GIVEUP_END_OP:
        if(GiveUpSent)
        {
            win();
        }
        else if(!GiveUpSent)
        {
            this->server->send(ConnectedOne,NetworkData(OPCODE::GIVEUP_END_OP,MyName,NULL));
            lose();
        }
        break;
    default:
        break;
    }
}

//客户端的信息处理函数
void NetWindow::receiveDataFromServer(NetworkData data)
{
    //接收数据
    switch (data.op) {
    case OPCODE::CHAT_OP:
        ui->ChatBox->append(HisName + " " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
        ui->ChatBox->append(data.data1);
        break;

    case OPCODE::READY_OP:

        if(!onGame)
        {
            HisName=data.data1;
            ui->StatusLabel->setText(HisName + " Accept");
            //调整自己的颜色
            if(ui->ColorButton->isChecked()) MyColor=1;
            else MyColor=-1;
            StartGame();
        }
        break;

    case OPCODE::REJECT_OP:
        ui->StatusLabel->setText(data.data1+" Reject "+data.data2);
        break;

    case OPCODE::MOVE_OP:
        CodetoNumber(data.data1);
        break;
    case OPCODE::SUICIDE_END_OP:
        if(SuicideSent)
        {

            win();
        }
        else if(!SuicideSent)
        {
            this->socket->send(NetworkData(OPCODE::SUICIDE_END_OP,MyName,NULL));
            lose();
        }
        break;
    case OPCODE::GIVEUP_END_OP:
        if(GiveUpSent)
        {
            win();
        }
        else if(!GiveUpSent)
        {
            this->socket->send(NetworkData(OPCODE::GIVEUP_END_OP,MyName,NULL));
            lose();
        }
        break;
    default:
        break;
    }
}

void NetWindow::reStart()
{
    qDebug()<<"restart the server.";
    PORT=ui->PORTEdit->text().toInt();
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
    //调整为客户端模式
    ServerorSocket=2;
    ui->StatusLabel->setText("Connected as Socket");
    this->socket->send(NetworkData(OPCODE::CHAT_OP,"hello!",NULL));
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
//重设姓名
void NetWindow::resetName()
{
    MyName=ui->NameEdit->text();
}
//发送聊天信息
void NetWindow::SendChat()
{

    switch (ServerorSocket) {
    //作为服务端发出信息
    case 1:
        this->server->send(ConnectedOne,NetworkData(OPCODE::CHAT_OP,ui->ChatEdit->text(),NULL));
        break;
    //作为客户端发出信息
    case 2:
        this->socket->send(NetworkData(OPCODE::CHAT_OP,ui->ChatEdit->text(),NULL));
        break;
    case 0:
        QMessageBox::information(this,"未建立连接","无法发送信息");
    default:
        break;
    }
    if(ServerorSocket)
    {
        ui->ChatBox->append(MyName +"(我) "+ QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
        ui->ChatBox->append(ui->ChatEdit->text());
    }
}
//发送对局请求
void NetWindow::SendRequest()
{
    if(!onGame)
    {
        ServerorSocket=2;
        QString color;
        if(ui->ColorButton->isChecked()) color='b';
        else color='w';
        this->socket->send(NetworkData(OPCODE::READY_OP,MyName,color));
        ui->StatusLabel->setText("Waiting for Reply");
    }
    else ui->StatusLabel->setText("On Game!No Request");
}
//处理对局请求
void NetWindow::GameRequest(QString Name,QString Color)
{
    QString RequestColor;
    if(Color == 'b')
    {
        RequestColor="黑";
        MyColor=-1;
    }
    else if(Color == 'w')
    {
        RequestColor="白";
        MyColor=1;
    }
    ServerorSocket=1;
    Request *r=new Request(Name,RequestColor);
    connect(r,&Request::Ac,this,&NetWindow::StartGame);
    connect(r,&Request::Re,this,&NetWindow::RejectGame);
    r->show();

}

//同意对局后开启游戏
void NetWindow::StartGame()
{
    onGame=true;
    if(ServerorSocket == 1) this->server->send(ConnectedOne,NetworkData(OPCODE::READY_OP,MyName,NULL));

    ui->StatusLabel->setText("On Game");
    //新建一个游戏窗口对象

    //新信号：Move(x,y)

    m=new MainWindow;
    connect(m,&MainWindow::Move,this,&NetWindow::Move);
    connect(m->game,&Game::Suicide,this,&NetWindow::Suicide);
    connect(m,&MainWindow::GiveUp,this,&NetWindow::GiveUp);
    m->NetMode=true;
    //全部设置一遍本方颜色
    m->MyColor=m->game->MyColor=MyColor;
    m->game->online=true;

    m->show();

}

void NetWindow::Move(int x,int y)
{
    //编码转换
    char *cd;


    QString code=QString(QLatin1String(cd));

    switch (ServerorSocket) {
    //作为服务端发出更新
    case 1:
        this->server->send(ConnectedOne,NetworkData(OPCODE::MOVE_OP,code,NULL));
        break;
    //作为客户端发出更新
    case 2:
        this->socket->send(NetworkData(OPCODE::MOVE_OP,code,NULL));
        break;
    default:
        break;
    }
}

void NetWindow::CodetoNumber(QString code)
{
    int y=code.at(0).toLatin1()-'A'+1;
    int x=code.at(1).toLatin1()-'0';

    m->PutChessOn(x,y);
}

//发送对方自杀信号
void NetWindow::Suicide()
{
    SuicideSent=1;
    switch (ServerorSocket) {
    //作为服务端发出更新
    case 1:

        this->server->send(ConnectedOne,NetworkData(OPCODE::SUICIDE_END_OP,MyName,NULL));
        break;
    //作为客户端发出更新
    case 2:

        this->socket->send(NetworkData(OPCODE::SUICIDE_END_OP,MyName,NULL));
        break;
    default:
        break;
    }
}
//发送对方投降信号
void NetWindow::GiveUp()
{
    GiveUpSent=1;
    switch (ServerorSocket) {
    //作为服务端发出更新
    case 1:
        this->server->send(ConnectedOne,NetworkData(OPCODE::GIVEUP_END_OP,MyName,NULL));
        break;
    //作为客户端发出更新
    case 2:
        this->socket->send(NetworkData(OPCODE::GIVEUP_END_OP,MyName,NULL));
        break;
    default:
        break;
    }
}

//胜利和失败
void NetWindow::win()
{
    QMessageBox::information(this,"你赢了","恭喜你赢了");

    onGame=0;
    SuicideSent=0;
    GiveUpSent=0;
    delete m;
}
void NetWindow::lose()
{
    QMessageBox::information(this,"你输了","很遗憾你输了");

    onGame=0;
    SuicideSent=0;
    GiveUpSent=0;
    delete m;
}
//拒绝对局后发送信息
void NetWindow::RejectGame(QString reason)
{
    this->server->send(ConnectedOne,NetworkData(OPCODE::REJECT_OP,MyName,reason));
}

//关闭本窗口的事件
void NetWindow::closeEvent(QCloseEvent *event)
{
    emit ReturnStart();
    delete this;
}
