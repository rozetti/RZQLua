#include "RZLuaExports.h"
#include "RZQLua.hpp"

#include "rz_lua_exports.h"

#include <string>

#include "RZQLua_QOpenGLFunctions.h"
#include "RZQLua_QOpenGLShaderProgram.h"

template <> char const *ClassName<RZLuaExports>() { return "rz"; }

template <> void declare_instance_functions(RZLuaInstance<RZLuaExports> &instance)
{
    instance.declare_function("import", &RZLuaExports::export_lib);
    instance.declare_function("importWithName", &RZLuaExports::export_lib_with_name);
}

RZLuaExports::RZLuaExports(RZQLua &qlua) :
    m_qlua(qlua)
{
}

bool RZLuaExports::export_lib_with_name(std::string lib, std::string name)
{
    if ("debug" == lib)
    {
        return export_debug(name);
    }

#define EXPORT(C) if (#C == lib) return export_class<C>(name);
#define EXPORT_PREFIXED(C) if (#C == lib) return export_class<RZQLua_##C>(name);

    EXPORT(QNetworkAccessManager);
    EXPORT(QNetworkRequest);
    EXPORT(QNetworkReply);
    EXPORT_PREFIXED(QOpenGLFunctions);
    EXPORT(QWebSocket);
    EXPORT(QUrl);
    EXPORT_PREFIXED(QOpenGLShaderProgram);

#undef EXPORT

    LOG_ERROR("unknown type: " << lib);

    return false;
}

RZLuaInstances &RZLuaExports::instances()
{
    return m_qlua.lua().instances();
}

lua_State *RZLuaExports::lua_state()
{
    return m_qlua.lua();
}

bool RZLuaExports::export_lib(std::string lib)
{
    return export_lib_with_name(lib, lib);
}

void debug(QString message)
{
    qDebug() << message;
}

bool RZLuaExports::export_debug(std::string const &/*name*/)
{
    m_qlua.lua().functions().declare("debug", &debug);

    return true;
}

template <typename TClass>
bool RZLuaExports::export_class(std::string const &name)
{
    auto ctor = ::get_ctor<TClass>();

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
