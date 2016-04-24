#pragma once

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#include "RZLuaClass.h"
#include "RZLuaFunctionBase.h"
#include "RZLuaFreeFunction.h"

#include <memory>
#include <map>
#include <vector>

class RZLuaInstances
{
    std::map<std::string, std::unique_ptr<RZLuaInstanceBase> > m_instances;
    lua_State *m_luaState;

public:
    RZLuaInstances(lua_State *state) :
        m_luaState(state)
    {
    }

    RZLuaInstances(RZLuaInstances &&other) :
        m_instances(std::move(other.m_instances)),
        m_luaState(other.m_luaState)
    {
        other.m_luaState = nullptr;
    }

    template <typename TClass>
    RZLuaInstance<TClass> &bind_instance(std::string const &instance_name, TClass *t)
    {
        auto instance = new RZLuaInstance<TClass>{m_luaState, t, instance_name};

        auto tmp = std::unique_ptr<RZLuaInstanceBase>(instance);
        m_instances[instance_name] = std::move(tmp);

        return *instance;
    }
};

class RZLuaFreeFunctions
{
    std::map<std::string, std::unique_ptr<RZLuaFunctionBase>> m_functions;
    lua_State *m_luaState;

public:
    RZLuaFreeFunctions(lua_State *state) :
        m_luaState(state)
    {
    }

    RZLuaFreeFunctions(RZLuaFreeFunctions &&other) :
        m_functions(std::move(other.m_functions)),
        m_luaState(other.m_luaState)
    {
        other.m_luaState = nullptr;
    }

    template <typename TRet, typename... TArgs>
    void declare(std::string const &name, std::function<TRet(TArgs...)> fun)
    {
        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;

        auto f = new RZLuaFreeFunction<number_of_return_values, TRet, TArgs...>(m_luaState, name, fun);
        auto ptr = std::unique_ptr<RZLuaFunctionBase>(f);

        m_functions[name] = std::move(ptr);
    }

    template <typename TRet, typename... TArgs>
    void declare(std::string const &name, TRet(*fun)(TArgs...))
    {
        constexpr int number_of_return_values = rz::detail::type_list_size<TRet>::value;

        auto f = new RZLuaFreeFunction<number_of_return_values, TRet, TArgs...>(m_luaState, name, fun);
        auto ptr = std::unique_ptr<RZLuaFunctionBase>(f);

        m_functions[name] = std::move(ptr);
    }
};

class RZLua
{
private:
    lua_State *m_luaState;
    bool m_ownsState;
    RZLuaInstances m_instances;
    RZLuaFreeFunctions m_functions;

public:
    ~RZLua();
    RZLua();
    RZLua(lua_State *);

    RZLua(RZLua &&other) :
        m_luaState(other.m_luaState),
        m_instances(std::move(other.m_instances)),
        m_functions(std::move(other.m_functions))
    {
        other.m_luaState = nullptr;
    }

    lua_State *state() const { return m_luaState; }
    RZLuaFreeFunctions &functions() { return m_functions; }
    RZLuaInstances &instances() { return m_instances; }

    RZLua(RZLua const &other) = delete;
    RZLua &operator=(RZLua const &other) = delete;
};

