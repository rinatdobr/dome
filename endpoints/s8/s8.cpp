#include "s8.h"

#include <message/message.h>
#include <spdlog/spdlog.h>
#include <utils/utils.h>
#include <sstream>

namespace dome {
namespace data {

S8::S8(const dome::config::EndPoint &endPointConfig)
    : m_endPointConfig(endPointConfig)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    I_am_valid();
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
    jData["type"] = dome::message::type::Data;
    for (const auto &source : m_endPointConfig.sources()) {
        if (source.type() == dome::config::Source::Type::Co2) {
            jData[source.id()] = m_co2;
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
