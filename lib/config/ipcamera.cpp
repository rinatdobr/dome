#include "ipcamera.h"
#include "utils.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

IpCamera::IpCamera(const std::string &path)
    : File(path)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, path);

    parse();
}

std::string IpCamera::id() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_id;
}

std::string IpCamera::name() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_name;
}

std::string IpCamera::login() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_login;
}

std::string IpCamera::pass() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_pass;
}

std::string IpCamera::ip() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_ip;
}

uint IpCamera::port() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_port;
}

void IpCamera::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    nlohmann::json jIpCamera = jConfig["ip_camera"];
    if (jIpCamera.size() != 6) {
        spdlog::error("Invalid ip camera format: \" {} \"", jIpCamera.dump());
        return;
    }
    m_id = jIpCamera["id"].get<std::string>();
    m_name = jIpCamera["name"].get<std::string>();
    m_login = jIpCamera["login"].get<std::string>();
    m_pass = jIpCamera["pass"].get<std::string>();
    m_ip = jIpCamera["ip"].get<std::string>();
    m_port = jIpCamera["port"].get<uint>();

    spdlog::info("IpCamera config: id={} name={} login={} pass={} ip={} port={}",
                    m_id, m_name, m_login, m_pass, m_ip, m_port);
}

}
}
