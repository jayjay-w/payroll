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
    homewindow.cpp \
    publics.cpp \
    newcompanydialog.cpp \
    companyinitializationdialog.cpp \
    paytypesdialog.cpp

HEADERS  += payrollmainwindow.h \
    homewindow.h \
    publics.h \
    newcompanydialog.h \
    companyinitializationdialog.h \
    paytypesdialog.h

FORMS    += payrollmainwindow.ui \
    homewindow.ui \
    newcompanydialog.ui \
    companyinitializationdialog.ui \
    paytypesdialog.ui

RESOURCES += \
    res.qrc

RC_FILE += \
    Windows.rc
