#pragma once

#include <string>
#include <vector>
#include <memory>

class RZLuaCallableBase;
struct lua_State;

class RZLuaInstanceBase
{
protected:
    lua_State *&m_luaState;
    std::vector<std::unique_ptr<RZLuaCallableBase> > m_functions;
    std::string m_instanceName;

    RZLuaInstanceBase(lua_State *&state, std::string const &instance_name);

    RZLuaInstanceBase(RZLuaInstanceBase const &other) = delete;
    RZLuaInstanceBase(RZLuaInstanceBase &&other) = delete;

public:
    virtual ~RZLuaInstanceBase();

    bool push_metatable();
    bool push_function(std::string const &name);
};

