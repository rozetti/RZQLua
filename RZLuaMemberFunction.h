#pragma once

#include "rz_lua_meta.h"
#include "RZLuaFunctionBase.h"

template <int N, typename TRet, typename... TArgs>
class RZLuaMemberFunctionBase : public RZLuaFunctionBaseT<N, TRet, TArgs...>
{
public:
    RZLuaMemberFunctionBase(lua_State *&l,
                 std::string const &className,
                 std::string const &functionName,
                 typename RZLuaFunctionBaseT<N, TRet, TArgs...>::type fun,
                 std::string const &desc) :
        RZLuaFunctionBaseT<N, TRet, TArgs...>(l, className, functionName, fun, desc)
    {
        lua_getglobal(l, className.c_str());
        lua_pushlightuserdata(l, (void *)static_cast<RZLuaFunctionBase *>(this));
        lua_pushcclosure(l, &dispatcher, 1);
        lua_setfield(l, -2, functionName.c_str());
        lua_pop(l, 1);
    }

//    RZLuaMemberFunctionBase(RZLuaMemberFunctionBase &&other) :
//        m_luaState(other.m_luaState),
//        m_className(other.m_className),
//        m_functionName(other.m_functionName),
//        m_function(other.m_function)
//    {
//        other.m_luaState = nullptr;
//        other.m_className = nullptr;
//        other.m_functionName = nullptr;
//        other.m_function = nullptr;
//    }
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

    RZLuaMemberFunction(const RZLuaMemberFunction &other) = delete;

    int dispatch(lua_State *L)
    {
        std::tuple<TArgs...> args = rz::detail::load_args<TArgs...>(L);
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

    RZLuaMemberFunction(const RZLuaMemberFunction &other) = delete;

    int dispatch(lua_State *L)
    {
        std::tuple<TArgs...> args = rz::detail::load_args<TArgs...>(L);
        rz::detail::invoke(RZLuaFunctionBaseT<0, void, TArgs...>::function(), args);
        return 0;
    }
};
