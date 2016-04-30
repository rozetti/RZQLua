#include "RZQLuaExports.h"
#include "RZQLua.hpp"

#include <QtWebSockets/QWebSocket>

static int ctor(lua_State *L)
{
    auto ex = (RZQLuaExports *)lua_touserdata(L, lua_upvalueindex(1));

    auto url = lua_tostring(L, -1);

    LOG_DEBUG("QUrl.new(" << url << ")");

    auto p = new QUrl(url);

    /*auto instance =*/ ex->bind_instance(p);

    return true;
}

template <>
int (*RZQLuaExports::get_ctor<QUrl>())(lua_State*)
{
    return &ctor;
}

template<>
void RZQLuaExports::declare_instance_functions(RZLuaInstance<QUrl> &instance)
{
    instance.declare_function("scheme", &QUrl::scheme);
}

template<>
void RZQLuaExports::declare_class_symbols(RZLuaClass<QUrl> &/*clazz*/)
{
}
