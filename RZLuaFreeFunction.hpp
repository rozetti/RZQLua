#pragma once

#include "RZLuaFunctionBase.hpp"

#include <map>

template <int N, typename TRet, typename... TArgs>
class RZLuaFreeFunctionBase : public RZLuaFunctionBaseT<N, TRet, TArgs...>
{
public:
    RZLuaFreeFunctionBase(lua_State *&l,
                 std::string const &className,
                 std::string const &functionName,
                 typename RZLuaFunctionBaseT<N, TRet, TArgs...>::type fun,
             std::string const &desc) :
        RZLuaFunctionBaseT<N, TRet, TArgs...>(l, className, functionName, fun, desc)
    {
        lua_pushlightuserdata(l, (void *)static_cast<RZLuaCallableBase *>(this));
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

    int dispatch(lua_State *L)
    {
        std::tuple<TArgs...> args = rz::detail::load_args<TArgs...>(L);
        LOG_VERBOSE("arguments:" << args);
        TRet value = rz::detail::invoke(RZLuaFunctionBaseT<0, void, TArgs...>::function(), args);
        push(L, std::forward<TRet>(value));
        return N;
    }
};

template <typename... TArgs>
class RZLuaFreeFunction<0, void, TArgs...> : public RZLuaFreeFunctionBase<0, void, TArgs...>
{
public:
    RZLuaFreeFunction(lua_State *&l, const std::string &name, void(*fun)(TArgs...)) :
        RZLuaFreeFunction<0, void, TArgs...>(l, name,
                                             typename RZLuaFunctionBaseT<0, void, TArgs...>::type{fun}) {}

    RZLuaFreeFunction(lua_State *&l,
                      const std::string &functionName,
                      typename RZLuaFunctionBaseT<0, void, TArgs...>::type fun) :
        RZLuaFreeFunctionBase<0, void, TArgs...>(l, "", functionName, fun, "void free function") {}

    RZLuaFreeFunction(const RZLuaFreeFunction &other) = delete;

    int dispatch(lua_State *L)
    {
        std::tuple<TArgs...> args = rz::detail::load_args<TArgs...>(L);
        LOG_VERBOSE("arguments:" << args);
        rz::detail::invoke(RZLuaFunctionBaseT<0, void, TArgs...>::function(), args);
        return 0;
    }
};

class RZLuaFreeFunctions
{
    std::map<std::string, std::unique_ptr<RZLuaCallableBase>> m_functions;
    lua_State *&m_luaState;

public:
    RZLuaFreeFunctions(lua_State *&state) :
        m_luaState(state)
    {
        LOG_VERBOSE("RZLuaFreeFunctions ctor");
    }

    RZLuaFreeFunctions(RZLuaFreeFunctions &&other) = delete;
    RZLuaFreeFunctions(RZLuaFreeFunctions const &other) = delete;

    template <typename TRet, typename... TArgs>
    void declare(std::string const &name, std::function<TRet(TArgs...)> fun)
    {
        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;

        auto f = new RZLuaFreeFunction<number_of_return_values, TRet, TArgs...>(m_luaState, name, fun);
        auto ptr = std::unique_ptr<RZLuaCallableBase>(f);

        m_functions[name] = std::move(ptr);
    }

    template <typename TRet, typename... TArgs>
    void declare(std::string const &name, TRet(*fun)(TArgs...))
    {
        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;

        auto f = new RZLuaFreeFunction<number_of_return_values, TRet, TArgs...>(m_luaState, name, fun);
        auto ptr = std::unique_ptr<RZLuaCallableBase>(f);

        m_functions[name] = std::move(ptr);
    }
};

