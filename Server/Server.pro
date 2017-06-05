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
<<<<<<< HEAD
    room.cpp
=======
    room.cpp \
    simplecrypte.cpp \
    smtp.cpp
>>>>>>> master

HEADERS += \
    server.h \
    roommanager.h \
<<<<<<< HEAD
    room.h
=======
    room.h \
    simplecrypte.h \
    smtp.h
>>>>>>> master
