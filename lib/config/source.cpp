#include "source.h"

#include "utils/utils.h"
#include <spdlog/spdlog.h>

namespace dome {
namespace config {

Source::DataType StrToDataType(const std::string& str)
{
    if (str == "float") {
        return Source::DataType::Float;
    }
    else if (str == "path") {
        return Source::DataType::Path;
    }
    else if (str == "int") {
        return Source::DataType::Int;
    }

    return Source::DataType::Undefined;
}

Source::Source(const std::string &id, const std::string &type, const std::string &dataType)
    : m_id(id)
    , m_type(Source::StrToType(type))
    , m_dataType(StrToDataType(dataType))
{
    spdlog::trace("{}:{} {} id={}, type={}, dataType={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            id, type, dataType);

}

Source::~Source()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

}

std::string Source::id() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_id;
}

Source::Type Source::type() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_type;
}

Source::DataType Source::dataType() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_dataType;
}

std::string Source::TypeToStr(Source::Type type)
{
    spdlog::trace("{}:{} {}, type={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            type);
    
    switch (type) {
        case Source::Type::Temperature:
            return "temperature";
        case Source::Type::Humidity:
            return "humidity";
        case Source::Type::Command:
            return "command";
        case Source::Type::IpCamera:
            return "ip_camera";
        case Source::Type::Co2:
            return "co2";
    }

    return "undefined";
}

Source::Type Source::StrToType(const std::string &str)
{
    spdlog::trace("{}:{} {}, str={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            str);

    if (str == "temperature") {
        return Source::Type::Temperature;
    }
    else if (str == "humidity") {
        return Source::Type::Humidity;
    }
    else if (str == "command") {
        return Source::Type::Command;
    }
    else if (str == "ip_camera") {
        return Source::Type::IpCamera;
    }
    else if (str == "co2") {
        return Source::Type::Co2;
    }

    return Source::Type::Undefined;
}

}
}
