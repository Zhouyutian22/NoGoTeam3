QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Game.cpp \
    chooseroadwindow.cpp \
    gochess.cpp \
    main.cpp \
    mainwindow.cpp \
    netwindow.cpp \
    networkdata.cpp \
    networkserver.cpp \
    networksocket.cpp \
    readrecords.cpp \
    rego.cpp \
    regowindow.cpp \
    request.cpp \
    requestlist.cpp \
    resultwidget.cpp \
    startwindow.cpp

HEADERS += \
    Game.h \
    chooseroadwindow.h \
    gochess.h \
    mainwindow.h \
    netwindow.h \
    networkdata.h \
    networkserver.h \
    networksocket.h \
    readrecords.h \
    rego.h \
    regowindow.h \
    request.h \
    requestlist.h \
    resultwidget.h \
    startwindow.h

FORMS += \
    chooseroadwindow.ui \
    mainwindow.ui \
    netwindow.ui \
    readrecords.ui \
    rego.ui \
    regowindow.ui \
    request.ui \
    requestlist.ui \
    resultwidget.ui \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
    img/logo.ico \
    img/logo.ico

RESOURCES +=

RC_ICONS = img/logo.ico

QT += network
