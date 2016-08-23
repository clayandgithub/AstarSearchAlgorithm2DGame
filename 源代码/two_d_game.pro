#-------------------------------------------------
#
# Project created by QtCreator 2014-04-20T16:09:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = two_d_game
TEMPLATE = app


SOURCES += main.cpp\
        Mainwindow.cpp \
    layer.cpp \
    mixedStuff.cpp \
    layermanager.cpp \
    gamemap.cpp \
    sprite.cpp \
    astarblock.cpp

HEADERS  += Mainwindow.h \
    layer.h \
    layermanager.h \
    gamemap.h \
    sprite.h \
    astarblock.h

FORMS    += Mainwindow.ui
