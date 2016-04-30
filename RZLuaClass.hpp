#pragma once

#include <QString>

#include "rz.h"
#include "RZLuaFunctionBase.hpp"
#include "RZLuaFreeFunction.hpp"
#include "RZLuaMemberFunction.hpp"
#include <assert.h>

class RZLuaClassBase
{
protected:
    lua_State *&m_luaState;
    std::vector<std::unique_ptr<RZLuaFunctionBase>> m_functions;
    std::string m_className;

    RZLuaClassBase(lua_State *&state, std::string const &class_name);

public:
    virtual ~RZLuaClassBase();

    RZLuaClassBase(RZLuaClassBase &&other) = delete;
    RZLuaClassBase(RZLuaClassBase const &other) = delete;

    bool push_table();
    bool push_metatable();
    bool push_function(std::string const &name);
};

template <typename TClass>
class RZLuaClass : public RZLuaClassBase
{
public:
    void declare_constant(std::string const &name, int value);

    template <typename... TArgs>
    void declare_function(const char *func_name, void(*func)(TArgs...))
    {
        std::function<void(TArgs...)> closure = [this, func, func_name](TArgs... args)
        {
            LOG_VERBOSE("dispatching static function " << m_className << "::" << func_name << "(...)");

            (*func)(args...);
        };

        auto m = new RZLuaFreeFunction<0, void, TArgs...>
                (m_luaState, m_className, std::string{func_name}, closure);

        m_functions.emplace_back(m);
    }

    template <typename TRet, typename... TArgs>
    void declare_function(const char *func_name, TRet(*func)(TArgs...))
    {
        std::function<TRet(TArgs...)> closure = [this, func, func_name](TArgs... args)
        {
            LOG_VERBOSE("dispatching " << m_className << "::" << func_name << "(...)");

            return (*func)(args...);
        };

        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;
        auto m = new RZLuaFreeFunction<number_of_return_values, TRet, TArgs...>
                (m_luaState, m_className, std::string{func_name}, closure);

        m_functions.emplace_back(m);
    }

    RZLuaClass(lua_State *&L, std::string const &class_name) :
        RZLuaClassBase(L, class_name)
    {
    }
};

class RZLuaClasses
{
    std::map<std::string, std::unique_ptr<RZLuaClassBase> > m_classes;
    lua_State *&m_luaState;

public:
    RZLuaClasses(lua_State *&state) :
        m_luaState(state)
    {
        LOG_VERBOSE("RZLuaClasses ctor");
    }

    RZLuaClasses(RZLuaClasses const &other) = delete;
    RZLuaClasses(RZLuaClasses &&other) = delete;

    template <typename TClass>
    RZLuaClass<TClass> &declare_class(std::string const &class_name)
    {
        auto cl = new RZLuaClass<TClass>{m_luaState, class_name};

        auto tmp = std::unique_ptr<RZLuaClassBase>(cl);
        m_classes[class_name] = std::move(tmp);

        return *cl;
    }
};
