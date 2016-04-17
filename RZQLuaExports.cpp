#include "RZQLuaExports.h"

#include "RZQLua.h"

RZQLuaExports::RZQLuaExports(RZQLua &qlua) :
    m_qlua(qlua)
{
}

bool RZQLuaExports::import(QString lib, QString name)
{
    if ("gles20" == lib)
    {
        m_qlua.declare_gl(name);
        return true;
    }

    if ("debug" == lib)
    {
        m_qlua.declare_debug(name);
        return true;
    }

    return false;
}

