#-------------------------------------------------
#
# Project created by QtCreator 2013-11-24T11:01:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ssot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ctoplabel.cpp \
    aboutdialog.cpp \
    controlbar.cpp

HEADERS  += mainwindow.h \
    ctoplabel.h \
    aboutdialog.h \
    controlbar.h

RESOURCES += \
    resource.qrc

RC_FILE  += \
    info.rc

OTHER_FILES += \
    info.rc \
    readme.txt
