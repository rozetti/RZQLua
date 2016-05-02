#include <QCoreApplication>

#include "Http.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Http http;
    QObject::connect(&http, &Http::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
