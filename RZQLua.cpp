#include "RZQLua.h"

#include <QtGui/QOpenGLFunctions>

#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QVariantList>
#include <QString>
#include <QDebug>

#include <assert.h>

#include "rz_qlua_meta.h"

RZQLua::RZQLua() :
    m_exports(*this)
{
#define RZQLUA_DECLARE_RZ_FUNCTION(F) rz.declare_function(#F, &RZQLuaExports::F);

    auto &rz = m_lua.classes().declare("rz", m_exports);
    rz.declare_function("import", &RZQLuaExports::import);
}

RZQLua::RZQLua(const QString &file) :
    RZQLua()
{
    doFile(file);
}

void RZQLua::declare_gl(QString const &name)
{
#define RZQLUA_DECLARE_GL_FUNCTION(F) gl.declare_function(#F, &QOpenGLFunctions::F);

    m_gl.initializeOpenGLFunctions();

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

QVariantList RZQLua::call(QString const &function, QVariantList const &parms)
{
    auto name = function.toStdString();
    auto L = m_lua.state();

    lua_getglobal(L, name.c_str());
    assert(lua_isfunction(L, -1));

    for(auto const &parm : parms)
    {
        switch (parm.type())
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
            lua_pushnumber(L, parm.toInt());
            break;
        default:
            assert(false);
            break;
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

