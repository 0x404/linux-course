#-------------------------------------------------
#
# Project created by QtCreator 2021-08-25T20:17:58
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ct3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    datapackage.cpp \
    dataanalyst.cpp \
    common.cpp \
    register.cpp \
    login.cpp \
    add_friend.cpp \
    mainwindow.cpp \
    talkpage.cpp \
    talkmessage.cpp \
    talkpagemany.cpp \
    talkmessagmany.cpp \
    file_sender.cpp \
    file_receiver.cpp

HEADERS += \
    datapackage.h \
    dataanalyst.h \
    common.h \
    register.h \
    login.h \
    add_friend.h \
    mainwindow.h \
    talkpage.h \
    talkmessage.h \
    talkpagemany.h \
    talkmessagmany.h \
    file_sender.h \
    file_receiver.h

FORMS += \
    register.ui \
    login.ui \
    add_friend.ui \
    mainwindow.ui \
    talkpage.ui \
    talkpagemany.ui \
    file_sender.ui \
    file_receiver.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



RC_ICONS =bitbug_favicon.ico

DISTFILES += \
    sizemould.bmp \
    blackaddfriend.png \
    blackchat.png

RESOURCES += \
    1.qrc \
    2.qrc \
    mainwindow.qrc
