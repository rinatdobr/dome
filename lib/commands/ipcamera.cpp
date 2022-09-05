#include "ipcamera.h"

#include <QtDBus/QtDBus>
#include <sstream>
#include <spdlog/spdlog.h>

#include "dbusservice.h"

#include <config/db.h>
#include <utils.h>

namespace command {

IpCamera::IpCamera(const std::vector<std::string> &args)
    : Command(IpCameraCommandId, args)
{
    spdlog::trace("{}:{} {} args.size()={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, args.size());
}

std::string IpCamera::name() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return IpCameraName;
}

Result IpCamera::execute()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (m_args.size() < 2) {
        spdlog::error("command::IpCamera: Not enought args");
        return {};
    }

    QDBusInterface iface(
        QString::fromStdString(dome::dbus::Service),
        QString::fromStdString(dome::dbus::Path),
        QString::fromStdString(dome::dbus::Interface),
        QDBusConnection::systemBus()
    );

    if (iface.isValid()) {
        std::string methodName;
        Result::FileType fileType;
        if (m_args[0] == "photo") {
            methodName = dome::dbus::MethodIpCameraPhoto;
            fileType = Result::FileType::Photo;
        }
        else if (m_args[0] == "image") {
            methodName = dome::dbus::MethodIpCameraPhoto;
            fileType = Result::FileType::Document;
        }

        QDBusReply<QString> reply = iface.call(QString::fromStdString(methodName), QVariant(QString::fromStdString(m_args[1])));
        if (reply.isValid()) {
            spdlog::info("Executing {}... Done", name());
            Result result(this, reply.value().toStdString(), fileType);
            return result;
        }

        spdlog::error("command::IpCamera: Method call failed: {}:{}", reply.error().name().toStdString(), reply.error().message().toStdString());
        return {};
    }

    spdlog::error("command::IpCamera: Invalid command");
    return {};
}

}
