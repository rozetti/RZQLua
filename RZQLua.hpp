#pragma once

#include "rz.h"
#include "RZLua.hpp"
#include "RZQLuaExports.h"

#include "rz_qlua_meta.h"

class RZQLua
{
    RZLua<RZQLua> m_lua;
    RZQLuaExports m_exports;

public:
    RZQLua(std::string const &file);
    ~RZQLua() {}

    RZLua<RZQLua> &lua() { return m_lua; }

    template <typename... TArgs>
    QVariantList vcall(std::string const &function_name, TArgs...);

    QVariantList call(std::string const &function_name, QVariantList const &args);

    void doFile(std::string const &filename);
};

template <typename... TArgs>
QVariantList RZQLua::vcall(std::string const &function_name, TArgs... args)
{
    QVariantList list;
    rz::detail::build_args_list(list, args...);
    return call(function_name, list);
}



