#include "RZQLua.hpp"

#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QVariantList>
#include <QString>

RZQLua::RZQLua(std::string const &file) :
    m_lua(*this),
    m_exports(*this)
{
    auto &rz = m_lua.instances().bind_instance("rz", &m_exports);
    rz.declare_function("import", &RZQLuaExports::export_lib);
    rz.declare_function("importWithName", &RZQLuaExports::export_lib_with_name);

    m_lua.doFile(":/debug.lua");
    m_lua.doFile(file);
}

QVariantList RZQLua::call(std::string const &function_name, QVariantList const &args)
{
    auto const &L = m_lua;

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
                LOG_WARNING("unhandled argument type: " << arg.typeName());
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
            LOG_WARNING("unhandled return type: " << lua_typename(L, idx));
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

    auto rv = luaL_dostring(m_lua, lua.toStdString().c_str());
    if (rv)
    {
        auto error = lua_tostring(m_lua, -1);
        LOG_ERROR("lua_dostring error: " << error);
    }
}

