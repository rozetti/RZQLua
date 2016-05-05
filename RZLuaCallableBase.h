#pragma once

#include <string>

struct lua_State;

int dispatcher(lua_State *L);

class RZLuaCallableBase
{
    lua_State *&m_luaState;
    std::string m_className;
    std::string m_functionName;
    std::string m_desc;

public:

    RZLuaCallableBase(lua_State *&L,
                      std::string const &className,
                      std::string const &functionName,
                      std::string const &desc);

    RZLuaCallableBase(RZLuaCallableBase &&other) = delete;
    RZLuaCallableBase(RZLuaCallableBase const &other) = delete;

    std::string getDesc() const;

    virtual int dispatch(lua_State *) = 0;

    virtual ~RZLuaCallableBase();
};
