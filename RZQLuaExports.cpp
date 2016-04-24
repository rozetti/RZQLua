#include "RZQLuaExports.h"
#include "RZQLua.h"
#include "RZQLua_QOpenGLShaderProgram.h"

RZQLuaExports::RZQLuaExports(RZQLua &qlua) :
    m_qlua(qlua)
{
}

bool RZQLuaExports::export_lib(std::string lib, std::string name)
{
    if ("gles20" == lib)
    {
        return export_gl(name);
    }

    if ("debug" == lib)
    {
        return export_debug(name);
    }

    return false;
}

template<typename TClass>
bool RZQLuaExports::bind_instance(TClass *instance, std::string const &instance_name)
{
    auto const L = m_qlua.lua().state();

    auto &instance_meta = m_qlua.lua().instances().bind_instance(instance_name, instance);

    declare_instance_functions(instance_meta);
    *static_cast<TClass**>(lua_newuserdata(L, sizeof(TClass))) = instance;

    lua_getglobal(L, instance_name.c_str());
    if (!lua_istable(L, -1))
    {
        return false;
    }

    lua_pushstring(L, "instanceName");
    lua_pushstring(L, instance_name.c_str());
    lua_settable(L, -3);

    lua_setmetatable(L, -2);

    return true;
}

bool RZQLuaExports::export_new_instance(std::string const &class_name, std::string const &instance_name)
{
    if ("QOpenGLShaderProgram" == class_name)
    {
        return bind_instance(new RZQLua_QOpenGLShaderProgram(nullptr), instance_name);
    }

    return false;
}

bool RZQLuaExports::export_gl(std::string const &name)
{
    m_gl.initializeOpenGLFunctions();

#define RZQLUA_DECLARE_GL_FUNCTION(F) gl.declare_function(#F, &QOpenGLFunctions::F);

    auto &gl = m_qlua.lua().instances().bind_instance(name, &m_gl);

    RZQLUA_DECLARE_GL_FUNCTION(glClearColor);
    RZQLUA_DECLARE_GL_FUNCTION(glClear);
    RZQLUA_DECLARE_GL_FUNCTION(glEnable);
    RZQLUA_DECLARE_GL_FUNCTION(glBlendFunc);
    RZQLUA_DECLARE_GL_FUNCTION(glDrawArrays);
    RZQLUA_DECLARE_GL_FUNCTION(glDisable);
    RZQLUA_DECLARE_GL_FUNCTION(glViewport);

    return true;
}

void debug(QString message)
{
    qDebug() << message;
}

bool RZQLuaExports::export_debug(std::string const &name)
{
    m_qlua.lua().functions().declare("debug", &debug);

    return true;
}

template<>
void RZQLuaExports::declare_instance_functions(RZLuaInstance<RZQLua_QOpenGLShaderProgram> &instance)
{
    instance.declare_function("bind", &QOpenGLShaderProgram::bind);
    instance.declare_function("release", &QOpenGLShaderProgram::release);
    instance.declare_function("enableAttributeArray", (void (QOpenGLShaderProgram::*)(int))&QOpenGLShaderProgram::enableAttributeArray);
    instance.declare_function("disableAttributeArray", (void (QOpenGLShaderProgram::*)(int))&QOpenGLShaderProgram::disableAttributeArray);

    instance.declare_function("addShaderFromSourceCode", (bool (QOpenGLShaderProgram::*)(QOpenGLShader::ShaderType, char const *))&QOpenGLShaderProgram::addShaderFromSourceCode);
    instance.declare_function("bindAttributeLocation", (void (QOpenGLShaderProgram::*)(char const *, int))&QOpenGLShaderProgram::bindAttributeLocation);
    instance.declare_function("link", &QOpenGLShaderProgram::link);

    instance.declare_function("setUniformValueF", (void (QOpenGLShaderProgram::*)(char const *, float))&QOpenGLShaderProgram::setUniformValue);
    instance.declare_function("setAttributeArrayF", &RZQLua_QOpenGLShaderProgram::setAttributeArrayF);
}

