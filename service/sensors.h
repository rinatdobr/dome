#ifndef SENSORS_H
#define SENSORS_H

#include <QtCore/QObject>
#include <QVariantMap>
#include <QSharedPointer>
#include <DHTXXD.h>

class Sensors
{
public:
    Sensors();

    double cpuTemperature() const;
    double roomTemperature() const;

private:
    int m_pi;
    QSharedPointer<DHTXXD_t> m_dht;
};

#endif
