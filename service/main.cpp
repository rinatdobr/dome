#include <stdio.h>
#include <stdlib.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtDBus/QtDBus>

#include <spdlog/spdlog.h>

#include "dome.h"
#include "dbusservice.h"
#include "config.h"

int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Start domed");

    QCoreApplication app(argc, argv);

    if (!QDBusConnection::systemBus().isConnected()) {
        spdlog::error("Cannot connect to the D-Bus system bus");
        return EXIT_FAILURE;
    }

    if (!QDBusConnection::systemBus().registerService(QString::fromStdString(dome::dbus::Service))) {
        spdlog::error("Can't register service: {}", qPrintable(QDBusConnection::systemBus().lastError().message()));
        return EXIT_FAILURE;
    }

    Config config;
    Dome dome(config.ipCameraConfig());
    QDBusConnection::systemBus().registerObject(
        QString::fromStdString(dome::dbus::Path),
        QString::fromStdString(dome::dbus::Interface),
        &dome,
        QDBusConnection::ExportAllSlots
    );

    app.exec();
    return 0;
}
