#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>

class startwindowData;

class startwindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit startwindow(QWidget *parent = nullptr);
    startwindow(const startwindow &);
    startwindow &operator=(const startwindow &);
    ~startwindow();

signals:


private:
    QSharedDataPointer<startwindowData> data;
};

#endif // STARTWINDOW_H
