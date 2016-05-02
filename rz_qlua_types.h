#pragma once

#include <ostream>
#include <string>

#include <QtWebSockets/QWebSocket>

class QUrl;
class QNetworkRequest;
class QByteArray;

std::ostream &operator<<(std::ostream &str, QWebSocketProtocol::CloseCode const &protocol);
std::ostream &operator<<(std::ostream &str, QUrl const &url);
std::ostream &operator<<(std::ostream &str, QByteArray const &);
std::ostream &operator<<(std::ostream &str, QNetworkRequest const &);

std::string operator+(std::string const &s, float f);
