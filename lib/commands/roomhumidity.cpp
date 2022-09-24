#include "roomhumidity.h"

#include <QtDBus/QtDBus>
#include <sstream>
#include <spdlog/spdlog.h>

#include "dbusservice.h"

namespace command {

std::string ProcessRoomHumidityReply(const double &reply)
{
    spdlog::trace("{}:{} {} reply={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, reply);

    std::ostringstream stream;
    stream << reply;
    return stream.str();
}

RoomHumidity::RoomHumidity(const std::vector<std::string> &args)
    : Command(RoomHumidityCommandId, args)
{
    spdlog::trace("{}:{} {} args.size()={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, args.size());
}

std::string RoomHumidity::name() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return RoomHumidityName;
}

Result RoomHumidity::execute()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    QDBusInterface iface(
        QString::fromStdString(dome::dbus::Service),
        QString::fromStdString(dome::dbus::Path),
        QString::fromStdString(dome::dbus::Interface),
        QDBusConnection::systemBus()
    );

    if (iface.isValid()) {
        QDBusReply<double> reply = iface.call(QString::fromStdString(dome::dbus::MethodRoomHumidity));
        if (reply.isValid()) {
            spdlog::info("Executing {}... Done", name());
            return Result(this, ProcessRoomHumidityReply(reply.value()), Result::Status::Success);
        }

        spdlog::error("command::RoomHumidity: D-Bus method call failed: {}:{}", reply.error().name().toStdString(), reply.error().message().toStdString());
        return Result(this, "D-Bus method call failed", Result::Status::Fail);
    }

    spdlog::error("command::RoomHumidity: D-Bus interface is not valid: {}:{}", iface.lastError().name().toStdString(), iface.lastError().message().toStdString());
    return Result(this, "D-Bus interface is not valid", Result::Status::Fail);
}

}
