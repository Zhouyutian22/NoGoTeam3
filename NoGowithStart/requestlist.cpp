#include "requestlist.h"
#include "ui_requestlist.h"


RequestList::RequestList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequestList)
{
    ui->setupUi(this);
    init();
}

RequestList::~RequestList()
{
    delete ui;
}

void RequestList::init()
{
    ItemModel=new QStandardItemModel(this);

    ShowRequest(" ");

    connect(ui->ListView, &QListView::clicked, [&](const QModelIndex& index) {

        //显示邀请
        ShowClick(index);
        // 获取被点击项的索引
        int clickedIndex = index.row();
        // 检查索引是否有效
        if (clickedIndex >= 0 && clickedIndex < StringList.count()) {
            // 移除被点击的项
            StringList.removeAt(clickedIndex);
            //更新显示
            ShowRequest(" ");
        }


    });

}

void RequestList::PutIn(RequestData re)
{
    QString user=re.Name;
    //将这个请求放入动态数组r
    r.append(re);
    ShowRequest("【新邀请】来自"+user+" 的邀请");
}

void RequestList::ShowClick(QModelIndex idx)
{
    QString strTemp;
    strTemp=idx.data().toString();
    //严格获取“【新邀请】来自xxx 的邀请”中的“xxx”（即用户名）
    int i=7;
    for(;i<strTemp.size();i++)
    {
                    //一个空格作姓名结尾              "的"字unicode的int型为30340
        if(strTemp.at(i) == ' ' &&(int)strTemp.at(i+1).unicode() == 30340) break;
    }
    QString str=strTemp.mid(7,i-7);
    //这个用户名将作为查找客户端的索引..（暂时没有更好地实现）

    for(RequestIndex=0;RequestIndex<r.size();RequestIndex++)
    {
        if(r[RequestIndex].Name == str) break;
    }
    Request *req=new Request(r[RequestIndex].Name,r[RequestIndex].Color);
    connect(req,&Request::Ac,this,&RequestList::Accepted);
    connect(req,&Request::Re,this,&RequestList::Rejected);

    req->show();


}

void RequestList::ShowRequest(QString str)
{

    if(str != ' ') StringList.append(str);
    ItemModel->clear();
    int ncnt=StringList.size();
    qDebug()<< ncnt;
    for(int i=0;i<ncnt;i++)
    {
        QString str=static_cast<QString>(StringList.at(i));
        QStandardItem *item=new QStandardItem(str);
        ItemModel->appendRow(item);

    }
    ui->ListView->setModel(ItemModel);
}

void RequestList::Accepted()
{
    for(int i=0;i<r.size();i++)
    {
        if(i != RequestIndex) emit Rej("我开启了别的对局",r[i].Socket);

    }
    emit Acc(r[RequestIndex].Name,r[RequestIndex].Color,r[RequestIndex].Socket);
    r.clear();
    StringList.clear();
}

void RequestList::Rejected(QString reason)
{
    qDebug()<<"Rej by list";
    emit Rej(reason,r[RequestIndex].Socket);
    r.erase(r.begin()+RequestIndex);

}
