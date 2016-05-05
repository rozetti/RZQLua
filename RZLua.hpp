#pragma once

#include "RZLuaClass.hpp"
#include "RZLuaInstance.hpp"
#include "RZLuaFunctionBase.hpp"
#include "RZLuaFreeFunction.hpp"

#include <memory>
#include <map>
#include <vector>

struct lua_State;

#include "RZLuaInstances.h"

class RZLuaBase
{
    lua_State *m_luaOwnedState;

protected:
    lua_State *&m_luaState;
    RZLuaInstances m_instances;
    RZLuaFreeFunctions m_functions;
    RZLuaClasses m_classes;

    RZLuaBase();
    RZLuaBase(lua_State *&L);

public:
    ~RZLuaBase();

    operator lua_State *() const { return m_luaState; }

    void call(std::string const &function_name);

    lua_State * const &state() const { return m_luaState; }
    RZLuaFreeFunctions &functions() { return m_functions; }
    RZLuaInstances &instances() { return m_instances; }
    RZLuaClasses &classes() { return m_classes; }
};

template <class TImpl>
class RZLua : public RZLuaBase
{
    TImpl &m_impl;

public:
    RZLua(TImpl &impl) : RZLuaBase(),
        m_impl(impl)
    {
        LOG_VERBOSE("RZLua()");
    }

    RZLua(lua_State *&L, TImpl &impl) : RZLuaBase(L),
        m_impl(impl)
    {
        LOG_VERBOSE("RZLua(state)");
    }

    RZLua(RZLua &&other) = delete;
    RZLua(RZLua const &other) = delete;
    RZLua &operator=(RZLua const &other) = delete;

    void doFile(std::string const &filename)
    {
        LOG_VERBOSE("doFile(" << filename << ")");

        m_impl.doFile(filename);
    }
};

