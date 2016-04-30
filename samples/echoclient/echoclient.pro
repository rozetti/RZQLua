QT       += core websockets qml quick
QT       -= gui

TARGET = echoclient
CONFIG   += console
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -O0
QMAKE_CXXFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS -= -O3

TEMPLATE = app

SOURCES += \
    main.cpp \
    echoclient.cpp

HEADERS += \
    echoclient.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../release/ -lRZQLua
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../debug/ -lRZQLua
else:unix: LIBS += -L$$PWD/../../ -lRZQLua

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../release/libRZQLua.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../debug/libRZQLua.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../release/RZQLua.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../debug/RZQLua.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../libRZQLua.a

RESOURCES += \
    echoclient.qrc

DISTFILES +=
