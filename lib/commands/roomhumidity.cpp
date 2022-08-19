#include "roomhumidity.h"

#include <QtDBus/QtDBus>
#include <sstream>
#include <spdlog/spdlog.h>

#include "dbusservice.h"

namespace command {

std::string ProcessRoomHumidityReply(const double &reply)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::ostringstream stream;
    stream << reply;
    return stream.str();
}

RoomHumidity::RoomHumidity(const std::vector<std::string> &args)
    : Command(RoomHumidityCommandId, args)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
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
        QString::fromStdString(dbus_dome::Service),
        QString::fromStdString(dbus_dome::Path),
        QString::fromStdString(dbus_dome::Interface),
        QDBusConnection::systemBus()
    );

    if (iface.isValid()) {
        QDBusReply<double> reply = iface.call(QString::fromStdString(dbus_dome::MethodRoomHumidity));
        if (reply.isValid()) {
            spdlog::info("Executing {}... Done", name());
            Result result(this, ProcessRoomHumidityReply(reply.value()));
            return result;
        }

        spdlog::error("command::RoomHumidity: Method call failed: {}:{}", reply.error().name().toStdString(), reply.error().message().toStdString());
        return {};
    }

    spdlog::error("command::RoomHumidity: Iface not valid: {}:{}", iface.lastError().name().toStdString(), iface.lastError().message().toStdString());
    return {};
}

}
