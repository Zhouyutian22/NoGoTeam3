#include "startwindow.h"

class startwindowData : public QSharedData
{
public:

};

startwindow::startwindow(QWidget *parent)
    : QMainWindow{parent}, data(new startwindowData)
{

}

startwindow::startwindow(const startwindow &rhs)
    : data{rhs.data}
{

}

startwindow &startwindow::operator=(const startwindow &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

startwindow::~startwindow()
{

}
