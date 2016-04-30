#include "RZLuaClass.hpp"

RZLuaClassBase::~RZLuaClassBase()
{
    LOG_DTOR("~RZLuaClassBase(" << m_className << ")");
}

RZLuaClassBase::RZLuaClassBase(lua_State *&state,
                               std::string const &class_name) :
    m_luaState(state),
    m_className(class_name)
{
    LOG_CTOR("RZLuaClassBase(" << class_name << ")");

    lua_createtable(m_luaState, 0, 0);

    luaL_newmetatable(m_luaState, (std::string("__mt:") + class_name).c_str());
    lua_setmetatable(m_luaState, -2);

    lua_pushstring(m_luaState, "__index");
    lua_pushvalue(m_luaState, -2);
    lua_settable(m_luaState, -3);

    lua_pushstring(m_luaState, "className");
    lua_pushstring(m_luaState, class_name.c_str());
    lua_settable(m_luaState, -3);

    lua_setglobal(m_luaState, m_className.c_str());
}

bool RZLuaClassBase::push_metatable()
{
    lua_getglobal(m_luaState, m_className.c_str());

    if (!lua_istable(m_luaState, -1))
    {
        LOG_ERROR("metatable '" << m_className << "' expected");
        return false;
    }

    return true;
}

bool RZLuaClassBase::push_table()
{
    lua_getglobal(m_luaState, m_className.c_str());

    if (!lua_istable(m_luaState, -1))
    {
        LOG_ERROR("metatable '" << m_className << "' expected");
        return false;
    }

    return true;
}

bool RZLuaClassBase::push_function(std::string const &name)
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

template <>
void RZLuaClass<QWebSocket>::declare_constant(std::string const &name, int value)
{
    LOG_VERBOSE("constant: " << name << "=" << value);

    push_table();

    lua_pushstring(m_luaState, name.c_str());
    lua_pushnumber(m_luaState, value);
    lua_settable(m_luaState, -3);

    lua_pop(m_luaState, 1);
}
