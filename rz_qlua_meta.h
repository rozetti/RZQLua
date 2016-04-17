#pragma once

#include <QVariantList>
#include <QString>

namespace rz
{

namespace detail
{

template <typename TArg, typename... TArgs>
static void build_args_list(QVariantList &list, TArg arg, TArgs... args)
{
    list << arg;

    build_args_list(list, args...);
}

static void build_args_list(QVariantList &)
{
}

template<>
static QString get_at_index(lua_State *l, int i)
{
    return QString(lua_tostring(l, i));
}

}
}
