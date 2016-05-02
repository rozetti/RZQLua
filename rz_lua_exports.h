#pragma once

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}
//struct lua_State;

template <typename> class RZLuaInstance;
class RZQLuaExports;

template <typename TClass>
char const *ClassName();

template <typename TClass>
int (*get_ctor())(lua_State*);

template <typename TClass>
void declare_instance_functions(RZLuaInstance<TClass> &instance);

static int id = 0;

#include <sstream>

template <typename TClass>
RZLuaInstance<TClass> *bind_instance(RZQLuaExports &ex, TClass *instance)
{
    std::stringstream ss;
    ss << ClassName<TClass>() << ":" << ++id;
    auto instance_name = ss.str();

    auto instance_meta = *(RZLuaInstance<TClass> *)nullptr;

    lua_State *L = nullptr;
    //    auto const &L = m_qlua.lua().state();

//    auto &instance_meta = m_qlua.lua().instances().bind_instance(instance_name, instance);

    ::declare_instance_functions(instance_meta);

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


#define RZ_LUA_DECLARE_CLASS(C) \
template <> char const *ClassName<C>() { return #C; } \
extern template void declare_instance_functions(RZLuaInstance<C> &instance); \
template RZLuaInstance<C> *bind_instance(RZQLuaExports &, C*); \
extern template int (*get_ctor<C>())(lua_State*);

/*
//template RZLuaInstance<C> *RZQLuaExports::bind_instance(C*); \
//extern template int (*RZQLuaExports::get_ctor<C>())(lua_State*);
*/
