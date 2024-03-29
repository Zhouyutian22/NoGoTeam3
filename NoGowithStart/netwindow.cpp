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
    setWindowTitle("不围棋（联机版）");
    //设置IP和PORT
    IP="10.46.183.98";
    PORT=30303;

    //初始化参数
    MyName="me";
    HisName="unknown";
    MyColor=-1;         //初始化为白棋，体现谦让之风
    ServerorSocket=0;
    ConnectedOne=nullptr;
    //列表邀请的内容
    rt=new RequestList();
    Requestcnt=0;
    connect(rt,&RequestList::Acc,this,&NetWindow::AcceptOne);
    connect(rt,&RequestList::Rej,this,&NetWindow::RejectOne);
    //
    init();
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
    //离开按钮
    connect(ui->LeaveButton,&QPushButton::clicked,this,&NetWindow::Left);
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
    //仅连接一台机器(暂时作废)
    /*if(ConnectedOne == nullptr)
    {
        ui->StatusLabel->setText("Connected as Server with"+client->peerAddress().toString());
    }
    else */
    if(onGame && ConnectedOne!=client)
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
        if(!onGame)
        {
            ConnectedOne=client;
            ui->StatusLabel->setText("new request");
            HisName=data.data1;
            GameRequest(data.data1,data.data2);
        }
        break;
    case OPCODE::MOVE_OP:
        CodetoNumber(data.data1);
        break;
    case OPCODE::SUICIDE_END_OP:
        if(SuicideSent)
        {

            win(1);
        }
        else if(!SuicideSent)
        {
            this->server->send(ConnectedOne,NetworkData(OPCODE::SUICIDE_END_OP,MyName,NULL));
            lose(1);
        }
        break;
    case OPCODE::GIVEUP_OP:
                                                                                                                    //(停止游戏)
        this->server->send(ConnectedOne,NetworkData(OPCODE::GIVEUP_END_OP,MyName,NULL));
        break;
    case OPCODE::GIVEUP_END_OP:
        if(GiveUpSent)
        {
            this->server->send(ConnectedOne,NetworkData(OPCODE::GIVEUP_END_OP,MyName,NULL));
            lose(2);
        }
        else if(!GiveUpSent)
        {

            win(2);
        }
        break;
    case OPCODE::TIMEOUT_END_OP:
        if(TimeoutSent)
        {
            win(3);
        }
        else if(!TimeoutSent)
        {
            this->server->send(ConnectedOne,NetworkData(OPCODE::TIMEOUT_END_OP,MyName,NULL));
            lose(3);
        }
        break;
    case OPCODE::LEAVE_OP:
        if(ConnectedOne != client) break;
        ui->StatusLabel->setText(HisName+" Left");
        QMessageBox::information(this,"离开信息","对方离开了");
        Leave();
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
        HisName=data.data1;
        ui->StatusLabel->setText(data.data1+" Reject "+data.data2);
        break;

    case OPCODE::MOVE_OP:
        CodetoNumber(data.data1);
        break;
    case OPCODE::SUICIDE_END_OP:
        if(SuicideSent)
        {

            win(1);
        }
        else if(!SuicideSent)
        {
            this->socket->send(NetworkData(OPCODE::SUICIDE_END_OP,MyName,NULL));
            lose(1);
        }
        break;
    case OPCODE::GIVEUP_OP:
        //(停止游戏)
        this->socket->send(NetworkData(OPCODE::GIVEUP_END_OP,MyName,NULL));
        break;
    case OPCODE::GIVEUP_END_OP:
        if(GiveUpSent)
        {
            this->socket->send(NetworkData(OPCODE::GIVEUP_END_OP,MyName,NULL));
            lose(2);
        }
        else if(!GiveUpSent)
        {

            win(2);
        }
        break;
    case OPCODE::TIMEOUT_END_OP:
        if(TimeoutSent)
        {
            win(3);
        }
        else if(!TimeoutSent)
        {
            this->socket->send(NetworkData(OPCODE::TIMEOUT_END_OP,MyName,NULL));
            lose(3);
        }
        break;
    case OPCODE::LEAVE_OP:
        ui->StatusLabel->setText(HisName+" Left");
        QMessageBox::information(this,"离开信息","对方离开了");
        this->socket->bye();
        Leave();
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
        ui->StatusLabel->setText("Connected fail");
    }
    else
    {
        //调整为客户端模式
        ServerorSocket=2;
        ui->StatusLabel->setText("Connected as Socket");
    }
}
//重设姓名、IP和端口并连接
void NetWindow::reset()
{
    MyName=ui->NameEdit->text();
    IP=ui->IPEdit->text();
    PORT=ui->PORTEdit->text().toInt();
    //reStart();
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
        ui->ChatEdit->clear();
    }
}
//发送对局请求
void NetWindow::SendRequest()
{
    if(ServerorSocket == 1) ui->StatusLabel->setText("No Connected one");
    else if(!onGame)
    {
        ServerorSocket=2;
        QString color;
        if(ui->ColorButton->isChecked()) color='b';
        else color='w';
        this->socket->send(NetworkData(OPCODE::READY_OP,MyName,color));
        ui->StatusLabel->setText("Waiting for Reply");
    }
    else if(onGame) ui->StatusLabel->setText("On Game!No Request");
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
    //对局申请的数据其实就是姓名、颜色和IP
    RequestData r{Name,RequestColor,ConnectedOne};

    rt->PutIn(r);
    if(Requestcnt == 0) rt->show();
    Requestcnt++;
    qDebug() <<"request++";
}

