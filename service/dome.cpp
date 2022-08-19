#include "dome.h"

Dome::~Dome()
{

}

double Dome::cpuTemperature()
{
    double cpuTemp = m_sensors.cpuTemperature();

    return cpuTemp;
}

double Dome::roomTemperature()
{
    double roomTemp = m_sensors.roomTemperature();

    return roomTemp;
}

double Dome::roomHumidity()
{
    double roomHumidity = m_sensors.roomHumidity();

    return roomHumidity;
}
