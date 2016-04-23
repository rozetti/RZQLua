#include "RZQLua.h"

#include <QtGui/QOpenGLFunctions>
#include "RZQLua_QOpenGLShaderProgram.h"

#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QVariantList>
#include <QString>
#include <QDebug>

#include <assert.h>

#include "rz_qlua_meta.h"

template<typename TClass>
void create_object(lua_State *L)
{
    auto object = new TClass(nullptr);
    *static_cast<TClass**>(lua_newuserdata(L, sizeof(TClass))) = object;
}

int new_object(lua_State *L)
{
    auto class_name = lua_tostring(L, -1);
    lua_getglobal(L, class_name);
    if (!lua_istable(L, -1))
    {
        return 0;
    }

    if (!strcmp("QOpenGLShaderProgram", class_name))
    {
        create_object<QOpenGLShaderProgram>(L);
    }
    else
    {
        return 0;
    }

    lua_setmetatable(L, -2);
    return 1;
}

RZQLua::RZQLua() :
    m_exports(*this)
{
    m_gl.initializeOpenGLFunctions();

#define RZQLUA_DECLARE_RZ_FUNCTION(F) rz.declare_function(#F, &RZQLuaExports::F);

    auto &rz = m_lua.classes().declare("rz", m_exports);
    rz.declare_function("import", &RZQLuaExports::import);

    lua_pushcclosure(m_lua.state(), new_object, 0);
    lua_setglobal(m_lua.state(), "new");
}

RZQLua::RZQLua(const QString &file) :
    RZQLua()
{
    m_gl.initializeOpenGLFunctions();

    doFile(file);
}

struct Class
{

};

RZQLua_QOpenGLShaderProgram _QOpenGLShaderProgram(nullptr);

void RZQLua::declare_QOpenGLShaderProgram()
{
    auto &program = m_lua.classes().declare("QOpenGLShaderProgram", _QOpenGLShaderProgram);

#define RZQLUA_DECLARE_GLSP_FUNCTION(F) program.declare_function(#F, &RZQLua_QOpenGLShaderProgram::F);
#define RZQLUA_DECLARE_GLSP_FUNCTION_OVERLOAD(F, S) program.declare_function(#F, (S)&RZQLua_QOpenGLShaderProgram::F);
#define RZQLUA_DECLARE_GLSP_FUNCTION_OVERLOAD_T(F, T, S) program.declare_function(#T, (S)&RZQLua_QOpenGLShaderProgram::F);

    program.declare_function("bind", &QOpenGLShaderProgram::bind);
    program.declare_function("release", &QOpenGLShaderProgram::release);
    program.declare_function("enableAttributeArray", (void (QOpenGLShaderProgram::*)(int))&QOpenGLShaderProgram::enableAttributeArray);
    program.declare_function("disableAttributeArray", (void (QOpenGLShaderProgram::*)(int))&QOpenGLShaderProgram::disableAttributeArray);

    program.declare_function("addShaderFromSourceCode", (bool (QOpenGLShaderProgram::*)(QOpenGLShader::ShaderType, char const *))&QOpenGLShaderProgram::addShaderFromSourceCode);
    program.declare_function("bindAttributeLocation", (void (QOpenGLShaderProgram::*)(char const *, int))&QOpenGLShaderProgram::bindAttributeLocation);
    program.declare_function("link", &QOpenGLShaderProgram::link);

    program.declare_function("setUniformValueF", (void (QOpenGLShaderProgram::*)(char const *, float))&QOpenGLShaderProgram::setUniformValue);
    program.declare_function("setAttributeArrayF", &RZQLua_QOpenGLShaderProgram::setAttributeArrayF);
}

