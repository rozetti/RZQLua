#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "rz_log.h"

#include "RZLuaInstanceBase.h"
#include "RZLuaMemberFunction.hpp"

struct lua_State;
class RZLuaCallableBase;

template <typename TClass>
class RZLuaInstance : public RZLuaInstanceBase
{
    TClass *m_nativeInstance;

public:
    template <typename... TArgs>
    void declare_function(const char *func_name,
                       void(TClass::*func)(TArgs...))
    {
        std::function<void(TArgs...)> closure = [this, func, func_name](TArgs... args)
        {
            LOG_VERBOSE("dispatching " << m_instanceName << "::" << func_name << "(...)");

            (m_nativeInstance->*func)(args...);
        };

        auto m = new RZLuaMemberFunction<0, void, TArgs...>
                (m_luaState, m_instanceName, std::string{func_name}, closure);

        m_functions.emplace_back(m);
    }

    template <typename TRet, typename... TArgs>
    void declare_function(const char *func_name,
                       TRet(TClass::*func)(TArgs...))
    {
        std::function<TRet(TArgs...)> closure = [this, func, func_name](TArgs... args)
        {
            LOG_VERBOSE("dispatching " << m_instanceName << "::" << func_name << "(...)");

            return (m_nativeInstance->*func)(args...);
        };

        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;
        auto m = new RZLuaMemberFunction<number_of_return_values, TRet, TArgs...>
                (m_luaState, m_instanceName, std::string{func_name}, closure);

        m_functions.emplace_back(m);
    }

    template <typename TRet, typename... TArgs, typename TBase>
    void declare_function(const char *func_name,
                          TRet(TBase::*func)(TArgs...),
                          typename std::enable_if<std::is_base_of<TBase, TClass>::value >::type* = 0)
    {
        std::function<TRet(TArgs...)> closure = [this, func, func_name](TArgs... args)
        {
            LOG_VERBOSE("dispatching " << m_instanceName << "::" << func_name << "(...)");

            return (m_nativeInstance->*func)(args...);
        };

        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;
        auto m = new RZLuaMemberFunction<number_of_return_values, TRet, TArgs...>
                (m_luaState, m_instanceName, std::string{func_name}, closure);

        m_functions.emplace_back(m);
    }

    template <typename TRet, typename... TArgs, typename TBase>
    void declare_function(const char *func_name,
                          TRet(TBase::*func)(TArgs...) const,
                          typename std::enable_if<std::is_base_of<TBase, TClass>::value >::type* = 0)
    {
        std::function<TRet(TArgs...)> closure = [this, func, func_name](TArgs... args)
        {
            LOG_VERBOSE("dispatching " << m_instanceName << "::" << func_name << "(...)");

            return (m_nativeInstance->*func)(args...);
        };

        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;
        auto m = new RZLuaMemberFunction<number_of_return_values, TRet, TArgs...>
                (m_luaState, m_instanceName, std::string{func_name}, closure);

        m_functions.emplace_back(m);
    }

    RZLuaInstance(lua_State *&L, TClass *instance, std::string const &instance_name) :
        RZLuaInstanceBase(L, instance_name),
        m_nativeInstance(instance)
    {
    }
};

