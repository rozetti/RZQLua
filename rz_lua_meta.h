#pragma once

// crz: lots of techniques and inspiriation for this lot came from:
// http://www.jeremyong.com/blog/2014/01/10/interfacing-lua-with-templates-in-c-plus-plus-11/

#include <functional>
#include <memory>
#include <string>
#include <sstream>

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

template<typename TRet>
static TRet get_at_index(lua_State *l, int i);

template<>
int get_at_index(lua_State *l, int i)
{
    return static_cast<int>(lua_tointeger(l, i));
}

template<>
unsigned int get_at_index(lua_State *l, int i)
{
    return static_cast<unsigned int>(lua_tointeger(l, i));
}

template<>
float get_at_index(lua_State *l, int i)
{
    return static_cast<float>(lua_tonumber(l, i));
}

template<>
double get_at_index(lua_State *l, int i)
{
    return lua_tonumber(l, i);
}

template<>
std::string get_at_index(lua_State *l, int i)
{
    return lua_tostring(l, i);
}

template<>
char const *get_at_index(lua_State *l, int i)
{
    return lua_tostring(l, i);
}

template<>
bool get_at_index(lua_State *l, int i)
{
    return (bool)lua_toboolean(l, i);
}

//--------------------------------------------------------

template <std::size_t... Indicies> struct _index_tag {};

template <std::size_t N, std::size_t... Indicies>
struct index_tag : index_tag<N-1, N-1, Indicies...> {};

template <std::size_t... Indicies>
struct index_tag<0, Indicies...>
{
    using tag = _index_tag<Indicies...>;
};

//--------------------------------------------------------

template <typename... TArgs, std::size_t... N>
std::tuple<TArgs...> load_args(lua_State *L, _index_tag<N...>)
{
    Q_UNUSED(L);

    return std::make_tuple(rz::detail::get_at_index<TArgs>(L, N+1)...);
}

template <typename... TArgs>
std::tuple<TArgs...> load_args(lua_State *L)
{
    return load_args<TArgs...>(L, typename index_tag<sizeof...(TArgs)>::tag());
}

//--------------------------------------------------------------

template <typename TRet, typename... TArgs, std::size_t... N>
TRet invoke(std::function<TRet(TArgs...)> func, std::tuple<TArgs...> args, _index_tag<N...>)
{
    Q_UNUSED(args);

    return func(std::get<N>(args)...);
}

template <typename TRet, typename... TArgs>
TRet invoke(std::function<TRet(TArgs...)> func, std::tuple<TArgs...> args)
{
    Q_UNUSED(args);

    return invoke(func, args, typename index_tag<sizeof...(TArgs)>::tag());
}

//--------------------------------------------------------------

template <typename T>
struct type_list_size
{
    static constexpr int value = 1;
};

template <typename... Vs>
struct type_list_size<std::tuple<Vs...>>
{
    static constexpr int value = sizeof...(Vs);
};

template <>
struct type_list_size<void>
{
    static constexpr int value = 0;
};

//---------------------------------------------------------------

static inline void push(lua_State *l, int value)
{
    lua_pushinteger(l, value);
}

static inline void push(lua_State *l, unsigned int value)
{
    lua_pushinteger(l, value);
}

static inline void push(lua_State *l, bool value)
{
    lua_pushboolean(l, value);
}

static inline void push(lua_State *l, float value)
{
    lua_pushnumber(l, value);
}

static inline void push(lua_State *l, char const *value)
{
    lua_pushstring(l, value);
}

static inline void push(lua_State *l, std::string value)
{
    lua_pushstring(l, value.c_str());
}

}
}
