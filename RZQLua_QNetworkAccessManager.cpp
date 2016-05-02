#include "RZQLuaExports.h"
#include "RZQLua.hpp"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QByteArray>

static int ctor(lua_State *L)
{
    auto ex = (RZQLuaExports *)lua_touserdata(L, lua_upvalueindex(1));

    LOG_DEBUG("QNetworkAccessManager.new()");

    auto p = new QNetworkAccessManager();

    auto instance = ::bind_instance(*ex, p);

    QObject::connect(p, &QNetworkAccessManager::finished,
                     [L, instance, ex](QNetworkReply *reply)
    {
        LOG_SIGNAL("QNetworkAccessManager.finished()");

        if (instance->push_function("finished"))
        {
            ::bind_instance(*ex, reply);

            lua_pcall(L, 1, 0, 0);
        }
    });

    return true;
}

template <>
int (*get_ctor<QNetworkAccessManager>())(lua_State*)
{
    return &ctor;
}

template<>
void declare_instance_functions(RZLuaInstance<QNetworkAccessManager> &instance)
{
    instance.declare_function("get", &QNetworkAccessManager::get);
}

template<>
void RZQLuaExports::declare_class_symbols(RZLuaClass<QNetworkAccessManager> &/*clazz*/)
{
}
