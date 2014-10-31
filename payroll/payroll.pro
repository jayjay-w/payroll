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
    publics.cpp \
    newcompanydialog.cpp \
    companyinitializationdialog.cpp \
    paytypesdialog.cpp \
    sqlmodels.cpp \
    employeecentre.cpp \
    companyinformationdialog.cpp

HEADERS  += payrollmainwindow.h \
    publics.h \
    newcompanydialog.h \
    companyinitializationdialog.h \
    paytypesdialog.h \
    sqlmodels.h \
    employeecentre.h \
    companyinformationdialog.h

FORMS    += payrollmainwindow.ui \
    newcompanydialog.ui \
    companyinitializationdialog.ui \
    paytypesdialog.ui \
    employeecentre.ui \
    companyinformationdialog.ui

RESOURCES += \
    res.qrc

RC_FILE = Windows.rc

ICON = P.icns
