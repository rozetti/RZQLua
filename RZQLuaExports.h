#pragma once

#include <QString>

class RZQLua;

class RZQLuaExports
{
    RZQLua &m_qlua;

public:
    RZQLuaExports(RZQLua &qlua);

    bool import(QString lib, QString name);
};
