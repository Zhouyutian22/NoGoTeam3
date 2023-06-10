#include "rego.h"
#include "ui_rego.h"
#define WIDTH 30
#define HEIGHT 30
namespace {
    int maxsize = 100;
    int initsum = 0;
}
ReGo::ReGo(QWidget *parent) :
    QMainWindow(parent),sum(::initsum),ptx(0),pty(0),cnt(0),step(0),show_modeon(false),flag(0),move(0),
    ui(new Ui::ReGo)
{
    ui->setupUi(this);
    rego_init();
    timer.setInterval(1000);
    connect(&timer,&QTimer::timeout,this,&ReGo::continueshow);      //播放棋局时的延时功能，用的一个计时器，记时时暂停，这是计时结束时结束暂停继续播放的槽
    //ui->label->setText("复盘");
    ui->lineEdit->setPlaceholderText("plz enter the step");
    QRegularExpression rx("[0-9/.]+$");         //正则表达式，设置lineEdit里面只能输入正整数，不能输入其他字符
    QRegularExpressionValidator *val = new QRegularExpressionValidator(rx,this);
    ui->lineEdit->setValidator(val);
}

ReGo::~ReGo()
{
    delete ui;
}

void ReGo::closeEvent(QCloseEvent *e)   //关闭本窗口时发出返回上级窗口的信号
{
    emit ReturnRego();
}

void ReGo::on_pushButton_clicked()          //点击播放，播放函数，update方法更新局面
{
    update();
    if(flag)
    {
        if(step<cnt)
            step++;
        else
            step = cnt;
    }
    else
        flag = 1;
}

void ReGo::drawboard()
{
    QPainter painter(this);     //调用绘画工具
    QPen pencil(Qt::black);     //黑色画笔画棋盘
    pencil.setWidth(2);         //宽度为2
    qDebug()<<road;
    painter.setPen(pencil);
    for(int r=1;r<=road-1;r++)
    {
        for(int c=1;c<=road-1;c++)
        {
            painter.drawRect((r+0.5)*WIDTH,(c+0.5)*HEIGHT,WIDTH,HEIGHT);
        }
    }
    //画棋盘使用drawRect方法
}

void ReGo::drawchess()
{
    QPainter painter(this);
    QPen pencil(Qt::transparent);
    painter.setPen(pencil);
    bool color = 1;
    for(int t = 0;t < step;t++)
    {
        if(!color)
            painter.setBrush(Qt::white);
        else
            painter.setBrush(Qt::black);

        QPoint center((board[t][0]+0.5)*WIDTH,(board[t][1]+0.5)*HEIGHT);
        painter.drawEllipse(center,WIDTH/2,HEIGHT/2);

        color = !color;
    }
    //qDebug()<<step;
}

