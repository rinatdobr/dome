#ifndef SENSORS_H
#define SENSORS_H

#include <QtCore/QObject>
#include <QVariantMap>

class Sensors
{
public:
    double cpuTemperature() const;
    double roomTemperature() const;
};

#endif
