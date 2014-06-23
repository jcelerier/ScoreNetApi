TEMPLATE = app
CONFIG += console warn_on
CONFIG -= app_bundle
QT += network

QMAKE_CXXFLAGS += -std=c++1y -stdlib=libc++
QMAKE_LFLAGS += -lc++ -lpthread

#### Libraries ####
  ##  Oscpack  ##
unix:!macx: LIBS += -L$$PWD/../../../../git/oscpack/build/ -loscpack

INCLUDEPATH += $$PWD/../../../../git/oscpack/src
DEPENDPATH += $$PWD/../../../../git/oscpack/src

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../git/oscpack/build/liboscpack.a


#### Source files ####
SOURCES += \ #  main.cpp \
\ #    Scenario.cpp \
    TimeProcess.cpp \
    IObservable.cpp \
    naiveImpl/Scenario.cpp \
    naiveImpl/later.cpp \
    naiveImpl/Event.cpp \
    naiveImpl/TimeBox.cpp \
    naiveImpl/TimeNode.cpp \
    naiveImpl/TimeValue.cpp \
    petrinet/PNScenario.cpp \
    petrinet/PNTimeBox.cpp \
    petrinet/PNTimeNode.cpp

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
    TimeValue.h \
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
    net/permission/full/Permission.h \
    net/permission/full/PermissionManager.h \
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
    net/client/LocalMaster.h \
    net/permission/view/PermissionView.h \
    net/permission/PermissionBase.h \
    net/permission/view/PermissionViewManager.h \
    naiveImpl/later.h \
    petrinet/conversion.h \
    petrinet/PNScenario.h \
    petrinet/PNTimeBox.h \
    petrinet/PNTimeNode.h \
    net/session/ZeroConfClientSessionBuilder.h \
    net/session/ZeroConfServer.h \
    net/session/ZeroConfServerThread.h \
    petrinet/deplacement.h

