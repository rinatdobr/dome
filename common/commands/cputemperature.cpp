#include "cputemperature.h"

#include <QtDBus/QtDBus>
#include <sstream>
#include <iostream>

#include "dbusservice.h"

namespace command {

std::string ProcessCpuTemperatureReply(const double &reply)
{
    std::ostringstream stream;
    stream << reply;
    return stream.str();
}

CpuTemperature::CpuTemperature(const std::vector<std::string> &args)
    : Command(CpuTemperatureCommandId, args)
{

}

std::string CpuTemperature::name() const
{
    return CpuTemperatureName;
}

Result CpuTemperature::execute()
{
    std::cout << "execute()" << std::endl;

    QDBusInterface iface(
        QString::fromStdString(dbus_dome::Service),
        QString::fromStdString(dbus_dome::Path),
        QString::fromStdString(dbus_dome::Interface),
        QDBusConnection::systemBus()
    );

    if (iface.isValid()) {
        QDBusReply<double> reply = iface.call(QString::fromStdString(dbus_dome::MethodCpuTemperature));
        if (reply.isValid()) {
            Result result(this, ProcessCpuTemperatureReply(reply.value()));
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
