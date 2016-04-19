#pragma once

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#include "RZLuaClass.h"
#include "RZLuaFunctionBase.h"
#include "RZLuaFreeFunction.h"

#include <memory>
#include <map>
#include <vector>

// todo crz: coalesce this lot

//struct RZLuaClassBase
//{
//    virtual ~RZLuaClassBase() {}
//    lua_State *m_luaState;
//    std::vector<std::unique_ptr<RZLuaFunctionBase>> m_functions;
//    std::string m_className;

//    RZLuaClassBase(lua_State *state,
//                   std::string const &class_name) :
//        m_luaState(state),
//        m_className(class_name)
//    {
//    }

//    RZLuaClassBase(RZLuaClassBase &&other) :
//        m_luaState(other.m_luaState),
//        m_functions(std::move(other.m_functions)),
//        m_className(other.m_className)
//    {
//        other.m_luaState = 0;
//        other.m_functions.clear();
//        other.m_className.clear();
//    }
//};

class RZLuaClasses
{
    std::map<std::string, std::unique_ptr<RZLuaClassBase>> m_instances;
    lua_State *m_luaState;

public:
    RZLuaClasses(lua_State *state) :
        m_luaState(state)
    {
    }

    RZLuaClasses(RZLuaClasses &&other) :
        m_instances(std::move(other.m_instances)),
        m_luaState(other.m_luaState)
    {
        other.m_luaState = nullptr;
    }

    template <typename T>
    RZLuaClass<T> &declare(const std::string &name, T &t)
    {
        auto cl = new RZLuaClass<T>{m_luaState, &t, name};

        auto tmp = std::unique_ptr<RZLuaClassBase>(cl);
        m_instances[name] = std::move(tmp);

        return *cl;
    }
};

class RZLuaFreeFunctions
{
    std::map<std::string, std::unique_ptr<RZLuaFunctionBase>> m_functions;
    lua_State *m_luaState;

public:
    RZLuaFreeFunctions(lua_State *state) :
        m_luaState(state)
    {
    }

    RZLuaFreeFunctions(RZLuaFreeFunctions &&other) :
        m_functions(std::move(other.m_functions)),
        m_luaState(other.m_luaState)
    {
        other.m_luaState = nullptr;
    }

    template <typename TRet, typename... TArgs>
    void declare(const std::string &name, std::function<TRet(TArgs...)> fun)
    {
        constexpr int arity = rz::detail::args_count<TRet>::value;

        auto f = new RZLuaFreeFunction<arity, TRet, TArgs...>(m_luaState, name, fun);
        auto ptr = std::unique_ptr<RZLuaFunctionBase>(f);

        m_functions[name] = std::move(ptr);
    }

    template <typename TRet, typename... TArgs>
    void declare(const std::string &name, TRet(*fun)(TArgs...))
    {
        constexpr int arity = rz::detail::args_count<TRet>::value;

        auto f = new RZLuaFreeFunction<arity, TRet, TArgs...>(m_luaState, name, fun);
        auto ptr = std::unique_ptr<RZLuaFunctionBase>(f);

        m_functions[name] = std::move(ptr);
    }
};

class RZLua
{
private:
    lua_State *m_luaState;
    bool m_ownsState;
    RZLuaFreeFunctions m_functions;
    RZLuaClasses m_classes;

public:
    ~RZLua();
    RZLua();
    RZLua(lua_State *);

    RZLua(RZLua &&other) :
        m_luaState(other.m_luaState),
        m_classes(std::move(other.m_classes)),
        m_functions(std::move(other.m_functions))
    {
        other.m_luaState = nullptr;
    }

    lua_State *state() const { return m_luaState; }
    RZLuaFreeFunctions &functions() { return m_functions; }
    RZLuaClasses &classes() { return m_classes; }

    RZLua(RZLua const &other) = delete;
    RZLua &operator=(RZLua const &other) = delete;
};

