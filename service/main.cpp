#include <stdio.h>
#include <stdlib.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtDBus/QtDBus>

#include "dome.h"

#define SERVICE_NAME    "ru.dome"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (!QDBusConnection::systemBus().isConnected()) {
        fprintf(stderr, "Cannot connect to the D-Bus system bus.\n"
                "To start it, run:\n"
                "\teval `dbus-launch --auto-syntax`\n");
        return 1;
    }

    if (!QDBusConnection::systemBus().registerService(SERVICE_NAME)) {
        fprintf(stderr, "%s\n",
                qPrintable(QDBusConnection::systemBus().lastError().message()));
        exit(1);
    }

    Dome dome;
    QDBusConnection::systemBus().registerObject("/", &dome, QDBusConnection::ExportAllSlots);

    app.exec();
    return 0;
}
