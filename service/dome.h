#ifndef DOME_H
#define DOME_H

#include <QtCore/QObject>
#include <QVariantMap>
#include <vector>
#include <memory>

#include <config/ipcamera.h>

#include "sensors.h"
#include "ipcamera.h"

class Dome: public QObject
{
    Q_OBJECT
public:
    Dome(const std::vector<dome::config::IpCamera::Raw> &ipCameraConfig);
    virtual ~Dome();

public slots:
    Q_SCRIPTABLE double cpuTemperature();

    Q_SCRIPTABLE double roomTemperature();
    Q_SCRIPTABLE double roomHumidity();

    Q_SCRIPTABLE QString ipCameraPhoto(const QString &name);

private:
    Sensors m_sensors;
    std::map<std::string, std::unique_ptr<IpCamera>> m_ipCameras;
};

#endif
