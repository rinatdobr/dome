#include "dome.h"

Dome::~Dome()
{

}

QVariantMap Dome::systemSensors()
{
    double cpuTemp = m_sensors.cpuTemparture();

    return QVariantMap{{"CpuTemerature", cpuTemp}};
}
