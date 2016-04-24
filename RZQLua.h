#pragma once

#include "RZLua.h"
#include "RZQLuaExports.h"

class RZQLua
{
    RZLua m_lua;
    RZQLuaExports m_exports;

public:
    RZQLua();
    RZQLua(std::string const &file);
    ~RZQLua() {}

    RZLua &lua() { return m_lua; }

    template <typename... TArgs>
    QVariantList vcall(std::string const &function_name, TArgs...);

    QVariantList call(std::string const &function_name, QVariantList const &args);
    void call(std::string const &function_name);

    void doFile(std::string const &filename);

    int new_object();
};

#include "rz_qlua_meta.h"

template <typename... TArgs>
QVariantList RZQLua::vcall(std::string const &function_name, TArgs... args)
{
    QVariantList list;
    rz::detail::build_args_list(list, args...);
    return call(function_name, list);
}



