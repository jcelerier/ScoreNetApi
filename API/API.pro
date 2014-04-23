TEMPLATE = app
CONFIG += console warn_on
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++1y -stdlib=libc++
QMAKE_LFLAGS += -lc++

#### Libraries ####
  ##  Oscpack  ##
unix:!macx: LIBS += -L$$PWD/../../../../git/oscpack/build/ -loscpack

INCLUDEPATH += $$PWD/../../../../git/oscpack/src
DEPENDPATH += $$PWD/../../../../git/oscpack/src

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../git/oscpack/build/liboscpack.a


#### Source files ####
SOURCES += main.cpp \
    Scenario.cpp \
    TimeProcess.cpp \
    IObservable.cpp

HEADERS += \
    Event.h \
    InteractiveBranching.h \
    IObservable.h \
    TimeProcess.h \
    IObserver.h \
    Message.h \
    Scenario.h \
    Score.h \
    State.h \
    StateElement.h \
    TimeBox.h \
    TimeNode.h \
    net/DistributedScenario.h \
    net/Iterable.h \
    net/client/ClientManager.h \
    net/group/GroupManager.h \
    net/group/BackupGroup.h \
    net/group/Group.h \
    net/properties/hasName.h \
    net/properties/GenerateUniqueProperty.h \
    net/properties/hasId.h \
    net/session/Session.h \
    net/permission/Permission.h \
    net/permission/PermissionManager.h \
    net/synchronisation_policy/SynchronisationPolicy.h \
    net/client/Client.h \
    net/client/LocalClient.h \
    net/client/RemoteClient.h \
    net/osc/oscsender.h \
    net/osc/oscreceiver.h \
    net/osc/oscmessagegenerator.h \
    net/session/MasterSession.h \
    net/session/ClientSession.h \
    net/session/ClientSessionBuilder.h \
    net/client/RemoteSender.h \
    net/client/RemoteMaster.h \
    net/client/LocalMaster.h

