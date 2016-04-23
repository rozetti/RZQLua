#include "RZLua.h"

RZLua::RZLua() :
    m_luaState(luaL_newstate()),
    m_ownsState(true),
    m_classes(m_luaState),
    m_functions(m_luaState)
{
}

RZLua::RZLua(lua_State *state) :
    m_luaState(state),
    m_ownsState(false),
    m_classes(state),
    m_functions(state)
{
}

RZLua::~RZLua()
{
    if (m_luaState && m_ownsState)
    {
        lua_close(m_luaState);
        m_luaState = nullptr;
        m_ownsState = false;
    }
}

// todo crz: move this
int dispatcher(lua_State *L)
{
    auto upvalue = lua_touserdata(L, lua_upvalueindex(1));
    RZLuaFunctionBase *function = (RZLuaFunctionBase *)upvalue;
    return function->dispatch(L);
}
