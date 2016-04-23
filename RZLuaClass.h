#pragma once

#include <QDebug>
#include "RZLuaFunctionBase.h"
#include "RZLuaMemberFunction.h"
#include <assert.h>

struct RZLuaClassBase
{
    virtual ~RZLuaClassBase() {}
    lua_State *m_luaState;
    std::vector<std::unique_ptr<RZLuaFunctionBase>> m_functions;
    std::string m_className;

    RZLuaClassBase(lua_State *state,
                   std::string const &class_name) :
        m_luaState(state),
        m_className(class_name)
    {
    }

    RZLuaClassBase(RZLuaClassBase &&other) :
        m_luaState(other.m_luaState),
        m_functions(std::move(other.m_functions)),
        m_className(other.m_className)
    {
        other.m_luaState = 0;
        other.m_functions.clear();
        other.m_className.clear();
    }
};

template <typename TClass>
class RZLuaClass : public RZLuaClassBase
{
private:
    TClass *m_nativeClass;

public:
    template <typename... TArgs>
    void declare_function(const char *fun_name,
                       void(TClass::*func)(TArgs...))
    {
        std::function<void(TArgs...)> closure = [this, func](TArgs... args)
        {
            (m_nativeClass->*func)(args...);
        };

        auto m = new RZLuaMemberFunction<0, void, TArgs...>
                (m_luaState, m_className, std::string{fun_name}, closure);

        m_functions.emplace_back(m);
    }

    template <typename TRet, typename... TArgs>
    void declare_function(const char *func_name,
                       TRet(TClass::*func)(TArgs...))
    {
        std::function<TRet(TArgs...)> closure = [this, func](TArgs... args)
        {
            return (m_nativeClass->*func)(args...);
        };

        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;
        auto m = new RZLuaMemberFunction<number_of_return_values, TRet, TArgs...>
                (m_luaState, m_className, std::string{func_name}, closure);

        m_functions.emplace_back(m);
    }

    template <typename TRet, typename... TArgs, typename TBase>
    void declare_function(const char *func_name,
                          TRet(TBase::*func)(TArgs...),
                          typename std::enable_if<std::is_base_of<TBase, TClass>::value >::type* dummy = 0)
    {
        std::function<TRet(TArgs...)> closure = [this, func](TArgs... args)
        {
            return (m_nativeClass->*func)(args...);
        };

        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;
        auto m = new RZLuaMemberFunction<number_of_return_values, TRet, TArgs...>
                (m_luaState, m_className, std::string{func_name}, closure);

        m_functions.emplace_back(m);
    }

    RZLuaClass(lua_State *L, TClass *c, std::string const &class_name) :
        RZLuaClassBase(L, class_name),
        m_nativeClass(c)
    {
        lua_createtable(L, 0, 0);
        lua_setglobal(L, class_name.c_str());
    }

    RZLuaClass(lua_State *L, TClass *c, std::string const &class_name, std::string const &metatable_name) :
        RZLuaClassBase(L, class_name),
        m_nativeClass(c)
    {
        lua_getglobal(L, metatable_name.c_str());
        if (!lua_istable(L, -1))
        {
            assert(false);
        }

        lua_createtable(L, 0, 0);
        lua_setmetatable(L, -2);

        lua_setglobal(L, class_name.c_str());

        lua_pop(L, 1);
    }
};

