#pragma once

#include <QVariantList>
#include <QString>
#include <QtGui/QOpenGLShaderProgram>
#include <assert.h>

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

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
    return QString(lua_tostring(l, i));
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
        /* ‘key’ is at index -2 and ‘value’ at index -1 */
        //printf("%s - %s\n", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));

        auto f = lua_tonumber(L, -1);
        ff.push_back(f);
        lua_pop(L, 1);
    }

    return ff;
}

template<>
QOpenGLShader::ShaderType get_at_index(lua_State *l, int i)
{
    return (QOpenGLShader::ShaderType)lua_tointeger(l, i);
}

}
}
