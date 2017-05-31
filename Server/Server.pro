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
    roommanager.cpp \
    room.cpp

HEADERS += \
    server.h \
    roommanager.h \
    room.h
