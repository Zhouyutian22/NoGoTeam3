QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Game.cpp \
    giveup.cpp \
    gochess.cpp \
    main.cpp \
    mainwindow.cpp \
    resultwidget.cpp

HEADERS += \
    Game.h \
    giveup.h \
    gochess.h \
    mainwindow.h \
    resultwidget.h

FORMS += \
    giveup.ui \
    mainwindow.ui \
    resultwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    img/logo.ico

RESOURCES +=

RC_ICONS = img/logo.ico
