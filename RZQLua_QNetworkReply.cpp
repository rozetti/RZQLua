#include "RZQLuaExports.h"
#include "RZQLua.hpp"

#include <QtNetwork/QNetworkReply>

//static int ctor(lua_State *L)
//{
//    auto ex = (RZQLuaExports *)lua_touserdata(L, lua_upvalueindex(1));

//    auto url = lua_tostring(L, -1);

//    LOG_DEBUG("QNetworkRequest.new(" << url << ")");

//    auto p = new QNetworkReply(url);

//    /*auto instance =*/ ex->bind_instance(p);

//    return true;
//}

template <>
int (*get_ctor<QNetworkReply>())(lua_State*)
{
    return nullptr;
}

template<>
void declare_instance_functions(RZLuaInstance<QNetworkReply> &instance)
{
    instance.declare_function("readAll", &QNetworkReply::readAll);
}

template<>
void RZQLuaExports::declare_class_symbols(RZLuaClass<QNetworkReply> &/*clazz*/)
{
}
