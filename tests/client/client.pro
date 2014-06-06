TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
QT += core network

QMAKE_CXXFLAGS += -std=c++1y -stdlib=libc++
QMAKE_LFLAGS += -lc++ -lpthread

INCLUDEPATH += ../../API/

#### Libraries ####
  ##  Oscpack  ##
unix:!macx: LIBS += -L$$PWD/../../../../../git/oscpack/build/ -loscpack

INCLUDEPATH += $$PWD/../../../../../git/oscpack/src
DEPENDPATH += $$PWD/../../../../../git/oscpack/src

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../git/oscpack/build/liboscpack.a

LIBS += -ldns_sd

#### Source files ####
SOURCES += main.cpp \
    mythread.cpp
HEADERS += $$PWD/../../../../../stage/soft/dpetri/src/lib/zeroconf/bonjourserviceresolver.h \
    mythread.h
HEADERS += $$PWD/../../../../../stage/soft/dpetri/src/lib/zeroconf/bonjourservicebrowser.h
HEADERS += $$PWD/../../../../../stage/NetAPI/ScoreNetApi/API/net/session/ZeroConfClientSessionBuilder.h

