TEMPLATE = app
CONFIG += console warn_on
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++1y -stdlib=libc++
QMAKE_LFLAGS += -lc++
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
    net/session/Permission.h \
    net/session/PermissionManager.h \
    net/synchronisation_policy/SynchronisationPolicy.h \
    net/client/Client.h \
    net/client/LocalClient.h \
    net/client/RemoteClient.h \
    net/DereferenceIterator.h

