#pragma once

#include <string>

#include <QtGui/QOpenGLFunctions>

struct lua_State;
class RZQLua;
template <typename> class RZLuaInstance;
template <typename> class RZLuaClass;

class RZQLuaExports
{
    RZQLua &m_qlua;

public:
    RZQLuaExports(RZQLua &qlua);

    bool export_lib(std::string lib);
    bool export_lib_with_name(std::string lib, std::string name);
//    bool export_new_instance(std::string const &class_name, std::string const &instance_name);

    template <typename TClass>
    RZLuaInstance<TClass> *bind_instance(TClass *instance);

    template <typename TClass>
    int (*get_ctor())(lua_State*);

private:
    int new_instance_id();

    bool export_gl(std::string const &name);
    bool export_debug(std::string const &name);

    template <typename TClass>
    bool export_class(std::string const &name);

    template <typename TClass>
    void declare_instance_functions(RZLuaInstance<TClass> &instance);

    template <typename TClass>
    void declare_class_symbols(RZLuaClass<TClass> &clazz);
};

template <typename TClass> char const *ClassName();

#define RZ_LUA_DECLARE_CLASS(C) \
template <> char const *ClassName<C>() { return #C; } \
extern template void RZQLuaExports::declare_instance_functions(RZLuaInstance<C> &instance); \
template RZLuaInstance<C> *RZQLuaExports::bind_instance(C*); \
extern template int (*RZQLuaExports::get_ctor<C>())(lua_State*);
