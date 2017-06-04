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
    roommanager.cpp \
    room.cpp \
    simplecrypte.cpp \
    libsmtp.cpp

HEADERS += \
    server.h \
    roommanager.h \
    room.h \
    simplecrypte.h \
    libsmtp.h \
    libsmtp_global.h
