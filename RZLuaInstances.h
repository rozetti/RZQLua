#pragma once

#include <memory>
#include <string>
#include <map>

#include "rz_lua.h"
#include "rz_log.h"

#include "RZLuaExports_includes.h"
#include "RZLuaExports_ClassName.h"

#include "RZLuaInstance.hpp"

class RZLuaInstances
{
    std::map<std::string, std::unique_ptr<RZLuaInstanceBase> > m_instances;
    lua_State *&m_luaState;

public:
    RZLuaInstances(lua_State *&state) :
        m_luaState(state)
    {
        LOG_CTOR("RZLuaInstances ctor");
    }

    RZLuaInstances(RZLuaInstances const &other) = delete;
    RZLuaInstances(RZLuaInstances &&other) = delete;

    template <typename TClass>
    RZLuaInstance<TClass> &bind_instance(TClass *native_instance)
    {
        static int id = 0;

        auto L = m_luaState;

        auto ud = lua_newuserdata(L, sizeof(TClass *));
        auto pi = static_cast<TClass**>(ud);
        *pi = native_instance;

        auto class_name = ::ClassName<TClass>();

        std::stringstream ss;
        if (!strcmp("rz", class_name))
        {
            ss << class_name;
        }
        else
        {
            ss << class_name << ":" << ++id;
        }
        auto instance_name = ss.str();

        auto instance = new RZLuaInstance<TClass>{m_luaState, *pi, instance_name};

        auto up = std::unique_ptr<RZLuaInstance<TClass> >(instance);
        m_instances.insert(std::make_pair(instance_name, std::move(up)));

        lua_getglobal(L, instance_name.c_str());
        if (!lua_istable(L, -1))
        {
            return *instance; // todo crz
        }

        lua_pushstring(L, "instanceName");
        lua_pushstring(L, instance_name.c_str());
        lua_settable(L, -3);

        lua_setmetatable(L, -2);

        return *instance;
    }
};
