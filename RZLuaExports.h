#pragma once

#include <string>

struct lua_State;
class RZQLua;
template <typename> class RZLuaClass;
class RZLuaInstances;

class RZLuaExports
{
    RZQLua &m_qlua;

public:
    RZLuaExports(RZQLua &qlua);

    bool export_lib(std::string lib);
    bool export_lib_with_name(std::string lib, std::string name);

    RZLuaInstances &instances();

    lua_State *lua_state();

private:
    bool export_gl(std::string const &name);
    bool export_debug(std::string const &name);

    template <typename TClass>
    bool export_class(std::string const &name);

    template <typename TClass>
    void declare_class_symbols(RZLuaClass<TClass> &clazz);
};

