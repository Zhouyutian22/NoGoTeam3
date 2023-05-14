#include "rego.h"
#include "ui_rego.h"
#define WIDTH 30
#define HEIGHT 30
namespace {
    int maxsize = 100;
    int initsum = 0;
}
ReGo::ReGo(QWidget *parent) :
    QMainWindow(parent),sum(::initsum),color(true),ptx(0),pty(0),cnt(0),step(0),go(1),
    ui(new Ui::ReGo)
{
    ui->setupUi(this);
    rego_init();
    ui->label->setText("复盘");
}

ReGo::~ReGo()
{
    delete ui;
}

void ReGo::closeEvent(QCloseEvent *e)
{
    emit ReturnRego();
}

void ReGo::on_pushButton_clicked()
{
    //go = 1;
    //repaint();
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
    step++;
    qDebug()<<step;
    go = 0;
}

void ReGo::rego_init()
{
    QString path = QFileDialog::getOpenFileName(this,"open","mygames","TXT(*.txt)");
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
            }
            else if(readstr == -1)
            {
                qDebug()<<"error in reading";
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
            readstr = f.readLine(str,::maxsize);
            while(str[0]!='g')
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
        }
    }
    else
    {
        qDebug()<<"file does not exist";
    }
}

void ReGo::paintEvent(QPaintEvent *)
{
        if(step<=cnt)
        {
            drawboard();
            drawchess();
            update();
        }
        else
        {
            drawboard();
            QPainter painter(this);
            QPen pencil(Qt::transparent);
            painter.setPen(pencil);
            bool c = 1;
            for(int t = 0;t<cnt;t++)
            {
                if(!c)
                    painter.setBrush(Qt::white);
                else
                    painter.setBrush(Qt::black);
                QPoint center((board[t][0]+0.5)*WIDTH,(board[t][1]+0.5)*HEIGHT);
                painter.drawEllipse(center,WIDTH/2,HEIGHT/2);

                c = !c;
            }
        }
}
