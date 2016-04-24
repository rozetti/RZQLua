#pragma once

#include <string>

#include <QtGui/QOpenGLFunctions>

class RZQLua;
template <typename> class RZLuaInstance;

class RZQLuaExports
{
    RZQLua &m_qlua;
    QOpenGLFunctions m_gl;

public:
    RZQLuaExports(RZQLua &qlua);

    bool export_lib(std::string lib, std::string name);
    bool export_new_instance(std::string const &class_name, std::string const &instance_name);

private:
    bool export_gl(std::string const &name);
    bool export_debug(std::string const &name);

    template <typename TClass>
    void declare_instance_functions(RZLuaInstance<TClass> &instance);

    template <typename TClass>
    bool bind_instance(TClass *instance, const std::string &instance_name);
};
