#include "dome.h"

Dome::~Dome()
{

}

double Dome::cpuTemperature()
{
    double cpuTemp = m_sensors.cpuTemperature();

    return cpuTemp;
}