//同意对局后开启游戏
void NetWindow::StartGame()
{
    onGame=true;
    if(ServerorSocket == 1) this->server->send(ConnectedOne,NetworkData(OPCODE::READY_OP,MyName,NULL));

    ui->StatusLabel->setText("On Game");
    //新建一个游戏窗口对象

    m=new MainWindow;
    connect(m,&MainWindow::Move,this,&NetWindow::Move);
    connect(m->game,&Game::Suicide,this,&NetWindow::Suicide);
    connect(m->game,&Game::RivalTimeout,this,&NetWindow::Timeout);
    connect(m,&MainWindow::GiveUp,this,&NetWindow::GiveUp);
    connect(m,&MainWindow::CloseByPerson,this,&NetWindow::Left);
    m->NetMode=true;
    //全部设置一遍本方颜色
    if(MyColor == 1)
        m->setName(MyName+"(我)",HisName);
    else m->setName(HisName,MyName+"(我)");
    m->MyColor=m->game->MyColor=MyColor;
    m->game->online=true;
    m->BotButtonHide();
    m->show();

}

void NetWindow::Move(int x,int y)
{
    //编码转换
    char *cd=new char[3];
    cd[0]=y+'A'-1;
    cd[1]=x+'0';
    cd[2]='\0';

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
//发送我方投降信号
void NetWindow::GiveUp()
{
    GiveUpSent=1;
    switch (ServerorSocket) {
    //作为服务端发出更新
    case 1:
        this->server->send(ConnectedOne,NetworkData(OPCODE::GIVEUP_OP,MyName,NULL));
        break;
    //作为客户端发出更新
    case 2:
        this->socket->send(NetworkData(OPCODE::GIVEUP_OP,MyName,NULL));
        break;
    default:
        break;
    }
}
//发送对方超时信号
void NetWindow::Timeout()
{
    if(onGame)
    {
        TimeoutSent=1;
        switch (ServerorSocket) {
        //作为服务端发出更新
        case 1:
            this->server->send(ConnectedOne,NetworkData(OPCODE::TIMEOUT_END_OP,MyName,NULL));
            break;
            //作为客户端发出更新
        case 2:
            this->socket->send(NetworkData(OPCODE::TIMEOUT_END_OP,MyName,NULL));
            break;
        default:
            break;
        }
    }
}
//胜利和失败
void NetWindow::win(int code)
{
    if(onGame)
    {
        switch(code)
        {
        case 1:
            QMessageBox::information(this,"对方自杀","恭喜你赢了");
            break;
        case 2:
            QMessageBox::information(this,"对方认输","恭喜你赢了");
            break;
        case 3:
            QMessageBox::information(this,"对方超时","恭喜你赢了");
            break;
        default:
            break;
        }

        init();
        switch(ServerorSocket)
        {
        case 1:
            ui->StatusLabel->setText("赢！你可以发信息让对方再来一局");
            break;
        case 2:
            ui->StatusLabel->setText("赢！你可以点击对局请求再来一局");
            break;
        default:
            break;
        }
        delete m;
    }
}
void NetWindow::lose(int code)
{
    if(onGame)
    {
        switch(code)
        {
        case 1:
            QMessageBox::information(this,"自方自杀","很遗憾你输了");
            break;
        case 2:
            QMessageBox::information(this,"自方认输","很遗憾你输了");
            break;
        case 3:
            QMessageBox::information(this,"自方超时","很遗憾你输了");
            break;
        default:
            break;
        }
        //停表
        m->game->Timer->stop();
        init();
        switch(ServerorSocket)
        {
        case 1:
            ui->StatusLabel->setText("输！你可以发信息让对方再来一局");
            break;
        case 2:
            ui->StatusLabel->setText("输！你可以点击对局请求再来一局");
            break;
        default:
            break;
        }
        delete m;
    }
}

//离开行为
void NetWindow::Leave()
{
    ServerorSocket=0;
    init();
    ConnectedOne=nullptr;
}

//重置参数
void NetWindow::init()
{
    onGame=false;
    SuicideSent=0;
    GiveUpSent=0;
    TimeoutSent=0;
}
//拒绝对局后发送信息
void NetWindow::RejectGame(QString reason)
{
    this->server->send(ConnectedOne,NetworkData(OPCODE::REJECT_OP,MyName,reason));
    ui->StatusLabel->setText("Rejected " + HisName);
    ServerorSocket=0;
}

//主动离开的行为
void NetWindow::Left()
{
    ui->StatusLabel->setText("You Left");
    switch (ServerorSocket) {
    case 1:
        this->server->send(ConnectedOne,NetworkData(OPCODE::LEAVE_OP,NULL,NULL));
        break;
    case 2:

        this->socket->send(NetworkData(OPCODE::LEAVE_OP,NULL,NULL));
        this->socket->bye();
        break;
    default:
        break;
    }

    Leave();
}
//同意这个申请
void NetWindow::AcceptOne(QString name,QString color,QTcpSocket * soc)
{
    HisName=name;

    if(color == "黑")
        MyColor=-1;
    else if(color == "白")
        MyColor=1;

    ConnectedOne=soc;
    Requestcnt--;
    if(Requestcnt == 0) rt->hide();
    StartGame();

}
//拒绝这个申请
void NetWindow::RejectOne(QString reason,QTcpSocket * soc)
{
    ConnectedOne=soc;
    Requestcnt--;
    if(Requestcnt == 0) rt->hide();
    RejectGame(reason);
    ServerorSocket=1;
}
//关闭本窗口的事件
void NetWindow::closeEvent(QCloseEvent *event)
{
    if(ConnectedOne != nullptr)
    {
        Left();
    }
    emit ReturnStart();
    delete this;
}
