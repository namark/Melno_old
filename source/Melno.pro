#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T14:23:16
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++0x -U__STRICT_ANSI__

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Melno
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

LIBS += -lQtUIUtils
