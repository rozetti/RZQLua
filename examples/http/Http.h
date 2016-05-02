#ifndef HTTP_H
#define HTTP_H

#include <QObject>

#include "RZQLua.hpp"

class Http : public QObject
{
    Q_OBJECT

    RZQLua m_qlua;

public:
    Http();

signals:
    void closed();
};

#endif // HTTP_H
