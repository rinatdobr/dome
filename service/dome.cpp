#include "dome.h"

#include <spdlog/spdlog.h>

Dome::Dome(const std::vector<dome::config::IpCamera::Raw> &ipCameraConfig)
    : m_ipCameras(IpCamera::Create(ipCameraConfig))
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Dome::~Dome()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

double Dome::cpuTemperature()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    spdlog::info("Got cpu temperature request");

    double cpuTemp = m_sensors.cpuTemperature();
    spdlog::debug("{}:{}", __PRETTY_FUNCTION__, cpuTemp);

    return cpuTemp;
}

double Dome::roomTemperature()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    spdlog::info("Got room temperature request");

    double roomTemp = m_sensors.roomTemperature();
    spdlog::debug("{}:{}", __PRETTY_FUNCTION__, roomTemp);

    return roomTemp;
}

double Dome::roomHumidity()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    spdlog::info("Got room humidity request");

    double roomHumidity = m_sensors.roomHumidity();
    spdlog::debug("{}:{}", __PRETTY_FUNCTION__, roomHumidity);

    return roomHumidity;
}

QString Dome::ipCameraPhoto(const QString &name)
{
    spdlog::trace("{}:{} {} name={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name.toStdString());
    spdlog::info("Got ip camera photo request");

    if (m_ipCameras.count(name.toStdString())) {
        QString photoPath = QString::fromStdString(m_ipCameras[name.toStdString()]->photo());
        spdlog::debug("{}:{}", __PRETTY_FUNCTION__, photoPath.toStdString());
        return photoPath;
    }

    return {};
}
