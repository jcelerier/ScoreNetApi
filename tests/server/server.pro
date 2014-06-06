TEMPLATE = app
CONFIG += console

QT += core network

QMAKE_CXXFLAGS += -std=c++1y -stdlib=libc++
QMAKE_LFLAGS += -lc++ -lpthread
LIBS += -ldns_sd

INCLUDEPATH += ../../API/

#### Libraries ####
  ##  Oscpack  ##
unix:!macx: LIBS += -L$$PWD/../../../../../git/oscpack/build/ -loscpack

INCLUDEPATH += $$PWD/../../../../../git/oscpack/src
DEPENDPATH += $$PWD/../../../../../git/oscpack/src

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../git/oscpack/build/liboscpack.a


#### Source files ####
SOURCES += main.cpp \
    serverthread.cpp
HEADERS += $$PWD/../../../../../stage/soft/dpetri/src/lib/zeroconf/bonjourserviceregister.h \
    serverthread.h
HEADERS += $$PWD/../../../../../stage/NetAPI/ScoreNetApi/API/net/session/ZeroConfServer.h

