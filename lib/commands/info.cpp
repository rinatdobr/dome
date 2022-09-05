#include "info.h"

#include <sstream>
#include <QtDBus/QtDBus>
#include <spdlog/spdlog.h>

#include "roomhumidity.h"
#include "roomtemperature.h"

namespace command {

Info::Info(const std::vector<std::string> &args)
    : Command(InfoCommandId, args)
{
    spdlog::trace("{}:{} {} args.size()={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, args.size());
}

std::string Info::name() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return InfoName;
}

Result Info::execute()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    std::ostringstream resultStr;

    command::RoomTemperature tempCommand({});
    Result resTemp = tempCommand.execute();
    if (resTemp.isValid()) {
        resultStr << "Температура: " << resTemp.toString();
    }
    else {
        resultStr << "Температура: " << "-";
    }

    command::RoomHumidity humidityCommand({});
    Result resHumidity = humidityCommand.execute();
    if (resHumidity.isValid()) {
        resultStr << "\nВлажность: " << resHumidity.toString();
    }
    else {
        resultStr << "\nВлажность: " << "-";
    }

    Result result(this, Result::Type::String, resultStr.str());

    return result;
}

}
