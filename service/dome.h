#ifndef DOME_H
#define DOME_H

#include <QtCore/QObject>
#include <QVariantMap>
#include <vector>
#include <memory>

#include <config/ipcamera.h>

#include "sources/ipcamera.h"
#include "sources/sensors.h"
#include "sources/system.h"

class Dome: public QObject
{
    Q_OBJECT
public:
    Dome(const std::vector<dome::config::IpCamera::Config> &ipCameraConfig);
    virtual ~Dome();

public slots:
    // system
    Q_SCRIPTABLE double cpuTemperature();

    // sensors
    Q_SCRIPTABLE double roomTemperature();
    Q_SCRIPTABLE double roomHumidity();

    // ip camera
    Q_SCRIPTABLE QString ipCameraPhoto(const QString &name);

private:
    Sensors m_sensors;
    System m_system;
    std::map<std::string, std::unique_ptr<IpCamera>> m_ipCameras;
};

#endif
