#-------------------------------------------------
#
# Project created by QtCreator 2019-11-24T20:07:08
#
#-------------------------------------------------

QT       += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#添加相关库

INCLUDEPATH += $$PWD/vlc_win32/include/
INCLUDEPATH += $$PWD/vlc_win32/include/vlc/
LIBS += -L$$PWD/vlc_win32
LIBS += -llibvlc -llibvlccore

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
    monitor.cpp \
    video.cpp \
    setting.cpp \
    chart.cpp

HEADERS += \
        mainwindow.h \
    monitor.h \
    video.h \
    setting.h \
    chart.h

FORMS += \
        mainwindow.ui \
    monitor.ui \
    video.ui \
    setting.ui \
    chart.ui

RESOURCES += \
    resources.qrc


