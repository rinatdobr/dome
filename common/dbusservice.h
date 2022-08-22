#ifndef DBUS_SERVICE_H
#define DBUS_SERVICE_H

#include <string>

namespace dome {
namespace dbus {
    const std::string Path("/");
    const std::string Service("ru.dome");
    const std::string Interface("ru.dome");
    const std::string MethodCpuTemperature("cpuTemperature");
    const std::string MethodRoomTemperature("roomTemperature");
    const std::string MethodRoomHumidity("roomHumidity");
    const std::string MethodIpCameraPhoto("ipCameraPhoto");
}
}

#endif
