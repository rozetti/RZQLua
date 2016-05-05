#include "RZLua.hpp"

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

RZLuaBase::RZLuaBase() :
    m_luaOwnedState(luaL_newstate()),
    m_luaState(m_luaOwnedState),
    m_instances(m_luaOwnedState),
    m_functions(m_luaOwnedState),
    m_classes(m_luaOwnedState)
{
    LOG_VERBOSE("RZLua()");

    luaL_openlibs(m_luaOwnedState);
}

RZLuaBase::RZLuaBase(lua_State *&L) :
    m_luaOwnedState(nullptr),
    m_luaState(L),
    m_instances(L),
    m_functions(L),
    m_classes(L)
{
    LOG_VERBOSE("RZLua(state)");
}

RZLuaBase::~RZLuaBase()
{
    if (m_luaOwnedState)
    {
        lua_close(m_luaOwnedState);
        m_luaOwnedState = nullptr;
    }
}

int dispatcher(lua_State *L)
{
    if (!lua_isuserdata(L, lua_upvalueindex(1)))
    {
        LOG_ERROR("should be some userdata");
        return 0;
    }

    auto upvalue = lua_touserdata(L, lua_upvalueindex(1));
    RZLuaCallableBase *function = (RZLuaCallableBase *)upvalue;
    return function->dispatch(L);
}

void RZLuaBase::call(std::string const &function_name)
{
    auto const &L = m_luaState;

    lua_getglobal(L, function_name.c_str());
    if (!lua_isfunction(L, -1))
    {
        assert(false);
        return;
    }

    lua_pcall(L, 0, 0, 0);
}
