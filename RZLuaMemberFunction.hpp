#pragma once

#include "rz_log.h"
#include "rz_lua_meta.h"
#include "rz_qlua_meta.h"
#include "RZLuaFunctionBase.hpp"

template <int N, typename TRet, typename... TArgs>
class RZLuaMemberFunctionBase : public RZLuaFunctionBaseT<N, TRet, TArgs...>
{
public:
    RZLuaMemberFunctionBase(lua_State *&L,
                 std::string const &className,
                 std::string const &functionName,
                 typename RZLuaFunctionBaseT<N, TRet, TArgs...>::type fun,
                 std::string const &desc) :
        RZLuaFunctionBaseT<N, TRet, TArgs...>(L, className, functionName, fun, desc)
    {
        lua_getglobal(L, className.c_str());
        lua_pushlightuserdata(L, (void *)static_cast<RZLuaCallableBase *>(this));
        lua_pushcclosure(L, &dispatcher, 1);
        lua_setfield(L, -2, functionName.c_str());
        lua_pop(L, 1);
    }

    RZLuaMemberFunctionBase(RZLuaMemberFunctionBase &&other) = delete;
    RZLuaMemberFunctionBase(RZLuaMemberFunctionBase const &other) = delete;
};

template <int N, typename TRet, typename... TArgs>
class RZLuaMemberFunction : public RZLuaMemberFunctionBase<N, TRet, TArgs...>
{
public:
    RZLuaMemberFunction(lua_State *&l, const std::string &name, TRet(*fun)(TArgs...)) :
        RZLuaMemberFunction(l, name, typename RZLuaFunctionBaseT<N, TRet, TArgs...>::type{fun})
    {
    }

    RZLuaMemberFunction(lua_State *&l, std::string const &className,
             const std::string &name,
                        typename RZLuaFunctionBaseT<N, TRet, TArgs...>::type fun) :
        RZLuaMemberFunctionBase<N, TRet, TArgs...>(l, className, name, fun, "non-void method")
    {
    }

    RZLuaMemberFunction(RZLuaMemberFunction const &other) = delete;
    RZLuaMemberFunction(RZLuaMemberFunction &&other) = delete;

    int dispatch(lua_State *L)
    {
        std::tuple<TArgs...> args = rz::detail::load_args<TArgs...>(L);
        LOG_VERBOSE("arguments:" << args);
        TRet value = rz::detail::invoke(RZLuaFunctionBaseT<N, TRet, TArgs...>::function(), args);
        rz::detail::push(L, std::forward<TRet>(value));
        return N;
    }
};

template <typename... TArgs>
class RZLuaMemberFunction<0, void, TArgs...> : public RZLuaMemberFunctionBase<0, void, TArgs...>
{
public:
    RZLuaMemberFunction(lua_State *&l, const std::string &name, void(*fun)(TArgs...)) :
        RZLuaMemberFunction(l, name,
                            typename RZLuaFunctionBaseT<0, void, TArgs...>::type{fun})
    {
    }

    RZLuaMemberFunction(lua_State *&l,
                        std::string const &className,
                        const std::string &name,
                        typename RZLuaFunctionBaseT<0, void, TArgs...>::type fun) :
        RZLuaMemberFunctionBase<0, void, TArgs...>(l, className, name, fun, "void method")
    {
    }

    RZLuaMemberFunction(RZLuaMemberFunction const &other) = delete;
    RZLuaMemberFunction(RZLuaMemberFunction &&other) = delete;

    int dispatch(lua_State *L)
    {
        std::tuple<TArgs...> args = rz::detail::load_args<TArgs...>(L);
        LOG_VERBOSE("arguments:" << args);
        rz::detail::invoke(RZLuaFunctionBaseT<0, void, TArgs...>::function(), args);
        return 0;
    }
};
