#ifndef DOME_H
#define DOME_H

#include <QtCore/QObject>
#include <QVariantMap>

#include "sensors.h"

class Dome: public QObject
{
    Q_OBJECT
public:
    virtual ~Dome();

public slots:
    Q_SCRIPTABLE double cpuTemperature();
    Q_SCRIPTABLE double roomTemperature();
    Q_SCRIPTABLE double roomHumidity();

private:
    Sensors m_sensors;
};

#endif
