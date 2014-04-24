TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++1y -stdlib=libc++
QMAKE_LFLAGS += -lc++ -lpthread

INCLUDEPATH += ../../API/

#### Libraries ####
  ##  Oscpack  ##
unix:!macx: LIBS += -L$$PWD/../../../../../git/oscpack/build/ -loscpack

INCLUDEPATH += $$PWD/../../../../../git/oscpack/src
DEPENDPATH += $$PWD/../../../../../git/oscpack/src

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../git/oscpack/build/liboscpack.a


#### Source files ####
SOURCES += main.cpp

