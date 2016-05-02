#include "rz_qlua_types.h"

#include <string>
#include <sstream>

#include <QtWebSockets/QWebSocket>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QByteArray>

std::ostream &operator<<(std::ostream &str, QWebSocketProtocol::CloseCode const &protocol)
{
    str << (int)protocol;

    return str;
}

std::ostream &operator<<(std::ostream &str, QNetworkRequest const &o)
{
    str << o.url().toString().toStdString();

    return str;
}

std::ostream &operator<<(std::ostream &str, QUrl const &url)
{
    str << url.toString().toStdString();

    return str;
}

std::ostream &operator<<(std::ostream &str, QByteArray const &o)
{
    str << "QByteArray(" << o.length() << ")";

    return str;
}

std::string operator+(std::string const &s, float f)
{
    std::stringstream ss;

    ss << s << f;

    return ss.str();
}

