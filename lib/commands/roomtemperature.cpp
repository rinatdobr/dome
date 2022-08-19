#include "roomtemperature.h"

#include <QtDBus/QtDBus>
#include <sstream>
#include <spdlog/spdlog.h>

#include "dbusservice.h"

namespace command {

std::string ProcessRoomTemperatureReply(const double &reply)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::ostringstream stream;
    stream << reply;
    return stream.str();
}

RoomTemperature::RoomTemperature(const std::vector<std::string> &args)
    : Command(RoomTemperatureCommandId, args)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

std::string RoomTemperature::name() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return RoomTemperatureName;
}

Result RoomTemperature::execute()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    QDBusInterface iface(
        QString::fromStdString(dbus_dome::Service),
        QString::fromStdString(dbus_dome::Path),
        QString::fromStdString(dbus_dome::Interface),
        QDBusConnection::systemBus()
    );

    if (iface.isValid()) {
        QDBusReply<double> reply = iface.call(QString::fromStdString(dbus_dome::MethodRoomTemperature));
        if (reply.isValid()) {
            spdlog::info("Executing {}... Done", name());
            Result result(this, ProcessRoomTemperatureReply(reply.value()));
            return result;
        }

        spdlog::error("command::RoomTemperature: Method call failed: {}:{}", reply.error().name().toStdString(), reply.error().message().toStdString());
        return {};
    }

    spdlog::error("command::RoomTemperature: Iface not valid: {}:{}", iface.lastError().name().toStdString(), iface.lastError().message().toStdString());
    return {};
}

}
