#-------------------------------------------------
#
# Project created by QtCreator 2014-10-24T16:34:33
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = payroll
TEMPLATE = app

INCLUDEPATH += .

mac: TARGET = SmartPayroll

DESTDIR = ../build

SOURCES += main.cpp\
        payrollmainwindow.cpp \
    publics.cpp \
    newcompanydialog.cpp \
    companyinitializationdialog.cpp \
    paytypesdialog.cpp \
    sqlmodels.cpp \
    employeecentre.cpp \
    companyinformationdialog.cpp \
    changemonthdialog.cpp \
    aboutdialog.cpp \
    startnewemployeedialog.cpp \
    employeeeditor.cpp \
    kmessagewidget.cpp

HEADERS  += payrollmainwindow.h \
    publics.h \
    newcompanydialog.h \
    companyinitializationdialog.h \
    paytypesdialog.h \
    sqlmodels.h \
    employeecentre.h \
    companyinformationdialog.h \
    changemonthdialog.h \
    aboutdialog.h \
    startnewemployeedialog.h \
    employeeeditor.h \
    kmessagewidget.h

FORMS    += payrollmainwindow.ui \
    newcompanydialog.ui \
    companyinitializationdialog.ui \
    paytypesdialog.ui \
    employeecentre.ui \
    companyinformationdialog.ui \
    changemonthdialog.ui \
    aboutdialog.ui \
    startnewemployeedialog.ui \
    employeeeditor.ui

RESOURCES += \
    res.qrc

RC_FILE = Windows.rc

ICON = P.icns
