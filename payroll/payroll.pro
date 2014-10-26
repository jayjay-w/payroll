#-------------------------------------------------
#
# Project created by QtCreator 2014-10-24T16:34:33
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = payroll
TEMPLATE = app


SOURCES += main.cpp\
        payrollmainwindow.cpp \
    homewindow.cpp

HEADERS  += payrollmainwindow.h \
    homewindow.h

FORMS    += payrollmainwindow.ui \
    homewindow.ui
