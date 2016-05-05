#pragma once

#include "rz_lua.h"

#include "RZLuaInstances.h"

#include <sstream>
#include <type_traits>

template <typename TClass>
struct class_traits
{
    using has_ctor = std::true_type;
    using has_instance_methods = std::true_type;
    using has_class_symbols = std::false_type;
};

#include "RZLuaExports.h"

template <>
struct class_traits<RZLuaExports>
{
    using has_ctor = std::true_type;
    using has_instance_methods = std::true_type;
    using has_class_symbols = std::false_type;
};

template <typename> class RZLuaInstance;

template <typename TClass>
char const *ClassName();

template <typename TClass>
int (*get_ctor())(lua_State*);

template <typename TClass>
void declare_instance_functions(RZLuaInstance<TClass> &instance);

#include "RZLuaExports_Qt.h"

namespace detail
{
template <typename TClass>
void declare_instance_functions(RZLuaInstance<TClass> &instance, std::true_type)
{
    LOG_VERBOSE(ClassName<TClass>() << ": declaring instance functions");

    ::declare_instance_functions(instance);
}

template <typename TClass>
void declare_instance_functions(RZLuaInstance<TClass> &instance, std::false_type)
{
    (void)instance;

    LOG_VERBOSE(ClassName<TClass>() << ": no instance functions");
}
}

template <typename TClass>
RZLuaInstance<TClass> *bind_instance(RZLuaExports &ex, TClass *instance);

template <typename TClass>
RZLuaInstance<TClass> *bind_instance(RZLuaExports &ex, TClass *instance)
{
    auto &instance_meta = ex.instances().bind_instance(instance);

    detail::declare_instance_functions(instance_meta, typename class_traits<TClass>::has_instance_methods());

    return &instance_meta;
}