void RZQLua::declare_shader_program_instance(QString const &name, RZQLua_QOpenGLShaderProgram &_program)
{
    auto &program = m_lua.classes().declare(name.toStdString(), _program, "QOpenGLShaderProgram");

#define RZQLUA_DECLARE_GLSP_FUNCTION(F) program.declare_function(#F, &RZQLua_QOpenGLShaderProgram::F);
#define RZQLUA_DECLARE_GLSP_FUNCTION_OVERLOAD(F, S) program.declare_function(#F, (S)&RZQLua_QOpenGLShaderProgram::F);
#define RZQLUA_DECLARE_GLSP_FUNCTION_OVERLOAD_T(F, T, S) program.declare_function(#T, (S)&RZQLua_QOpenGLShaderProgram::F);

//    program.declare_function("bind", &QOpenGLShaderProgram::bind);
//    program.declare_function("release", &QOpenGLShaderProgram::release);
//    program.declare_function("enableAttributeArray", (void (QOpenGLShaderProgram::*)(int))&QOpenGLShaderProgram::enableAttributeArray);
//    program.declare_function("disableAttributeArray", (void (QOpenGLShaderProgram::*)(int))&QOpenGLShaderProgram::disableAttributeArray);

//    program.declare_function("addShaderFromSourceCode", (bool (QOpenGLShaderProgram::*)(QOpenGLShader::ShaderType, char const *))&QOpenGLShaderProgram::addShaderFromSourceCode);
//    program.declare_function("bindAttributeLocation", (void (QOpenGLShaderProgram::*)(char const *, int))&QOpenGLShaderProgram::bindAttributeLocation);
//    program.declare_function("link", &QOpenGLShaderProgram::link);

//    program.declare_function("setUniformValueF", (void (QOpenGLShaderProgram::*)(char const *, float))&QOpenGLShaderProgram::setUniformValue);
//    program.declare_function("setAttributeArrayF", &RZQLua_QOpenGLShaderProgram::setAttributeArrayF);
}

void RZQLua::declare_gl(QString const &name)
{
#define RZQLUA_DECLARE_GL_FUNCTION(F) gl.declare_function(#F, &QOpenGLFunctions::F);

    auto &gl = m_lua.classes().declare(name.toStdString(), m_gl);

    RZQLUA_DECLARE_GL_FUNCTION(glClearColor);
    RZQLUA_DECLARE_GL_FUNCTION(glClear);
    RZQLUA_DECLARE_GL_FUNCTION(glEnable);
    RZQLUA_DECLARE_GL_FUNCTION(glBlendFunc);
    RZQLUA_DECLARE_GL_FUNCTION(glDrawArrays);
    RZQLUA_DECLARE_GL_FUNCTION(glDisable);
    RZQLUA_DECLARE_GL_FUNCTION(glViewport);
}

void debug(QString message)
{
    qDebug() << message;
}

void RZQLua::declare_debug(QString const &)
{
    m_lua.functions().declare("debug", &debug);
}

void RZQLua::call(QString const &function)
{
    auto name = function.toStdString();
    auto L = m_lua.state();

    lua_getglobal(L, name.c_str());
    assert(lua_isfunction(L, -1));

    lua_pcall(L, 0, LUA_MULTRET, 0);
}

QVariantList RZQLua::call(QString const &function, QVariantList const &parms)
{
    auto name = function.toStdString();
    auto L = m_lua.state();

    lua_getglobal(L, name.c_str());
    assert(lua_isfunction(L, -1));

    if (parms.size() != 0)
    {
        for(auto const &parm : parms)
        {
            auto type = parm.type();
            //auto name = parm.typeName();
            switch (type)
            {
            case QVariant::Type::String:
                lua_pushstring(L, parm.toString().toStdString().c_str());
                break;
            case QVariant::Type::Int:
                lua_pushinteger(L, parm.toInt());
                break;
            case QVariant::Type::Bool:
                lua_pushboolean(L, parm.toInt());
                break;
            case QVariant::Type::Double:
                lua_pushnumber(L, parm.toDouble());
                break;
            default:
                assert(false);
                break;
            }
        }
    }

    auto top = lua_gettop(L) - parms.count();
    lua_pcall(L, parms.size(), LUA_MULTRET, 0);
    auto num_rv = lua_gettop(L) - top + 1;

    QVariantList rv;

    for (int i = num_rv - 1; i >= 0; --i)
    {
        int idx = -1 - i;
        auto type = lua_type(L, idx);

        switch (type)
        {
        case LUA_TNUMBER:
            rv.append(lua_tonumber(L, idx));
            break;
        case LUA_TBOOLEAN:
            rv.append(lua_toboolean(L, idx));
            break;
        case LUA_TSTRING:
            rv.append(lua_tostring(L, idx));
            break;
        default:
            qDebug() << "unhandled return type: " << lua_typename(L, idx);
            assert(false);
            break;
        }
    }

    lua_pop(L, num_rv);

    // todo crz: pop parms?
    return rv;
}

// todo crz: this, properly
void RZQLua::doFile(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QFile::ReadOnly))
    {
        return;
    }

    QTextStream ts(&f);
    auto lua = ts.readAll();

    auto rv = luaL_dostring(m_lua.state(), lua.toStdString().c_str());
    if (rv)
    {
        qDebug() << "lua_dostring error: " << rv;
    }
}

