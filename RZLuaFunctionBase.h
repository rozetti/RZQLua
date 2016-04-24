#pragma once

#include "rz_lua_meta.h"

int dispatcher(lua_State *L);

class RZLuaFunctionBase
{
    lua_State *m_luaState;
    std::string m_className;
    std::string m_functionName;
    std::string m_desc;

public:
    RZLuaFunctionBase() :
        m_luaState(nullptr)
    {
    }

    RZLuaFunctionBase(lua_State *L,
            std::string const &className,
            std::string const &functionName,
            std::string const &desc) :
        m_luaState(L),
        m_className(className),
        m_functionName(functionName),
        m_desc(desc)
    {
    }

    RZLuaFunctionBase(RZLuaFunctionBase &&other) :
            m_luaState(other.m_luaState),
            m_className(other.m_className),
            m_functionName(other.m_functionName),
            m_desc(other.m_desc)
    {
        other.m_luaState = 0;
    }

    std::string getDesc() const
    {
        std::stringstream ss;
        ss << m_desc << m_className << "::" << m_functionName;
        return ss.str();
    }

    virtual int dispatch(lua_State *) = 0;

    virtual ~RZLuaFunctionBase()
    {
        if (nullptr != m_luaState)
        {
            lua_pushnil(m_luaState);
            lua_setglobal(m_luaState, m_functionName.c_str());
        }
    }
};

template <int N, typename TRet, typename... TArgs>
class RZLuaFunctionBaseT : public RZLuaFunctionBase
{
protected:
    typedef std::function<TRet(TArgs...)> type;
    type m_function;

public:
    RZLuaFunctionBaseT(lua_State *L,
                 std::string const &className,
                 std::string const &functionName,
                 type function,
             std::string const &desc) :
        RZLuaFunctionBase(L, className, functionName, desc), m_function(function)
    {
    }

    RZLuaFunctionBaseT(RZLuaFunctionBaseT &&other) :
        RZLuaFunctionBase(other),
        m_function(other.m_function)
    {
    }

    type function() const { return m_function; }
};





