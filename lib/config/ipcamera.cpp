#include "ipcamera.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

IpCamera::Raw::Raw(const std::string &name,
                   const std::string &login,
                   const std::string &pass,
                   const std::string &ip,
                   const std::string &port)
    : m_name(name)
    , m_login(login)
    , m_pass(pass)
    , m_ip(ip)
    , m_port(port)
{
    spdlog::trace("{}:{} {} name={} login={} pass={} ip={} port={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name, login, pass, ip, port);
}

IpCamera::IpCamera(const std::string &configPath)
    : Config(configPath)
{
    spdlog::trace("{}:{} {} configPath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, configPath);

    parseConfig();
}

const std::vector<IpCamera::Raw> IpCamera::ipCameraConfig() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_ipCameras;
}


void IpCamera::parseConfig()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = readConfigFile();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jIpCameras = jConfig["ip_cameras"];
    for(const auto &jIpCamera : jIpCameras) {
        if (jIpCamera.size() != 5) {
            spdlog::error("Invalid ipcamera format: \" {} \"", jIpCamera.dump());
            continue;
        }

        std::string name = jIpCamera["name"].get<std::string>();
        std::string login = jIpCamera["login"].get<std::string>();
        std::string pass = jIpCamera["pass"].get<std::string>();
        std::string ip = jIpCamera["ip"].get<std::string>();
        std::string port = jIpCamera["port"].get<std::string>();

        spdlog::info("IP Camera config: name={} login={} pass={} ip={} port={}",
            name, login, pass, ip, port);

        m_ipCameras.push_back(
            Raw(name, login, pass, ip, port)
        );
    }
}

}
}
