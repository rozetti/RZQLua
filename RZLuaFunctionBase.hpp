#pragma once

#include "rz_lua_meta.h"

#include "RZLuaCallableBase.h"

template <int N, typename TRet, typename... TArgs>
class RZLuaFunctionBaseT : public RZLuaCallableBase
{
protected:
    typedef std::function<TRet(TArgs...)> type;
    type m_function;

public:
    RZLuaFunctionBaseT(lua_State *&L,
                       std::string const &className,
                       std::string const &functionName,
                       type function,
                       std::string const &desc) :
        RZLuaCallableBase(L, className, functionName, desc),
        m_function(function)
    {
    }

    RZLuaFunctionBaseT(RZLuaFunctionBaseT &&other) :
        RZLuaCallableBase(other),
        m_function(other.m_function)
    {
    }

    type function() const { return m_function; }
};





