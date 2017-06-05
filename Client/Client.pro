#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T15:56:36
#
#-------------------------------------------------

QT       += core gui
QT       += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    dlsignin.cpp \
    simplecrypt.cpp \
    user.cpp

HEADERS  += \
    mainwindow.h \
    dlsignin.h \
    simplecrypt.h \
    user.h

FORMS    += \
    mainwindow.ui \
    dlsignin.ui

RESOURCES += \
    icons.qrc
