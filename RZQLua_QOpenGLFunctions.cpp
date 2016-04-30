#include "RZQLua_QOpenGLFunctions.h"

#include "RZQLuaExports.h"
#include "RZQLua.hpp"

#include "rz.h"

RZQLua_QOpenGLFunctions::RZQLua_QOpenGLFunctions()
{
    LOG_CTOR("RZQLua_QOpenGLFunctions");

    initializeOpenGLFunctions();
}

static int ctor(lua_State *L)
{
    auto ex = (RZQLuaExports *)lua_touserdata(L, lua_upvalueindex(1));

    LOG_DEBUG("RZQLua_QOpenGLFunctions()");

    auto p = new RZQLua_QOpenGLFunctions();

    /*auto instance =*/ ex->bind_instance(p);

    return true;
}

template <>
int (*RZQLuaExports::get_ctor<RZQLua_QOpenGLFunctions>())(lua_State*)
{
    LOG_DEBUG("RZQLua_QOpenGL get_ctor()");

    return ctor;
}

template<>
void RZQLuaExports::declare_instance_functions(RZLuaInstance<RZQLua_QOpenGLFunctions> &instance)
{
    LOG_DEBUG("RZQLua_QOpenGL declare_instance_functions() nothing to do");

#define DEF(F) instance.declare_function(#F, &QOpenGLFunctions::F);

    DEF(glClearColor);
    DEF(glClear);
    DEF(glEnable);
    DEF(glBlendFunc);
    DEF(glDrawArrays);
    DEF(glDisable);
    DEF(glViewport);

#undef DEF
}

template<>
void RZQLuaExports::declare_class_symbols(RZLuaClass<RZQLua_QOpenGLFunctions> &/*clazz*/)
{
}