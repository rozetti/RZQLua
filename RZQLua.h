#pragma once

#include "RZLua.h"
#include "RZQLuaExports.h"
#include "rz_qlua_meta.h"

#include <QtGui/QOpenGLFunctions>
//#include <QtGui/QOpenGLShaderProgram>

class RZQLua_QOpenGLShaderProgram;

class RZQLua
{
    RZLua m_lua;
    RZQLuaExports m_exports;
    QOpenGLFunctions m_gl;

public:
    RZQLua();
    RZQLua(QString const &file);
    ~RZQLua() {}

    RZLua &lua() { return m_lua; }

    template <typename... TArgs>
    QVariantList vcall(QString const &function, TArgs...);

    QVariantList call(QString const &function, QVariantList const &args);
    void call(const QString &function);

    void doFile(QString const &filename);

    void declare_debug(const QString &name);
    void declare_gl(const QString &name);
    void declare_shader_program_instance(const QString &name, RZQLua_QOpenGLShaderProgram &program);
    void declare_QOpenGLShaderProgram();
    void new_QOpenGLShaderProgram();
};

template <typename... TArgs>
QVariantList RZQLua::vcall(QString const &function, TArgs... args)
{
    QVariantList list;
    rz::detail::build_args_list(list, args...);
    return call(function, list);
}

