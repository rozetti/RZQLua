#include "RZLuaInstance.hpp"

RZLuaInstanceBase::~RZLuaInstanceBase()
{
    LOG_DTOR("~RZLuaInstanceBase(" << m_instanceName << ")");
}

RZLuaInstanceBase::RZLuaInstanceBase(lua_State *&state,
                                     std::string const &instance_name) :
    m_luaState(state),
    m_instanceName(instance_name)
{
    LOG_CTOR("RZLuaInstanceBase(" << instance_name << ")");

    lua_createtable(m_luaState, 0, 0);

    lua_pushstring(m_luaState, "__index");
    lua_pushvalue(m_luaState, -2);
    lua_settable(m_luaState, -3);

    lua_pushstring(m_luaState, "__newindex");
    lua_pushvalue(m_luaState, -2);
    lua_settable(m_luaState, -3);

    lua_setglobal(m_luaState, m_instanceName.c_str());
}

bool RZLuaInstanceBase::push_metatable()
{
    lua_getglobal(m_luaState, m_instanceName.c_str());

    if (!lua_istable(m_luaState, -1))
    {
        LOG_ERROR("metatable '" << m_instanceName << "' expected");
        return false;
    }

    return true;
}

bool RZLuaInstanceBase::push_function(std::string const &name)
{
    push_metatable();

    lua_getfield(m_luaState, -1, name.c_str());

    if (!lua_isfunction(m_luaState, -1))
    {
        LOG_ERROR("function '" << name << "' expected");
        return false;
    }

    return true;
}

