#include "RZQLuaExports.h"
#include "RZQLua.hpp"

#include <string>

#include <QUrl>
RZ_LUA_DECLARE_CLASS(QUrl)
#include <QtWebSockets/QWebSocket>
RZ_LUA_DECLARE_CLASS(QWebSocket)
#include <RZQLua_QOpenGLFunctions.h>
RZ_LUA_DECLARE_CLASS(RZQLua_QOpenGLFunctions)
#include <RZQLua_QOpenGLShaderProgram.h>
RZ_LUA_DECLARE_CLASS(RZQLua_QOpenGLShaderProgram)

RZQLuaExports::RZQLuaExports(RZQLua &qlua) :
    m_qlua(qlua)
{
}

bool RZQLuaExports::export_lib_with_name(std::string lib, std::string name)
{
    if ("QOpenGLFunctions" == lib)
    {
        return export_class<RZQLua_QOpenGLFunctions>(name);
    }

    if ("debug" == lib)
    {
        return export_debug(name);
    }

    if ("QWebSocket" == lib)
    {
        return export_class<QWebSocket>(name);
    }

    if ("QUrl" == lib)
    {
        return export_class<QUrl>(name);
    }

    if ("QOpenGLShaderProgram" == lib)
    {
        return export_class<RZQLua_QOpenGLShaderProgram>(name);
    }

    return false;
}

bool RZQLuaExports::export_lib(std::string lib)
{
    return export_lib_with_name(lib, lib);
}

template<typename TClass>
RZLuaInstance<TClass> *RZQLuaExports::bind_instance(TClass *instance)
{
    std::stringstream ss;
    ss << ClassName<TClass>() << ":" << new_instance_id();
    auto instance_name = ss.str();

    auto const &L = m_qlua.lua().state();

    auto &instance_meta = m_qlua.lua().instances().bind_instance(instance_name, instance);

    declare_instance_functions(instance_meta);

    auto ud = lua_newuserdata(L, sizeof(TClass *));
    auto pi = static_cast<TClass**>(ud);
    *pi = instance;

    lua_getglobal(L, instance_name.c_str());
    if (!lua_istable(L, -1))
    {
        return false;
    }

    lua_pushstring(L, "instanceName");
    lua_pushstring(L, instance_name.c_str());
    lua_settable(L, -3);

    lua_setmetatable(L, -2);

    return &instance_meta;
}

int RZQLuaExports::new_instance_id()
{
    static int id = 0;

    return ++id;
}

void debug(QString message)
{
    qDebug() << message;
}

bool RZQLuaExports::export_debug(std::string const &/*name*/)
{
    m_qlua.lua().functions().declare("debug", &debug);

    return true;
}

template <typename TClass>
bool RZQLuaExports::export_class(std::string const &name)
{
    auto ctor = get_ctor<TClass>();

    auto L = m_qlua.lua().state();

    auto &cl = m_qlua.lua().classes().declare_class<TClass>(name);

    cl.push_table();

    lua_pushstring(L, "new");
    lua_pushlightuserdata(L, this);
    lua_pushcclosure(L, ctor, 1);
    lua_settable(L, -3);

    lua_getmetatable(L, -1);

    lua_pushstring(L, "__call");
    lua_pushlightuserdata(L, this);
    lua_pushcclosure(L, ctor, 1);
    lua_settable(L, -3);

    lua_pop(L, 1);

    declare_class_symbols<TClass>(cl);

    return true;
}
