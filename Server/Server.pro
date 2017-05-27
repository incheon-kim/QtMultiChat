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
    simplecrypt.cpp

HEADERS += \
    server.h \
    simplecrypt.h
