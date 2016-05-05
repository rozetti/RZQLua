#include "RZLuaExports.h"
#include "RZQLua.hpp"

#include <QtNetwork/QNetworkRequest>
#include <QUrl>

#include "rz_lua_meta.h"
#include "rz_qlua_meta.h"
#include "rz_lua_exports.h"

static int ctor(lua_State *L)
{
    auto ex = (RZLuaExports *)lua_touserdata(L, lua_upvalueindex(1));

    auto url = rz::detail::get_at_index<QUrl const &>(L, -1);

    LOG_DEBUG("QNetworkRequest.new(" << url.toString() << ")");

    auto p = new QNetworkRequest(url);

    /*auto instance =*/ ::bind_instance(*ex, p);

    return true;
}

template <>
int (*get_ctor<QNetworkRequest>())(lua_State*)
{
    return &ctor;
}

template<>
void declare_instance_functions(RZLuaInstance<QNetworkRequest> &/*instance*/)
{
    //instance.declare_function("scheme", &QUrl::scheme);
}

template<>
void RZLuaExports::declare_class_symbols(RZLuaClass<QNetworkRequest> &/*clazz*/)
{
}
