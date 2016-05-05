#include "RZLuaExports.h"
#include "RZQLua.hpp"

#include "rz_lua_exports.h"

#include <QtWebSockets/QWebSocket>
#include <string>

static int ctor(lua_State *L)
{
    LOG_DEBUG("QWebSocket.new()");

    auto ex = (RZLuaExports *)lua_touserdata(L, lua_upvalueindex(1));

    auto p = new QWebSocket();

    auto instance = ::bind_instance(*ex, p);

    QObject::connect(p, &QWebSocket::connected, [L, instance]()
    {
        LOG_SIGNAL("QWebSocket.connected()");

        if (instance->push_function("connected"))
        {
            lua_pcall(L, 0, 0, 0);
        }
    });

    QObject::connect(p, &QWebSocket::disconnected, [L, instance]()
    {
        LOG_SIGNAL("QWebSocket.disconnected()");

        if (instance->push_function("disconnected"))
        {
            lua_pcall(L, 0, 0, 0);
        }
    });

    QObject::connect(p, &QWebSocket::textMessageReceived,
                     [L, instance](QString const &message)
    {
        LOG_DEBUG("QWebSocket.textMessageReceived(" << message.toStdString() << ")");

        if (instance->push_function("textMessageReceived"))
        {
            lua_pushstring(L, message.toStdString().c_str());
            lua_pcall(L, 1, 0, 0);
        }
    });

    return 1;
}

template <>
int (*get_ctor<QWebSocket>())(lua_State*)
{
    return &ctor;
}

template<>
void declare_instance_functions(RZLuaInstance<QWebSocket> &instance)
{
    instance.declare_function("open", (void (QWebSocket::*)(QUrl const &))&QWebSocket::open);
    instance.declare_function("sendTextMessage", &QWebSocket::sendTextMessage);
    instance.declare_function("close", &QWebSocket::close);
}

template<>
void RZLuaExports::declare_class_symbols(RZLuaClass<QWebSocket> &clazz)
{
#define DEF(S) clazz.declare_constant(std::string(#S), (int)QWebSocketProtocol::S)

    DEF(CloseCodeNormal);
    DEF(CloseCodeGoingAway);
    DEF(CloseCodeProtocolError);
    DEF(CloseCodeDatatypeNotSupported);
    DEF(CloseCodeReserved1004);
    DEF(CloseCodeMissingStatusCode);
    DEF(CloseCodeAbnormalDisconnection);
    DEF(CloseCodeWrongDatatype);
    DEF(CloseCodePolicyViolated);
    DEF(CloseCodeTooMuchData);
    DEF(CloseCodeMissingExtension);
    DEF(CloseCodeBadOperation);
    DEF(CloseCodeTlsHandshakeFailed);

    DEF(VersionUnknown);
    DEF(Version0);
    DEF(Version4);
    DEF(Version5);
    DEF(Version6);
    DEF(Version7);
    DEF(Version8);
    DEF(Version13);
    DEF(VersionLatest);

#undef DEF
}
