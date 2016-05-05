#include "RZLuaCallableBase.h"

#include "rz_log.h"
#include "rz_lua.h"

#include <sstream>

RZLuaCallableBase::RZLuaCallableBase(lua_State *&L, const std::string &className, const std::string &functionName, const std::string &desc) :
    m_luaState(L),
    m_className(className),
    m_functionName(functionName),
    m_desc(desc)
{
    LOG_CTOR("RZLuaCallableBase(" << className
             << "::" << functionName
             << ") (" << desc << ")");
}

std::string RZLuaCallableBase::getDesc() const
{
    std::stringstream ss;
    ss << m_desc << m_className << "::" << m_functionName;
    return ss.str();
}

RZLuaCallableBase::~RZLuaCallableBase()
{
    LOG_DTOR("~RZLuaCallableBase("
             << m_className << "::"
             << m_functionName << ")");

    if (nullptr != m_luaState)
    {
        lua_pushnil(m_luaState);
        lua_setglobal(m_luaState, m_functionName.c_str());
    }
}
