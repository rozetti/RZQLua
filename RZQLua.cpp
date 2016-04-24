#include "RZQLua.h"

#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QVariantList>
#include <QString>
#include <QDebug>

#include <assert.h>
#include <sstream>

#include "rz_qlua_meta.h"

static int instance_number = 1;

int RZQLua::new_object()
{
    auto class_name = lua_tostring(m_lua.state(), -1);

    std::stringstream ss;
    ss << "__mt:" << instance_number << ":" << class_name;
    auto instance_name = ss.str();

    if (!m_exports.export_new_instance(class_name, instance_name))
    {
        return 0;
    }

    ++instance_number;

    return 1;
}

int _new_object(lua_State *L)
{
    auto _This = (RZQLua *)lua_touserdata(L, lua_upvalueindex(1));
    return _This->new_object();
}

RZQLua::RZQLua() :
    m_exports(*this)
{
    auto &rz = m_lua.instances().bind_instance("rz", &m_exports);
    rz.declare_function("import", &RZQLuaExports::export_lib);

    lua_pushlightuserdata(m_lua.state(), this);
    lua_pushcclosure(m_lua.state(), _new_object, 1);
    lua_setglobal(m_lua.state(), "new");
}

RZQLua::RZQLua(std::string const &file) :
    RZQLua()
{
    doFile(file);
}

void RZQLua::call(std::string const &function_name)
{
    auto L = m_lua.state();

    lua_getglobal(L, function_name.c_str());
    if (!lua_isfunction(L, -1))
    {
        assert(false);
        return;
    }

    lua_pcall(L, 0, 0, 0);
}

QVariantList RZQLua::call(std::string const &function_name, QVariantList const &args)
{
    auto L = m_lua.state();

    lua_getglobal(L, function_name.c_str());
    assert(lua_isfunction(L, -1));

    if (args.size() != 0)
    {
        for(auto const &arg : args)
        {
            auto type = arg.type();
            switch (type)
            {
            case QVariant::Type::String:
                lua_pushstring(L, arg.toString().toStdString().c_str());
                break;
            case QVariant::Type::Int:
                lua_pushinteger(L, arg.toInt());
                break;
            case QVariant::Type::Bool:
                lua_pushboolean(L, arg.toInt());
                break;
            case QVariant::Type::Double:
                lua_pushnumber(L, arg.toDouble());
                break;
            default:
                qDebug() << "unhandled argument type: " << arg.typeName();
                assert(false);
                break;
            }
        }
    }

    auto top = lua_gettop(L) - args.count();
    lua_pcall(L, args.size(), LUA_MULTRET, 0);
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

    return rv;
}

void RZQLua::doFile(std::string const &filename)
{
    QFile f(QString(filename.c_str()));
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

