#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H

#include <string>

namespace dbus_dome {
    const std::string Path("/");
    const std::string Service("ru.dome");
    const std::string Interface("ru.dome");
    const std::string MethodCpuTemperature("cpuTemperature");
    const std::string MethodRoomTemperature("roomTemperature");
    const std::string MethodRoomHumidity("roomHumidity");
}

#endif
