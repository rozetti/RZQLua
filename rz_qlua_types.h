#pragma once

#include <ostream>
#include <string>

#include <QtWebSockets/QWebSocket>

class QUrl;

std::ostream &operator<<(std::ostream &str, QWebSocketProtocol::CloseCode const &protocol);
std::ostream &operator<<(std::ostream &str, QUrl const &url);

std::string operator+(std::string const &s, float f);
