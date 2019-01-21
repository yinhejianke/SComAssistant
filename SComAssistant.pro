#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T19:15:30
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SComAssistant
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    datamanage.cpp \
    qcustomplot.cpp \
    datalogthrad.cpp \
    manysenddialog.cpp

HEADERS  += mainwindow.h \
    datamanage.h \
    qcustomplot.h \
    datalogthrad.h \
    manysenddialog.h

FORMS    += mainwindow.ui \
    manysenddialog.ui

include(global.pri)

RC_ICONS = DLAM.ico