void ReGo::rego_init()              //初始化
{
    QString path = QFileDialog::getOpenFileName(this,"open","mygames","TXT(*.txt)");        //打开并阅读文件，从中得到上一局对局记录
    if(!path.isEmpty())
    {
        QFile f(path);

        if(f.open(QIODevice::ReadOnly))
        {
            char *str = new char[::maxsize];
            qint64 readstr = f.readLine(str,::maxsize);
            if(readstr==0)
            {
                qDebug()<<"No games saved!";
                emit ReturnRego();
            }
            else if(readstr == -1)
            {
                qDebug()<<"error in reading";
                emit ReturnRego();
            }
            else;

            while(readstr!=0&&readstr!=-1)          //统计当前局数
            {
                if(str[0]=='-')
                    sum++;

                readstr = f.readLine(str,::maxsize);
            }
            delete []str;
            f.close();

        }
        else
        {
            qDebug()<<"open fail";
            emit ReturnRego();
        }

        if(f.open(QIODevice::ReadOnly))
        {
            char *str = new char[::maxsize];
            int current = 0;
            qint64 readstr = f.readLine(str,::maxsize);

            while(readstr!=0&&readstr!=-1&&current<sum)          //统计当前局数
            {

                if(str[0]=='-')
                    current++;

                if(str[2]=='\n')
                    road = (int)(str[1]-48);
                else
                    road = (int)(str[1]-48)*10+(int)(str[2]-48);
                readstr = f.readLine(str,::maxsize);
            }

            qDebug()<<road<<' '<<sum;
            //getchar();
            readstr = f.readLine(str,::maxsize);        //按行来读，根据储存的格式，找到记录棋子位置的信息，读取并转化为int值储存在静态二维数组中
            while(str[0]!='g')
            {
                if(str[1]==':')
                {
                    ptx = (int)(str[15]-64);
                    if(str[17]=='\n')
                        pty = (int)(str[16]-48);
                    else
                        pty = (int)(str[16]-48)*10+(int)(str[17]-48);
                    board[cnt][0] = ptx;
                    board[cnt][1] = pty;
                    cnt++;
                    //qDebug()<<ptx<<' '<<pty;
                    readstr = f.readLine(str,::maxsize);
                }
                else if(str[2]==':')
                {
                    ptx = (int)(str[16]-64);
                    if(str[18]=='\n')
                        pty = (int)(str[17]-48);
                    else
                        pty = (int)(str[17]-48)*10+(int)(str[18]-48);
                    board[cnt][0] = ptx;
                    board[cnt][1] = pty;
                    cnt++;
                    //qDebug()<<ptx<<' '<<pty;
                    readstr = f.readLine(str,::maxsize);
                }
                else if(str[3]==':')
                {
                    ptx = (int)(str[17]-64);
                    if(str[17]=='\n')
                        pty = (int)(str[18]-48);
                    else
                        pty = (int)(str[18]-48)*10+(int)(str[19]-48);
                    board[cnt][0] = ptx;
                    board[cnt][1] = pty;
                    cnt++;
                    //qDebug()<<ptx<<' '<<pty;
                    readstr = f.readLine(str,::maxsize);
                }
                else;
            }
            while(str[0]!='W')
            {
                readstr = f.readLine(str,::maxsize);
            }
            if(str[14]=='b')
                winner = 1;
            else
                winner = 0;

            ui->label_2->setText(str);

            qDebug()<<cnt;
            delete []str;
            f.close();
        }
        else
        {
            qDebug()<<"open fail";
            emit ReturnRego();
        }
    }
    else
    {
        qDebug()<<"file does not exist";
        emit ReturnRego();
    }
}

void ReGo::paintEvent(QPaintEvent *)
{
    if(!show_modeon)
    {
        if(move)
        {
            if(to_which_step>=0&&to_which_step<=cnt)
            {
                step = to_which_step;
                update();
                    move = 0;
            }
            else
            {
                QWidget *w = new QWidget;
                QLabel *l = new QLabel(w);

                l->setText("步数错误！");
                move = 0;
                w->show();
            }
        }
        else
        {
            drawboard();
            drawchess();
        }
    }
    else
    {
        if(move)
        {
            if(to_which_step>=0&&to_which_step<=cnt)
            {
                step = to_which_step;
                update();
                move = 0;
            }
            else
            {
                QWidget *w = new QWidget;
                QLabel *l = new QLabel(w);

                l->setText("步数错误！");
                move = 0;
                w->show();
            }
        }
        else
        {
            drawboard();
            drawchess();
            stopshow();
            timer.start();
            if(step<cnt)
                step++;
            else;
        }
    }
}

void ReGo::on_pushButton_2_clicked()
{
        if(step>0)
            step--;
        else
            step = 0;
        update();
}


void ReGo::on_pushButton_3_clicked()
{
    show_modeon = true;
}


void ReGo::on_pushButton_4_clicked()
{
    timer.stop();
    show_modeon = false;
}

void ReGo::stopshow()
{
    show_modeon = false;
    flag = 0;
}

void ReGo::continueshow()
{
    show_modeon = true;
    update();
}

void ReGo::on_lineEdit_editingFinished()
{
    ;//no use
}



void ReGo::on_lineEdit_returnPressed()
{
    QString str = ui->lineEdit->text();
    to_which_step = str.toInt();
    move = 1;
}


void ReGo::on_pushButton_5_clicked()        //重新开始下棋函数，本质上是重开了一个游戏窗口，但提前预置了此步以前的局面
{
    MainWindow *m=new MainWindow;
    m->game->closed=0;
    m->game->recmode = 0;
    m->game->road = road;
    bool col = 1;
    step--;
    for(int i = 0;i<step;i++)
    {
            QPoint point;
            point.setX(board[i][0]);
            point.setY(board[i][1]);
            goChess chess(point,col);
            m->chesses.append(chess);
            col = !col;
    }
    m->current = col;
    m->show();
}


