#-------------------------------------------------
#
# Project created by QtCreator 2018-03-21T15:30:15
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = intellij_machine_tool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    rulesettingwidget.cpp \
    rulelistwidget.cpp \
    datarulelinewidgetitem.cpp \
    tablelinepushbutton.cpp \
    context.cpp \
    rulewidget.cpp \
    ruleobject.cpp \
    inputform.cpp \
    inputitem.cpp \
    util.cpp \
    console.cpp

HEADERS += \
        mainwindow.h \
    rulesettingwidget.h \
    rulelistwidget.h \
    datarulelinewidgetitem.h \
    tablelinepushbutton.h \
    context.h \
    rulewidget.h \
    ruleobject.h \
    inputform.h \
    inputitem.h \
    util.h \
    console.h

FORMS += \
        mainwindow.ui \
    rulesettingwidget.ui \
    rulelistwidget.ui \
    inputform.ui \
    inputitem.ui \
    rulewidget.ui

RESOURCES += \
    resources.qrc
