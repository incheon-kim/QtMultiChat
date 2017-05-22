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
    signin.cpp

HEADERS  += \
    mainwindow.h \
    signin.h

FORMS    += \
    mainwindow.ui \
    signin.ui

RESOURCES += \
    icons.qrc
