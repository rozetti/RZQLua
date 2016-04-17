#pragma once

#include <QDebug>
#include "RZLuaFunctionBase.h"
#include "RZLuaMemberFunction.h"

template <typename T, typename... Funs>
class RZLuaClass : public RZLuaClassBase
{
private:
    T *m_nativeClass;

public:
    template <typename... Args>
    void declare_function(const char *fun_name,
                       void(T::*func)(Args...))
    {
        std::function<void(Args...)> closure = [this, func](Args... args)
        {
            (m_nativeClass->*func)(args...);
        };

        auto m = new RZLuaMemberFunction<0, void, Args...>
                (m_luaState, m_className, std::string{fun_name}, closure);

        m_functions.emplace_back(m);
    }

    template <typename Ret, typename... Args>
    void declare_function(const char *func_name,
                       Ret(T::*func)(Args...))
    {
        std::function<Ret(Args...)> closure = [this, func](Args... args)
        {
            return (m_nativeClass->*func)(args...);
        };

        constexpr int arity = rz::detail::args_count<Ret>::value;
        auto m = new RZLuaMemberFunction<arity, Ret, Args...>
                (m_luaState, m_className, std::string{func_name}, closure);

        m_functions.emplace_back(m);
    }

    void declare_functions() {}

    template <typename F, typename... Fs>
    void declare_functions(const char *name,
                        F func,
                        Fs... funcs)
    {
        declare_function(name, func);
        declare_functions(funcs...);
    }

    template <typename T, typename F, typename... Funs>
    RZLuaClass(lua_State *state,
               T *t,
               std::string const &class_name,
               std::string const &name,
               F func,
               Funs... funcs) :
        RZLuaClassBase(state, class_name),
        m_nativeClass(t)
    {
        lua_createtable(state, 0, 0);
        lua_setglobal(state, class_name.c_str());

        declare_functions(name.c_str(), func, funcs...);
    }

    template <typename T>
    RZLuaClass(lua_State *state,
               T *t,
               std::string const &class_name) :
        RZLuaClassBase(state, class_name),
        m_nativeClass(t)
    {
        lua_createtable(state, 0, 0);
        lua_setglobal(state, class_name.c_str());
    }
};

