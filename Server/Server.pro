QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    userinfo.cpp \
    room.cpp \
    thread.cpp

HEADERS += \
    server.h \
    userinfo.h \
    room.h \
    thread.h
