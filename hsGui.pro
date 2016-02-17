#-------------------------------------------------
#
# Project created by QtCreator 2010-07-30T16:13:33
#
#-------------------------------------------------

QT       += core gui network webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = hsGui
TEMPLATE = app


SOURCES += main.cpp\
        hsgui.cpp \
    jalousieeinstellungen.cpp \
    lichteinstellungen.cpp \
    schliessen.cpp \
    allgemeineeinstellungen.cpp \
    musikplayer.cpp \
    mpnetworkthread.cpp

HEADERS  += hsgui.h \
    jalousieeinstellungen.h \
    lichteinstellungen.h \
    hsguinetcmd.h \
    iniparser.h \
    hsguiglobal.h \
    schliessen.h \
    allgemeineeinstellungen.h \
    musikplayer.h \
    mpnetworkthread.h

FORMS    += hsgui.ui \
    jalousieeinstellungen.ui \
    lichteinstellungen.ui \
    schliessen.ui \
    allgemeineeinstellungen.ui

RESOURCES += \
    hsGui.qrc

#LIBS += -L/usr/lib -liniparser
LIBS += -L/home/florian/Entwicklung/iniparser-build-desktop/ -liniparser
