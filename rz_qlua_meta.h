#pragma once

#include "rz_lua.h"

#include <QVariantList>
#include <QString>
#include <assert.h>
#include <QByteArray>

#include "RZLuaExports_includes.h"

namespace rz
{

namespace detail
{

template <typename TArg>
void build_args_list(QVariantList &list, TArg arg)
{
    list << arg;
}

template <typename TArg, typename... TArgs>
void build_args_list(QVariantList &list, TArg arg1, TArg arg2, TArgs... args)
{
    //list << arg;
    build_args_list(list, arg1);
    build_args_list(list, arg2, args...);
}

template<>
QString get_at_index(lua_State *l, int i)
{
    auto v = QString(lua_tostring(l, i));
    LOG_VERBOSE("get_at_index<QString>(" << i << ") -> " << v.toStdString());
    return v;
}

template<>
std::vector<float> get_at_index(lua_State *L, int i)
{
    std::vector<float> ff;

    if (!lua_istable(L  , i))
    {
        assert(false);
        return ff;
    }

    lua_pushnil(L);
    while (lua_next(L, i) != 0)
    {
        auto f = lua_tonumber(L, -1);
        ff.push_back(f);
        lua_pop(L, 1);
    }

    return ff;
}

template<>
QUrl const &get_at_index(lua_State *L, int i)
{
    if (!lua_isuserdata(L, i))
    {
        assert(false);
        return *(QUrl const *)nullptr;
    }

    auto ud = lua_touserdata(L, i);
    auto pi = static_cast<QUrl **>(ud);
    auto v = *pi;

    LOG_VERBOSE("get_at_index<QUrl const &>(" << i << ") -> " << v->url().toStdString());

    return *v;
}

template<>
QNetworkRequest const &get_at_index(lua_State *L, int i)
{
    if (!lua_isuserdata(L, i))
    {
        assert(false);
        return *(QNetworkRequest const *)nullptr;
    }

    auto ud = lua_touserdata(L, i);
    auto pi = static_cast<QNetworkRequest **>(ud);
    auto v = *pi;

    LOG_DEBUG("get_at_index<QNetworkRequest const &>("
              << i << ") -> " << v->url().toString().toStdString());

    return *v;
}

static QString temp;

template<>
QString const &get_at_index(lua_State *L, int i)
{
    if (!lua_isstring(L, i))
    {
        LOG_ERROR("string expected");
        assert(false);
        temp = "";
        return temp;
    }

    auto s = lua_tostring(L, i);
    temp = s;

    LOG_VERBOSE("get_at_index<QString const &>(" << i << ") -> " << temp.toStdString());

    return temp;
}

template<>
QOpenGLShader::ShaderType get_at_index(lua_State *l, int i)
{
    auto v = lua_tointeger(l, i);

    LOG_VERBOSE("get_at_index<QOpenGLShader::ShaderType>(" << i << ") -> " << v);

    return (QOpenGLShader::ShaderType)v;
}

template<>
QWebSocketProtocol::CloseCode get_at_index(lua_State *l, int i)
{
    auto v = lua_tointeger(l, i);

    LOG_VERBOSE("get_at_index<QWebSocketProtocol::CloseCode>(" << i << ") -> " << v);

    return (QWebSocketProtocol::CloseCode)v;
}

static inline void push(lua_State *l, QString value)
{
    lua_pushstring(l, value.toStdString().c_str());
}

static inline void push(lua_State *l, QByteArray const &value)
{
    auto len = value.length();
    auto data = value.data();
    lua_pushlstring(l, data, len);
}

}
}
