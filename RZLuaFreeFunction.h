#pragma once

#include <QDebug>

#include "RZLuaFunctionBase.h"

template <int N, typename TRet, typename... TArgs>
class RZLuaFreeFunctionBase : public RZLuaFunctionBaseT<N, TRet, TArgs...>
{
public:
    RZLuaFreeFunctionBase(lua_State *l,
                 std::string const &className,
                 std::string const &functionName,
                 typename RZLuaFunctionBaseT<N, TRet, TArgs...>::type fun,
             std::string const &desc) :
        RZLuaFunctionBaseT<N, TRet, TArgs...>(l, className, functionName, fun, desc)
    {
        lua_pushlightuserdata(l, (void *)static_cast<RZLuaFunctionBase *>(this));
        lua_pushcclosure(l, &dispatcher, 1);
        lua_setglobal(l, functionName.c_str());
    }
};

template <int N, typename TRet, typename... TArgs>
class RZLuaFreeFunction : public RZLuaFreeFunctionBase<N, TRet, TArgs...>
{
public:
    RZLuaFreeFunction(lua_State *&l, const std::string &name, TRet(*fun)(TArgs...)) :
        RZLuaFreeFunction<N, TRet, TArgs...>(l, name,
                                             typename RZLuaFreeFunction<N, TRet, TArgs...>::type{fun}) {}

    RZLuaFreeFunction(lua_State *&l, const std::string &functionName,
                      typename RZLuaFreeFunction<N, TRet, TArgs...>::type fun) :
        RZLuaFreeFunctionBase<N, TRet, TArgs...>(l, "", functionName, fun, "non-void free function") {}

    RZLuaFreeFunction(const RZLuaFreeFunction &other) = delete;

    int dispatch(lua_State *l)
    {
        std::tuple<TArgs...> args = rz::detail::load_args<TArgs...>(l);
        TRet value = rz::detail::invoke(RZLuaFunctionBaseT<0, void, TArgs...>::function(), args);
        push(l, std::forward<TRet>(value));
        return N;
    }
};

template <typename... TArgs>
class RZLuaFreeFunction<0, void, TArgs...> : public RZLuaFreeFunctionBase<0, void, TArgs...>
{
public:
    RZLuaFreeFunction(lua_State *l, const std::string &name, void(*fun)(TArgs...)) :
        RZLuaFreeFunction<0, void, TArgs...>(l, name,
                                             typename RZLuaFunctionBaseT<0, void, TArgs...>::type{fun}) {}

    RZLuaFreeFunction(lua_State *l,
                      const std::string &functionName,
                      typename RZLuaFunctionBaseT<0, void, TArgs...>::type fun) :
        RZLuaFreeFunctionBase<0, void, TArgs...>(l, "", functionName, fun, "void free function") {}

    RZLuaFreeFunction(const RZLuaFreeFunction &other) = delete;

    int dispatch(lua_State *l)
    {
        std::tuple<TArgs...> args = rz::detail::load_args<TArgs...>(l);
        rz::detail::invoke(RZLuaFunctionBaseT<0, void, TArgs...>::function(), args);
        return 0;
    }
};

