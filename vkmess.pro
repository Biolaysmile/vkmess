#-------------------------------------------------
#
# Project created by QtCreator 2016-08-09T22:31:00
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vkmess
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    json.cpp

HEADERS  += widget.h \
    json.h

FORMS    += widget.ui
