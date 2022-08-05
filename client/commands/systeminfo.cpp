#include "systeminfo.h"

#include <QtDBus/QtDBus>
#include <sstream>
#include <iostream>

#include "dbusservice.h"

namespace command {

std::string ProcessSystemSensorsReply(const QVariantMap &reply)
{
    std::ostringstream stream;
    auto i = reply.constBegin();
    while (i != reply.constEnd()) {
        stream << i.key().toStdString() << ": " << i.value().toString().toStdString() << '\n';
        ++i;
    }
    return stream.str();
}

SystemInfo::SystemInfo(const std::vector<std::string> &args)
    : Command(SystemInfoCommandId, args)
{

}

Result SystemInfo::execute()
{
    std::cout << "execute()" << std::endl;

    QDBusInterface iface(
        QString::fromStdString(dbus_dome::Service),
        QString::fromStdString(dbus_dome::Path),
        QString::fromStdString(dbus_dome::Interface),
        QDBusConnection::systemBus()
    );

    if (iface.isValid()) {
        QDBusReply<QVariantMap> reply = iface.call(QString::fromStdString(dbus_dome::MethodSystemSensors));
        if (reply.isValid()) {
            Result result(ProcessSystemSensorsReply(reply.value()));
            std::cout << "Reply was: " << result.toString() << std::endl;
            return result;
        }

        std::cerr << "Method call failed: " << reply.error().name().toStdString() << ":" << reply.error().message().toStdString() << std::endl;
        return {};
    }

    std::cerr << "Iface not valid: " << iface.lastError().name().toStdString() << ":" << iface.lastError().message().toStdString() << std::endl;
    return {};
}

}
