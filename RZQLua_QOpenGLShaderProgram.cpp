#include "RZQLua_QOpenGLShaderProgram.h"

#include "rz_lua_exports.h"

#include "RZLuaExports.h"
#include "RZQLua.hpp"

#include <sstream>

RZQLua_QOpenGLShaderProgram::RZQLua_QOpenGLShaderProgram(QObject *parent) :
    QOpenGLShaderProgram(parent)
{
}

void RZQLua_QOpenGLShaderProgram::setAttributeArrayF(int location,
                                                     std::vector<float> floats,
                                                     int tupleSize,
                                                     int stride)
{
    m_floatsStorage = floats;

    QOpenGLShaderProgram::setAttributeArray(location,
                                            GL_FLOAT,
                                            m_floatsStorage.data(),
                                            tupleSize,
                                            stride);
}

static int ctor(lua_State *L)
{
    auto ex = (RZLuaExports *)lua_touserdata(L, lua_upvalueindex(1));

    LOG_CTOR("QOpenGLShaderProgram()");

    auto p = new RZQLua_QOpenGLShaderProgram(nullptr);

    /*auto instance =*/ ::bind_instance(*ex, p);

    return true;
}

template <>
int (*get_ctor<RZQLua_QOpenGLShaderProgram>())(lua_State*)
{
    return ctor;
}

template<>
void declare_instance_functions(RZLuaInstance<RZQLua_QOpenGLShaderProgram> &instance)
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

template<>
void RZLuaExports::declare_class_symbols(RZLuaClass<RZQLua_QOpenGLShaderProgram> &/*clazz*/)
{
}
