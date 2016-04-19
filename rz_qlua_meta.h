#pragma once

#include <QVariantList>
#include <QString>

namespace rz
{

namespace detail
{

template <typename TArg>
void build_args_list(QVariantList &list, TArg arg)
{
    list << arg;
}

template <typename TArg, typename... TArgs>
void build_args_list(QVariantList &list, TArg arg1, TArg arg2, TArgs... args)
{
    //list << arg;
    build_args_list(list, arg1);
    build_args_list(list, arg2, args...);
}

template<>
QString get_at_index(lua_State *l, int i)
{
    return QString(lua_tostring(l, i));
}

}
}
