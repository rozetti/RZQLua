#include "rz_qlua_types.h"

#include <string>
#include <sstream>

#include <QtWebSockets/QWebSocket>
#include <QUrl>

std::ostream &operator<<(std::ostream &str, QWebSocketProtocol::CloseCode const &protocol)
{
    str << (int)protocol;

    return str;
}

std::ostream &operator<<(std::ostream &str, QUrl const &url)
{
    str << url.toString().toStdString();

    return str;
}

std::string operator+(std::string const &s, float f)
{
    std::stringstream ss;

    ss << s << f;

    return ss.str();
}

