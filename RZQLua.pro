#-------------------------------------------------
#
# Project created by QtCreator 2016-04-14T21:20:16
#
#-------------------------------------------------

QT       -= gui

QMAKE_CXXFLAGS += -O0
QMAKE_CXXFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS -= -O3

TARGET = RZQLua
TEMPLATE = lib
CONFIG += staticlib

DEFINES += RZQLUA_LIBRARY

SOURCES += \
    lua/lapi.c \
    lua/lauxlib.c \
    lua/lbaselib.c \
    lua/lbitlib.c \
    lua/lcode.c \
    lua/lcorolib.c \
    lua/lctype.c \
    lua/ldblib.c \
    lua/ldebug.c \
    lua/ldo.c \
    lua/ldump.c \
    lua/lfunc.c \
    lua/lgc.c \
    lua/linit.c \
    lua/liolib.c \
    lua/llex.c \
    lua/lmathlib.c \
    lua/lmem.c \
    lua/loadlib.c \
    lua/lobject.c \
    lua/lopcodes.c \
    lua/loslib.c \
    lua/lparser.c \
    lua/lstate.c \
    lua/lstring.c \
    lua/lstrlib.c \
    lua/ltable.c \
    lua/ltablib.c \
    lua/ltm.c \
    lua/lundump.c \
    lua/lvm.c \
    lua/lzio.c \
    RZLua.cpp \
    RZQLua.cpp \
    RZQLuaExports.cpp \
    RZQLua_QOpenGLShaderProgram.cpp \
    RZLuaClass.cpp \
    RZLuaInstance.cpp \
    RZQLua_QUrl.cpp \
    RZQLua_QWebSocket.cpp \
    RZQLua_QOpenGLFunctions.cpp \
    rz_qlua_types.cpp \
    RZQLua_QNetworkAccessManager.cpp \
    RZQLua_QNetworkReply.cpp \
    RZQLua_QNetworkRequest.cpp

HEADERS += \
    lua/lapi.h \
    lua/lauxlib.h \
    lua/lcode.h \
    lua/lctype.h \
    lua/ldebug.h \
    lua/ldo.h \
    lua/lfunc.h \
    lua/lgc.h \
    lua/llex.h \
    lua/llimits.h \
    lua/lmem.h \
    lua/lobject.h \
    lua/lopcodes.h \
    lua/lparser.h \
    lua/lstate.h \
    lua/lstring.h \
    lua/ltable.h \
    lua/ltm.h \
    lua/lua.h \
    lua/lua.hpp \
    lua/luaconf.h \
    lua/lualib.h \
    lua/lundump.h \
    lua/lvm.h \
    lua/lzio.h \
    RZQLuaExports.h \
    RZQLua_QOpenGLShaderProgram.h \
    rz.h \
    RZLuaInstance.hpp \
    RZQLua_QOpenGLFunctions.h \
    rz_log.h \
    RZLuaClass.hpp \
    RZLuaFreeFunction.hpp \
    RZLuaFunctionBase.hpp \
    RZLuaMemberFunction.hpp \
    RZQLua.hpp \
    RZLua.hpp \
    rz_tuple.hpp \
    rz_qlua_types.h \
    RZQLua_QNetworkAccessManager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
