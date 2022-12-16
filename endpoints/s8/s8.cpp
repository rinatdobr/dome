#include "s8.h"

#include <spdlog/spdlog.h>
#include <utils/utils.h>
#include <sstream>

namespace dome {
namespace data {

S8::S8(const dome::config::Provider &config)
    : m_config(config)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

S8::~S8()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

bool S8::prepareData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::ostringstream cmd;
    cmd << "python" << " "
    << "senseairs8.py";

    auto execRes = Exec(cmd.str().c_str());
    if (execRes.first == 0) {
        spdlog::debug("co2: {}", execRes.second);
        m_co2 = std::stoi(execRes.second);
        return true;
    }
    else {
        spdlog::error("Can't get co2: {}", execRes.first);
        return false;
    }
}

nlohmann::json S8::getData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    nlohmann::json jData;
    jData["type"] = "data";
    for (const auto &source : m_config.sources()) {
        if (source.type == dome::config::Source::Type::Co2) {
            jData[source.id] = m_co2;
        }
    }

    return jData;
}

bool S8::isDataLeft()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return false;
}

}
}
