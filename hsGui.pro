#-------------------------------------------------
#
# Project created by QtCreator 2010-07-30T16:13:33
#
#-------------------------------------------------

QT       += core gui network webkit


TARGET = hsGui
TEMPLATE = app


SOURCES += main.cpp\
        hsgui.cpp \
    jalousieeinstellungen.cpp \
    lichteinstellungen.cpp \
    schliessen.cpp \
    allgemeineeinstellungen.cpp

HEADERS  += hsgui.h \
    jalousieeinstellungen.h \
    lichteinstellungen.h \
    hsguinetcmd.h \
    iniparser.h \
    hsguiglobal.h \
    schliessen.h \
    allgemeineeinstellungen.h

FORMS    += hsgui.ui \
    jalousieeinstellungen.ui \
    lichteinstellungen.ui \
    schliessen.ui \
    allgemeineeinstellungen.ui

RESOURCES += \
    hsGui.qrc

#LIBS += -L/usr/lib -liniparser
LIBS += -L/home/florian/Entwicklung/iniparser-build-desktop/ -liniparser
