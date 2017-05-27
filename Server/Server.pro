QT += core
QT += network
QT -= gui
QT += sql
CONFIG += c++11

TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    simplecrypt.cpp \
    dbmanager.cpp

HEADERS += \
    server.h \
    simplecrypt.h \
    dbmanager.h
